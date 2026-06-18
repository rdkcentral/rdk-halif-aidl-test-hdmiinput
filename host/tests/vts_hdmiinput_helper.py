#!/usr/bin/env python3
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2024 RDK Management
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
import tarfile
import yaml
import time

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.core.logModule import logModule
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

HDMIINPUT_TEST_MENU_CONFIGURATION_FILE = os.path.join(dir_path, "vts_hdmiinput_testMenuConfiguration.yaml")
HDMIINPUT_TESTCASES_CONFIGURATION_FILE = os.path.join(os.path.join(dir_path, "../configs"), "vts_hdmiinput_testcase_configuration.yaml")
HDMIINPUT_L3_TEST_CONFIG_FILE = os.path.join(os.path.join(dir_path, "../../test_configuration"), "vts_l3_HdmiInput.yaml")

class hdmiInputHelperClass(utHelperClass):

    def __init__(self, testName:str, qcId:str, log:logModule=None, hfpfile:str=None, testMenuConfigFile:str=None, moduleName:str=None, artifactDirPath:str=None ):
        """
        Initializes the test class
        """
        if moduleName is None:
            self.moduleName = "hdmiinput"
        else:
            self.moduleName = moduleName

        self.testLevel = "L3"
        super().__init__(testName, qcId, log)

        self.log = log
        if log is None:
            self.log = logModule(self.__class__.__name__)
            self.log.setLevel(self.log.INFO)

        # Parse device configuration
        self.deviceTestSetup = self.cpe.get("test")
        self.targetWorkspace = os.path.join(self.cpe.get("target_directory"), self.moduleName).replace("\\", "/")
        
        profileFileFromConfig = self.deviceTestSetup.get("profile")
        profileFileTmp = hfpfile or profileFileFromConfig
        self.moduleHFPFile = os.path.join(dir_path, profileFileTmp).replace("\\", "/")
        if moduleName is None:
            # Individual test case
            profileOnTarget = os.path.join(self.targetWorkspace, "profiles")
            profileOnTarget = os.path.join(profileOnTarget, os.path.basename(self.moduleHFPFile))
        else:
            # Unified test case
            profileOnTarget = os.path.join(self.targetWorkspace, "bin", "profiles")
            profileOnTarget = os.path.join(profileOnTarget, os.path.basename(profileFileFromConfig))
        profileOnTarget = profileOnTarget.replace("\\", "/")

        # Load test menu and test case configuration files
        self.l3TestConfigFile = HDMIINPUT_L3_TEST_CONFIG_FILE.replace("\\", "/")
        if testMenuConfigFile is not None:
            # Unified test menu configuration file for all HDMI CEC tests
            hdmiInputTestMenuConfigFile = testMenuConfigFile
        else:
            # Individual test menu configuration file for HDMI CEC tests
            hdmiInputTestMenuConfigFile = HDMIINPUT_TEST_MENU_CONFIGURATION_FILE
        
        self.testMenuConfiguration = ConfigRead(hdmiInputTestMenuConfigFile, self.moduleName)
        if moduleName is None:
            # Only needed while running individual tests
            self.testCaseConfiguration = ConfigRead(HDMIINPUT_TESTCASES_CONFIGURATION_FILE, self.moduleName)
        self.testMenuConfiguration.test.execute = self.testMenuConfiguration.test.execute + f" -p {profileOnTarget}"

        # Establish HAL session
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test_0")

        # Initialize UT menu navigator and utilities
        self.utMenu = UTSuiteNavigatorClass(self.testMenuConfiguration, None, self.hal_session)
        self.utilities = utBaseUtils()

        # Copy required artifacts to target device
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)
        for artifact in self.testMenuConfiguration.test.artifacts:
            if artifactDirPath is None:
                filesPath = os.path.join(dir_path, artifact).replace("\\", "/")
            else:
                filesPath = os.path.join(artifactDirPath, artifact).replace("\\", "/")
            self.hal_session.write("ls " + self.targetWorkspace + "/" + os.path.basename(artifact) + "\n")
            res = self.hal_session.read_until(self.hal_session.prompt, timeout=5)
            if "No such file or directory" not in res:
                continue  # File already exists on target, skip copying and continue with next artifact
            self.utilities.sftpCopy(self.hal_session, filesPath, self.targetWorkspace)

            if tarfile.is_tarfile(filesPath):
                target_tarfile = os.path.join(self.targetWorkspace, os.path.basename(artifact))
                target_tarfile = target_tarfile.replace('\\', '/')
                self.utilities.untar(self.hal_session, target_tarfile, self.targetWorkspace)

        if moduleName is None:
            self.utilities.change_directory(self.hal_session, self.targetWorkspace)
            time.sleep(2)
        else:
            target_workspace = os.path.join(self.targetWorkspace, "bin")
            target_workspace = target_workspace.replace("\\", "/")
            self.targetWorkspace = target_workspace
        self.moduleName = "hdmiinput"

    def hdmiInputReadYaml(self, yaml_path: str) -> dict:
        """
        Reads a YAML file and returns its content as a dictionary.
        Args:
            yaml_path (str): Path to the YAML file
        Returns:
            dict: Content of the YAML file as a dictionary
        """
        with open(yaml_path, "r") as f:
            return yaml.safe_load(f)

    def updateOpenConnectionTestConfiguration(self, testName, sourceHDCPVersion):
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
        template["source_HDCP_Version"] = sourceHDCPVersion

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

    def updateStartConnectionTestConfiguration(self, testName, port_ids, sourceHDCPVersion):
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
        template["source_HDCP_Version"] = sourceHDCPVersion

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

                # Send HDCP statuses if connect true and signal locked
                if conn_state is True and signal_state == "LOCKED" and port['source_HDCP_Version'] != "UNDEFINED":
                    self.hdmiController.setHDCPStatus(port_id, port['source_HDCP_Version'], 'AUTHENTICATION_IN_PROGRESS')
                    self.hdmiController.setHDCPStatus(port_id, port['source_HDCP_Version'], 'AUTHENTICATED')
                time.sleep(1)

    def hdmiInputWriteYaml(self, yaml_path: str, data: dict):
        """
        Writes a dictionary to a YAML file.
        Args:
            yaml_path (str): Path to the YAML file
            data (dict): Content to write to the YAML file
        """
        with open(yaml_path, "w") as f:
            yaml.dump(data, f, sort_keys=False, default_flow_style=None)

    def testEndFunction(self, powerOff=True):
        """
        Ends the test case.
        Args:
            powerOff (bool): Whether to power off the device
        """
        super().testEndFunction(powerOff)