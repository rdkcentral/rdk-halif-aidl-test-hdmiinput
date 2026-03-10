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
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse
from vts_hdmiinput_helper import hdmiInputHelperClass

HDMIINPUT_L3_TEST_SUITE_NAME = "VTS_L3_HDMIINPUT"

class  VTS_HDMIINPUT_L3_EdidTest(hdmiInputHelperClass):
    """
    Runs L3 EDID test
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class
        Args:
            log (logModule, optional): Log module instance. Defaults to None.
        Returns:
            None
        """
        self.testName = "HdmiInput_L3_EdidTest"
        qcId = "1"
        super().__init__(self.testName, qcId, log)

        self.hdmiController = HdmiController(self.log, self.dut.rawConfig.get("dut").get("hdmiController"))

    def testFunction(self):
        """
        The main test function to run the L3 EDID test.
        Args:
            None
        Returns:
            bool: Final result of the test.
        """
        self.testcase = "EDIDTest"
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)

        self.testUserResponse = utUserResponse()

        # Read profile file
        self.test_config_data = self.hdmiInputReadYaml(self.l3TestConfigFile)

        self.utMenu.start()

        # Loop through ports and set connection/signal status
        ports = self.test_config_data[self.moduleName][self.testLevel][self.testcase]['ports']
        for port in ports:
            port_id = port.get('id')
            self.hdmiController.connectDevice(port_id, True)
            self.hdmiController.setSignalStatus(port_id, "LOCKED")

        self.log.stepStart(f'Test Suit: {HDMIINPUT_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
        output = self.utMenu.select(HDMIINPUT_L3_TEST_SUITE_NAME, self.testcase, None, 40, False)

        results = self.utMenu.collect_results(output, True)
        if results is None:
            results = False

        ports = self.test_config_data[self.moduleName][self.testLevel][self.testcase]['ports']
        for port in ports:
            port_id = port.get('id')
            results &= self.hdmiController.validateEdid(port_id, port.get('edid', []))

        self.log.stepResult(results, f'Test Suit: {HDMIINPUT_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')

        self.utMenu.stop()

        return results

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summary"
    summaryLog = logModule(summaryLogName, level=logModule.INFO)
    test = VTS_HDMIINPUT_L3_EdidTest(summaryLog)
    test.run(False)