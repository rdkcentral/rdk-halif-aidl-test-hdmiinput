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

from abc import ABC, abstractmethod

from framework.core.logModule import logModule

class HdmiInterface(ABC):

    def __init__(self, logger:logModule):
        """
        Abstract base class for HDMI controllers. Defines the interface for HDMI control and monitoring.
        """
        self._log = logger

    @abstractmethod
    def setHDCPVersion(self, port: int, hdcp_version: str):
        """
        Set the HDCP version for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            hdcp_version (str): HDCP version string. (VERSION_1_X, VERSION_2_X, UNDEFINED)
        Returns:
            bool: True if HDCP version set successfully.
        """
        pass

    @abstractmethod
    def validateEdid(self, port: int, expected_edid: list):
        """
        Validate the EDID data for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            expected_edid (list): Expected EDID data bytes.
        Returns:
            bool: True if EDID data matches expected values.
        """
        pass

    @abstractmethod
    def sendAudioInfoFrame(self, port: int, data: list):
        """
        Abstract method to send an Audio Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): Audio info frame data bytes.
        """
        pass

    @abstractmethod
    def sendAVIInfoFrame(self, port: int, data: list):
        """
        Abstract method to send an AVI Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): AVI info frame data bytes.
        """
        pass

    @abstractmethod
    def sendDRMInfoFrame(self, port: int, data: list):
        """
        Abstract method to send a DRM Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): DRM info frame data bytes.
        """
        pass

    @abstractmethod
    def connectDevice(self, port: int, connected: bool):
        """
        Abstract method to set the connection status for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            connected (bool): Connection status (True for connected, False for disconnected).
        """
        pass

    @abstractmethod
    def setHDCPStatus(self, port: int, hdcp_version: str, authenticated: str):
        """
        Abstract method to set the HDCP status for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            hdcp_version (str): HDCP version string.
            authenticated (str): HDCP authentication status.
        """
        pass

    @abstractmethod
    def setSignalStatus(self, port: int, signal_state: str):
        """
        Abstract method to set the signal status for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            signal_state (str): Signal state string (e.g., 'LOCKED', 'NO_SIGNAL').
        """
        pass

    @abstractmethod
    def sendSPDInfoFrame(self, port: int, data: list):
        """
        Abstract method to send an SPD Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): SPD info frame data bytes.
        """
        pass

    @abstractmethod
    def sendVSIFInfoFrame(self, port: int, data: list):
        """
        Abstract method to send a Vendor Specific Info Frame message to the HDMI input port.

        Args:
            port (int): HDMI input port number.
            data (list): Vendor specific info frame data bytes.
        """
        pass

    @abstractmethod
    def SetVIC(self, port: int, vic: str):
        """
        Abstract method to set the Video Identification Code (VIC) for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            vic (str): Video Identification Code string.
        """
        pass

    @abstractmethod
    def setVRRStatus(self, port: int, vrrActive: bool,M_CONST: bool, fastVActive: bool, frameRate: float):
        """
        Abstract method to set the Variable Refresh Rate (VRR) status for the HDMI input port.

        Args:
            port (int): HDMI input port number.
            vrrActive (bool): VRR enabled status.
            M_CONST (bool): M_CONST status.
            fastVActive (bool): Fast V Active status.
            frameRate (float): Frame rate value.
        """
        pass

    @abstractmethod
    def start(self):
        """
        Abstract method to start the HDMI controller.
        """
        pass

    @abstractmethod
    def stop(self):
        """
        Abstract method to stop the HDMI controller.
        """
        pass
