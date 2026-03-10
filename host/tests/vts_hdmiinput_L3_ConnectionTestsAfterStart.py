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

class  VTS_HDMIINPUT_L3_ConnectionTestsAfterStart(hdmiInputHelperClass):
    """
    Runs L3 ConnectionTestsAfterStart test
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class
        Args:
            log (logModule, optional): Log module instance. Defaults to None.
        Returns:
            None
        """
        self.sourceHDCPVersions = ["UNDEFINED", "VERSION_1_X", "VERSION_2_X"]
        self.testName = "HdmiInput_L3_ConnectionTestsAfterStart"
        qcId = "1"
        super().__init__(self.testName, qcId, log)

        self.hdmiController = HdmiController(self.log, self.dut.rawConfig.get("dut").get("hdmiController"))

    def testFunction(self):
        """
        The main test function to run the L3 ConnectionTestsAfterStart.
        Args:
            None
        Returns:
            bool: Final result of the test.
        """
        self.testcase = "ConnectionTestAfterStart"
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)

        final_results = True

        for sourceHDCPVersion in self.sourceHDCPVersions:
            # Read profile file to get port list and maximumConcurrentStartedPorts
            profile_data = self.hdmiInputReadYaml(self.moduleHFPFile)
            port_list = profile_data[self.moduleName]['ports']
            port_ids = [port['id'] for port in port_list]
            max_ports = profile_data['hdmiinput'].get('platformCapabilities', {}).get('maximumConcurrentStartedPorts', 1)

            # Loop through ports in groups of max_ports
            for i in range(0, len(port_ids), max_ports):
                current_ports = port_ids[i:i+max_ports]
                self.updateStartConnectionTestConfiguration(self.testcase, current_ports, sourceHDCPVersion)

                self.connectionThreadObj = threading.Thread(target=self.connectionThread)
                self.utMenu.start()

                ports = self.test_config_data[self.moduleName][self.testLevel][self.testcase]['ports']
                for port in ports:
                    port_id = port.get('id')
                    conn_state = port.get('connection_state_before_open', False)
                    self.hdmiController.connectDevice(port_id, conn_state)
                    signal_state = port.get('signal_state_before_open', "UNKNOWN")
                    self.hdmiController.setSignalStatus(port_id, signal_state)
                    self.hdmiController.setHDCPVersion(port_id, sourceHDCPVersion)

                self.connectionThreadObj.start()

                self.log.stepStart(f'Test Suit: {HDMIINPUT_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
                output = self.utMenu.select(HDMIINPUT_L3_TEST_SUITE_NAME, self.testcase, None, 40, False)

                group_results = self.utMenu.collect_results(output, True)
                if group_results is None:
                    group_results = False

                self.connectionThreadObj.join()

                self.log.stepResult(group_results, f'Test Suit: {HDMIINPUT_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
                self.utMenu.stop()

                final_results = final_results and group_results

        return final_results

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summary"
    summaryLog = logModule(summaryLogName, level=logModule.INFO)
    test = VTS_HDMIINPUT_L3_ConnectionTestsAfterStart(summaryLog)
    test.run(False)