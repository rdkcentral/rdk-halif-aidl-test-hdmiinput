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

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(dir_path)

from hdmiModules.virtualHdmiController import virtualHdmiController
from hdmiModules.manualHdmiController import manualHdmiController

# Add other controller imports here as needed

class HdmiController:
    """
    High-level HDMI input controller, similar to HDMICECController.
    Instantiates the correct controller type based on config.
    """
    def __init__(self, log, config: dict):
        self._log = log
        self.controllerType = config.get('type', 'virtual-hdmi-controller')

        if self.controllerType == 'virtual-hdmi-controller':
            self.controller = virtualHdmiController(self._log,
                                                    address=config.get('address'),
                                                    username=config.get('username',''),
                                                    password=config.get('password',''),
                                                    port=config.get('port',22),
                                                    prompt=config.get('prompt', '~#'),
                                                    control_port=config.get('control_port', 8080))
        elif self.controllerType == 'manual-hdmi-controller':
            self.controller = manualHdmiController(self._log,
                                                    address=config.get('address'),
                                                    username=config.get('username',''),
                                                    password=config.get('password',''),
                                                    port=config.get('port',22),
                                                    prompt=config.get('prompt', '~#'),
                                                    control_port=config.get('control_port', 8080))
        # Add other controller types here if needed

    def sendAudioInfoFrame(self, port: int, data: list):
        """
        Send an Audio Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): Audio info frame data bytes.

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.sendAudioInfoFrame(port, data)

    def sendAVIInfoFrame(self, port: int, data: list):
        """
        Send an AVI Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): AVI info frame data bytes.

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.sendAVIInfoFrame(port, data)

    def sendDRMInfoFrame(self, port: int, data: list):
        """
        Send a DRM Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): DRM info frame data bytes.

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.sendDRMInfoFrame(port, data)

    def connectDevice(self, port: int, connected: bool):
        """
        Set the connection status for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            connected (bool): Connection status (True for connected, False for disconnected).

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.connectDevice(port, connected)

    def setHDCPStatus(self, port: int, hdcp_version: str, authenticated: str):
        """
        Set the HDCP status for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            hdcp_version (str): HDCP version string.
            authenticated (str): HDCP authentication status.

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.setHDCPStatus(port, hdcp_version, authenticated)

    def setSignalStatus(self, port: int, signal_state: str):
        """
        Set the signal status for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            signal_state (str): Signal state string (e.g., 'LOCKED', 'NO_SIGNAL').

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.setSignalStatus(port, signal_state)

    def sendSPDInfoFrame(self, port: int, data: list):
        """
        Send an SPD Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): SPD info frame data bytes.

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.sendSPDInfoFrame(port, data)

    def sendVSIFInfoFrame(self, port: int, data: list):
        """
        Send a Vendor Specific Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): Vendor specific info frame data bytes.

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.sendVSIFInfoFrame(port, data)

    def SetVIC(self, port: int, vic: str):
        """
        Set the Video Identification Code (VIC) for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            vic (str): Video Identification Code string.

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.SetVIC(port, vic)

    def setVRRStatus(self, port: int, vrrActive: bool,M_CONST: bool, fastVActive: bool, frameRate: float):
        """
        Abstract method to set the Variable Refresh Rate (VRR) status for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            vrrActive (bool): VRR enabled status.
            M_CONST (bool): M_CONST status.
            fastVActive (bool): Fast V Active status.
            frameRate (float): Frame rate value.

        Returns:
            bool: True if message sent successfully.
        """
        return self.controller.setVRRStatus(port, vrrActive, M_CONST, fastVActive, frameRate)

    def start(self):
        """
        Start the HDMI controller.

        Returns:
            None
        """
        return self.controller.start()

    def stop(self):
        """
        Stop the HDMI controller.

        Returns:
            None
        """
        return self.controller.stop()
