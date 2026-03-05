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

class  VTS_HDMIINPUT_L3_SendMessage(hdmiInputHelperClass):
    """
    Runs L3 SendMessage test
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class
        """
        self.testName = "HdmiInput_L3_ConnectionTests"
        qcId = "1"
        super().__init__(self.testName, qcId, log)

        self.hdmiController = HdmiController(self.log, self.dut.rawConfig.get("dut").get("hdmiController"))

    def testFunction(self):
        """
        The main test function to run the L3 SendMessage.

        Returns:
            bool: Final result of the test.
        """
        self.testcase = "ConnectionTestAfterOpen"
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)

        self.utMenu.start()

        return True

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summary"
    summaryLog = logModule(summaryLogName, level=logModule.INFO)
    test = VTS_HDMIINPUT_L3_SendMessage(summaryLog)
    test.run(False)