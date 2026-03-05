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
from framework.core.utPlaneController import utPlaneController
import yaml

class virtualHdmiController(HdmiInterface):
    """
    Virtual HDMI Controller for HDMI input vcomponent.
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

            self.utPlaneController = utPlaneController(self.session, port=self.control_port)

        except Exception as e:
            self._log.critical(f"Failed to load device configuration: {e}")
            raise

    def sendAudioInfoFrame(self, port: int, data: list):
        """
        Send an Audio Info Frame message to the HDMI input port using a YAML template.
        """
        with open(AUDIO_INFOFRAME_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['data'] = data
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def sendAVIInfoFrame(self, port: int, data: list):
        """
        Send an AVI Info Frame message to the HDMI input port using a YAML template.
        """
        with open(AVI_INFOFRAME_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['data'] = data
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def sendDRMInfoFrame(self, port: int, data: list):
        """
        Send a DRM Info Frame message to the HDMI input port using a YAML template.
        """
        with open(DRM_INFOFRAME_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['data'] = data
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def connectDevice(self, port: int, connected: bool):
        """
        Set the connection status for the HDMI input port using a YAML template.
        """
        with open(CONNECTION_STATUS_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['connected'] = connected
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def setHDCPStatus(self, port: int, hdcp_version: str, status: str):
        """
        Set the HDCP status for the HDMI input port using a YAML template.
        """
        with open(HDCP_STATUS_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['hdcp_version'] = hdcp_version
        msg['hdmiinput']['params']['status'] = status
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def setSignalStatus(self, port: int, signal_state: str):
        """
        Set the signal status for the HDMI input port using a YAML template.
        """
        with open(SIGNAL_STATUS_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['signal_state'] = signal_state
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def sendSPDInfoFrame(self, port: int, data: list):
        """
        Send an SPD Info Frame message to the HDMI input port using a YAML template.
        """
        with open(SPD_INFOFRAME_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['data'] = data
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def sendVSIFInfoFrame(self, port: int, data: list):
        """
        Send a Vendor Specific Info Frame message to the HDMI input port using a YAML template.
        """
        with open(VENDOR_SPECIFIC_INFOFRAME_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['data'] = data
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def SetVIC(self, port: int, vic: str):
        """
        Set the Video Identification Code (VIC) for the HDMI input port using a YAML template.
        """
        with open(VIDEO_FORMAT_CHANGE_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['vic'] = vic
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

    def setVRRStatus(self, port: int, vrrActive: bool,M_CONST: bool, fastVActive: bool, frameRate: float):
        """
        Set the Variable Refresh Rate (VRR) status for the HDMI input port using a YAML template.
        """
        with open(VRR_STATUS_CMD_TEMPLATE, 'r') as f:
            msg = yaml.safe_load(f)
        msg['hdmiinput']['params']['port'] = port
        msg['hdmiinput']['params']['vrrActive'] = vrrActive
        msg['hdmiinput']['params']['M_CONST'] = M_CONST
        msg['hdmiinput']['params']['fastVActive'] = fastVActive
        msg['hdmiinput']['params']['frameRate'] = frameRate
        yaml_str = yaml.dump(msg)
        return self.utPlaneController.sendMessage(yaml_str)

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
