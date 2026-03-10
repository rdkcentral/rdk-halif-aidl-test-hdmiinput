#!/usr/bin/env python3
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2023 RDK Management
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
import re
import yaml

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(dir_path)
sys.path.append(os.path.join(dir_path, "../../raft/"))

command_templates_dir = os.path.join(dir_path, 'commands')
AUDIO_INFOFRAME_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_audioinfo_frame.yaml')
AVI_INFOFRAME_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_aviinfo_frame.yaml')
DRM_INFOFRAME_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_drminfo_frame.yaml')
CONNECTION_STATUS_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_connection_status.yaml')
HDCP_STATUS_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_hdcp_status.yaml')
SIGNAL_STATUS_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_signal_status.yaml')
SPD_INFOFRAME_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_spdinfo_frame.yaml')
VENDOR_SPECIFIC_INFOFRAME_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_vendorspecificinfo_frame.yaml')
VIDEO_FORMAT_CHANGE_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_videoformat_change.yaml')
VRR_STATUS_CMD_TEMPLATE = os.path.join(command_templates_dir, 'hdmiinput_vrr_status.yaml')

from framework.core.logModule import logModule
from framework.core.commandModules.sshConsole import sshConsole
from abstractHdmiController import HdmiInterface
from framework.plugins.ut_raft.utUserResponse import utUserResponse
import yaml

class manualHdmiController(HdmiInterface):
    """
    Manual HDMI Controller for HDMI input vcomponent.
    Sends specific HDMI input messages to vcomponent using utPlaneController and YAML format constructed in code.
    """
    def __init__(self, logger: logModule, address: str, username: str,
                password: str, port: int = 22, prompt: str = '~#', control_port: int = 8080):
        """
        Initializes the virtualHdmiController class for HDMI device communication.

        Args:
            logger (logModule): Logger module instance for logging operations.
            address (str): IP address or hostname of the remote device.
            username (str, optional): SSH username for authentication. Defaults to ''.
            password (str, optional): SSH password for authentication. Defaults to ''.
            port (int, optional): SSH port number for connection. Defaults to 22.
            prompt (str, optional): Command prompt string for the SSH session. Defaults to '~#'.
            device_configuration (str, optional): Path to the HDMI device network configuration YAML file. Defaults to ''.
            control_port (int, optional): Port number for ut-controller communication. Defaults to 8080.

        """
        super().__init__(logger)

        self.control_port = control_port
        self.commandPrompt = prompt

        try:
            self.session = sshConsole(self._log, address, username, password, port=port, prompt=prompt)

            self.testUserResponse = utUserResponse()

        except Exception as e:
            self._log.critical(f"Failed to load device configuration: {e}")
            raise

    def setHDCPVersion(self, port: int, hdcp_version: str):
        """
        Set the HDCP version for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            hdcp_version (str): HDCP version string. (VERSION_1_X, VERSION_2_X, UNDEFINED)
        Returns:
            bool: True if HDCP version set successfully.
        """
        results = self.testUserResponse.getUserYN(f"Set HDCP version '{hdcp_version}' for HDMI input port {port}? (Y/N):")
        return results

    def validateEdid(self, port: int, expected_edid: list):
        """
        Validate the EDID data for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            expected_edid (list): Expected EDID data bytes.
        Returns:
            bool: True if EDID data matches expected values.
        """
        results = self.testUserResponse.getUserYN(f"Validate EDID data for HDMI input port {port} against expected data {expected_edid}? (Y/N):")
        return results

    def sendAudioInfoFrame(self, port: int, data: list):
        """
        Ask user to manually set Audio Info Frame for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set Audio Info Frame for HDMI input port {port} with data {data}? (Y/N):")
        return results

    def sendAVIInfoFrame(self, port: int, data: list):
        """
        Ask user to manually set AVI Info Frame for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set AVI Info Frame for HDMI input port {port} with data {data}? (Y/N):")
        return results

    def sendDRMInfoFrame(self, port: int, data: list):
        """
        Ask user to manually set DRM Info Frame for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set DRM Info Frame for HDMI input port {port} with data {data}? (Y/N):")
        return results

    def connectDevice(self, port: int, connected: bool):
        """
        Ask user to manually connect/disconnect the HDMI input port and confirm.
        """
        action = 'connect' if connected else 'disconnect'
        results = self.testUserResponse.getUserYN(f"Please {action} HDMI input port {port}? (Y/N):")
        return results

    def setHDCPStatus(self, port: int, hdcp_version: str, status: str):
        """
        Ask user to manually set HDCP status for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set HDCP status '{status}' (version: {hdcp_version}) for HDMI input port {port}? (Y/N):")
        return results

    def setSignalStatus(self, port: int, signal_state: str):
        """
        Ask user to manually set signal status for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set signal status '{signal_state}' for HDMI input port {port}? (Y/N):")
        return results

    def sendSPDInfoFrame(self, port: int, data: list):
        """
        Ask user to manually set SPD Info Frame for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set SPD Info Frame for HDMI input port {port} with data {data}? (Y/N):")
        return results

    def sendVSIFInfoFrame(self, port: int, data: list):
        """
        Ask user to manually set Vendor Specific Info Frame for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set Vendor Specific Info Frame for HDMI input port {port} with data {data}? (Y/N):")
        return results

    def SetVIC(self, port: int, vic: str):
        """
        Ask user to manually set VIC for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set VIC '{vic}' for HDMI input port {port}? (Y/N):")
        return results

    def setVRRStatus(self, port: int, vrrActive: bool, M_CONST: bool, fastVActive: bool, frameRate: float):
        """
        Ask user to manually set VRR status for the HDMI input port and confirm.
        """
        results = self.testUserResponse.getUserYN(f"Set VRR status for HDMI input port {port} with vrrActive={vrrActive}, M_CONST={M_CONST}, fastVActive={fastVActive}, frameRate={frameRate}? (Y/N):")
        return results

    def start(self):
        """
        Start the virtual HDMI controller (stub).

        Returns:
            None
        """
        pass

    def stop(self):
        """
        Stop the virtual HDMI controller (stub).

        Returns:
            None
        """
        pass
