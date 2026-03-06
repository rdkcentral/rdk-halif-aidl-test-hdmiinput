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

class  VTS_HDMIINPUT_L3_ConnectionTestsAfterOpen(hdmiInputHelperClass):
    """
    Runs L3 ConnectionTestsAfterOpen test
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class
        Args:
            log (logModule, optional): Log module instance. Defaults to None.
        Returns:
            None
        """
        self.testName = "HdmiInput_L3_ConnectionTestsAfterOpen"
        qcId = "1"
        super().__init__(self.testName, qcId, log)

        self.hdmiController = HdmiController(self.log, self.dut.rawConfig.get("dut").get("hdmiController"))

    def updateConnectionTestConfiguration(self, testName):
        """
        Updates the connection test configuration for the specified test.

        Args:
            testName (str): Name of the test case to update.

        Returns:
            None
        """
        profile_path = self.moduleHFPFile

        # Read profile file
        self.test_config_data = self.hdmiInputReadYaml(self.l3TestConfigFile)
        profile_data = self.hdmiInputReadYaml(profile_path)

        port_list = profile_data[self.moduleName]['ports']
        port_ids = [port['id'] for port in port_list]

        # Get the template from the first port entry
        ports_section = self.test_config_data[self.moduleName][self.testLevel][testName]['ports']
        template = ports_section[0].copy()
        # Remove 'id' field from template
        template.pop('id', None)

        # Create new ports section
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

    def connectionThread(self):
        """
        Thread function to handle HDMI connection and signal status updates.
        """
        time.sleep(10)
        ports = self.test_config_data[self.moduleName][self.testLevel][self.testcase]['ports']
        for port in ports:
            conn_states = port.get('connection_state', [])
            signal_states = port.get('signal_state', [])
            port_id = port.get('id')
            for conn_state, signal_state in zip(conn_states, signal_states):
                self.hdmiController.connectDevice(port_id, conn_state)
                # Set signal status
                self.hdmiController.setSignalStatus(port_id, signal_state)

                # Send HDCP statuses if connect true
                if conn_state is True:
                    self.hdmiController.setHDCPStatus(port_id, port['source_HDCP_Version'], 'AUTHENTICATION_IN_PROGRESS')
                    self.hdmiController.setHDCPStatus(port_id, port['source_HDCP_Version'], 'AUTHENTICATED')

                time.sleep(1)

    def testFunction(self):
        """
        The main test function to run the L3 ConnectionTestsAfterOpen.
        Args:
            None
        Returns:
            bool: Final result of the test.
        """
        self.testcase = "ConnectionTestAfterOpen"
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)

        self.updateConnectionTestConfiguration(self.testcase)

        self.connectionThreadObj = threading.Thread(target=self.connectionThread)

        self.utMenu.start()

        # Loop through ports and set connection/signal status
        ports = self.test_config_data[self.moduleName][self.testLevel][self.testcase]['ports']
        for port in ports:
            port_id = port.get('id')
            conn_state = port.get('connection_state_before_open', False)
            self.hdmiController.connectDevice(port_id, conn_state)
            signal_state = port.get('signal_state_before_open', "UNKNOWN")
            self.hdmiController.setSignalStatus(port_id, signal_state)

        self.connectionThreadObj.start()

        self.log.stepStart(f'Test Suit: {HDMIINPUT_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
        output = self.utMenu.select(HDMIINPUT_L3_TEST_SUITE_NAME, self.testcase, None, 40, False)

        results = self.utMenu.collect_results(output, True)
        if results is None:
            results = False

        self.connectionThreadObj.join()

        self.log.stepResult(results, f'Test Suit: {HDMIINPUT_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')

        self.utMenu.stop()

        return results

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summary"
    summaryLog = logModule(summaryLogName, level=logModule.INFO)
    test = VTS_HDMIINPUT_L3_ConnectionTestsAfterOpen(summaryLog)
    test.run(False)