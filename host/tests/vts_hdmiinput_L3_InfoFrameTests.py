#!/usr/bin/env python3
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2026 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *
# http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
#* ******************************************************************************

import os
import sys
import time
import threading

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from hdmi_controller.hdmiController import HdmiController
from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.core.logModule import logModule
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils
from vts_hdmiinput_helper import hdmiInputHelperClass

HDMIINPUT_L3_TEST_SUITE_NAME = "VTS_L3_HDMIINPUT"

class  VTS_HDMIINPUT_L3_InfoFrameTests(hdmiInputHelperClass):
    """
    Runs L3 InfoFrameTests
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class
        Args:
            log (logModule, optional): Log module instance. Defaults to None.
        Returns:
            None
        """
        self.testName = "HdmiInput_L3_InfoFrameTests"
        qcId = "1"
        super().__init__(self.testName, qcId, log)

        self.hdmiController = HdmiController(self.log, self.dut.rawConfig.get("dut").get("hdmiController"))

    def updateConnectionTestConfiguration(self, testName, port_ids):
        '''
        Updates the connection test configuration for the specified test and port IDs.
        Args:
            testName (str): Name of the test case to update.
            port_ids (list): List of port IDs to include in the test configuration.
        Returns:
            None
        '''
        # Read profile file
        self.test_config_data = self.hdmiInputReadYaml(self.l3TestConfigFile)

        # Get the template from the first port entry
        ports_section = self.test_config_data[self.moduleName][self.testLevel][testName]['ports']
        template = ports_section[0].copy()
        template.pop('id', None)

        # Create new ports section for the selected port_ids
        new_ports = []
        for pid in port_ids:
            entry = {'id': pid}
            entry.update(template)
            new_ports.append(entry)

        # Update the config
        self.test_config_data[self.moduleName][self.testLevel][testName]['ports'] = new_ports

        # Write back to YAML file
        self.hdmiInputWriteYaml(self.l3TestConfigFile, self.test_config_data)

        target_config_path = self.targetWorkspace + "/test_configuration/"
        self.utilities.sftpCopy(self.hal_session, self.l3TestConfigFile, target_config_path)

    def infoFramesThread(self):
        """
        Thread function to handle HDMI info frames.
        Sends info frames based on info_frame_type and frame_data from self.test_config_data.
        """
        time.sleep(10)
        ports = self.test_config_data[self.moduleName][self.testLevel][self.testcase]['ports']

        for port in ports:
            port_id = port.get('id')
            info_types = port.get('info_frame_type', [])
            frame_data = port.get('frame_data', [])

            for idx, info_type in enumerate(info_types):
                data = frame_data[idx] if idx < len(frame_data) else None
                if info_type == 'AUDIO':
                    self.hdmiController.sendAudioInfoFrame(port_id, data)
                elif info_type == 'AVI':
                    self.hdmiController.sendAVIInfoFrame(port_id, data)
                elif info_type == 'SPD':
                    self.hdmiController.sendSPDInfoFrame(port_id, data)
                elif info_type == 'VSIF':
                    self.hdmiController.sendVSIFInfoFrame(port_id, data)
                elif info_type == 'VIC':
                    self.hdmiController.SetVIC(port_id, data[0] if isinstance(data, list) else data)
                elif info_type == 'VRR':
                    # VRR expects [vrrActive, mConst, fastVActive, frameRate]
                    if isinstance(data, list) and len(data) == 4:
                        self.hdmiController.setVRRStatus(port_id, data[0], data[1], data[2], data[3])
                time.sleep(1)

    def testFunction(self):
        """
        The main test function to run the L3 InfoFrameTests.
        Args:
            None
        Returns:
            bool: Final result of the test.
        """
        self.testcase = "InfoFramesTest"
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)

        # Read profile file to get port list and maximumConcurrentStartedPorts
        profile_data = self.hdmiInputReadYaml(self.moduleHFPFile)
        port_list = profile_data[self.moduleName]['ports']
        port_ids = [port['id'] for port in port_list]
        max_ports = profile_data['hdmiinput'].get('platformCapabilities', {}).get('maximumConcurrentStartedPorts', 1)

        # Loop through ports in groups of max_ports
        results = True
        for i in range(0, len(port_ids), max_ports):
            current_ports = port_ids[i:i+max_ports]
            self.updateConnectionTestConfiguration(self.testcase, current_ports)

            self.infoFramesThreadObj = threading.Thread(target=self.infoFramesThread)
            self.utMenu.start()

            ports = self.test_config_data[self.moduleName][self.testLevel][self.testcase]['ports']
            for port in ports:
                port_id = port.get('id')
                self.hdmiController.connectDevice(port_id, True)
                self.hdmiController.setSignalStatus(port_id, "LOCKED")

            self.infoFramesThreadObj.start()

            self.log.stepStart(f'Test Suit: {HDMIINPUT_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
            output = self.utMenu.select(HDMIINPUT_L3_TEST_SUITE_NAME, self.testcase, None, 40, False)

            group_results = self.utMenu.collect_results(output, True)
            if group_results is None:
                group_results = False

            self.infoFramesThreadObj.join()

            self.log.stepResult(group_results, f'Test Suit: {HDMIINPUT_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
            self.utMenu.stop()

            results = results and group_results

        return results

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summary"
    summaryLog = logModule(summaryLogName, level=logModule.INFO)
    test = VTS_HDMIINPUT_L3_InfoFrameTests(summaryLog)
    test.run(False)