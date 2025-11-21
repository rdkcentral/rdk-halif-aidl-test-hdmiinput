# HDMI Input High Level Test Specification Document

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Introduction](#introduction)
- [Module Description](#module-description)
- [L2 Test Specification](#l2-test-specification)
- [L3 Test Specification](#l3-test-specification)

## Acronyms, Terms and Abbreviations

- `HAL`    - Hardware Abstraction Layer
- `API`    - Application Programming Interface
- `AIDL`   - Android Interface Definition Language
- `L2`     - Level 2 Testing
- `L3`     - Level 3 Testing
- `NA`     - Not Applicable

## Introduction

HDMI Input HAL provides the control and monitoring of HDMI input ports on the platforms. It abstracts the behavior of HDMI like Reading the EDID, detecting HDMI Signal, HDCP authentication and info frame processing into uniform interfaces to use by middleware or apps.
It supports multiple HDMI ports, each with its own capabilities, state machine, and controller interface.  

- Hdmi Input Interface Definitions - [Hdmi input HAL](https://github.com/rdkcentral/rdk-halif-aidl/tree/main/hdmiinput/current/com/rdk/hal/hdmiinput)

- Hdmi Input HFP - [Hdmi input Overview]()


## L2 Test Specification

| Test | Description | HAL APIs |
|----|-----------|--------|
|Test the capabilities for HDMI input port|Verifies the capabilities for each HDMI input port| getCapabilities()|
|Validate property|Validate the property value for each input port|getProperty()|
|Validate multi property|Verify the multiple property value from the PropertyKVPair|getPropertyMulti()|
|Verifies the state of HDMI input state|Verifies the current HDMI input state when IHDMIInputEventListener.onStateChanged() changes |getState()|
|Verifies the EDID |Verifies the current EDID for the HDMI input port|getEDID()|
|Verifies the Default EDID |Verifies the default EDID for HDMI input port based on given HDMI version|getDefaultEDID()|
|Verifies the open for HDMI controller |Verifies open controller for each HDMI input port on OPENING state and switched to READY state| open(),IHDMIInputControllerListener(),IHDMIInputControllerListener.onConnectionStateChanged(),IHDMIInputController.stop(), close()|
|Verify states using listener |Verifies the HDMI input events for third party|open(),registerEventListener(), unregisterEventListener(), close()|
|Verify Connection state of each HDMI input |Verifies current connection state for a device attached to the HDMI input port.  |open(),IHDMIInputControllerListener.onConnectionStateChanged(), close()|
|Verifies Platform Capabilities |Gets the platform capabilities for HDMI inputs|getCapabilities()|


## L3 Test Specification

| Test | Description | HAL APIs used |
|-----|------------|-------------|
|Test the Video start for HDMI Input port|Starts video for HDMI input and mapped to video plane and audio mixed |start(),IHDMIInputControllerListener.onSignalStateChanged(),IHDMIInputEventListener.onStateChanged(), stop()|
|Verifies the HDCP status|Gets the HDCP status for each HDMI input port|start(),  getHDCPCurrentVersion(), IHDMIInputControllerListener.onHDCPStatusChanged(), stop()|
|Verifies the SPDIF info frame for HDMI input|Gets the SDP info frame for each HDMI input port |start(),  IHDMIInputControllerListener.onSPDInfoFrame(), stop()|
|Verifies the VIC(Video ID code) change for HDMI input|Verifies change in the video format from the HDMI source device. |start(),  IHDMIInputControllerListener.onVideoFormatChanged(), stop()|
|Verifies the VRR change notification for HDMI input|Verifies change in the VRR/FVA signaling from the source device decoded from the received Video |start(),  IHDMIInputControllerListener.onVRRChanged(), stop()|
|Verifies the AVI info frame event change notification for HDMI input|Verifies received InfoFrame after the HDMI input port is in STARTED state or Info frame changes |start(),  IHDMIInputControllerListener.onAVIInfoFrame(), stop()|
|Verifies the Audio InfoFrame event change notification for HDMI input|Verifies received InfoFrame after the HDMI input port is in STARTED state or Info frame changes |start(),  IHDMIInputControllerListener.onAudioInfoFrame(), stop()|
|Verifies the Dynamic Range and Mastering (DRM) InfoFrame event change notification for HDMI input|Verifies received InfoFrame after the HDMI input port is in STARTED state or Info frame changes |start(),  IHDMIInputControllerListener.onDRMInfoFrame(), stop()|
|Verifies the Vendor Specific InfoFrame (VSIF) event change notification for HDMI input|Verifies received InfoFrame after the HDMI input port is in STARTED state or Info frame changes|start(), IHDMIInputControllerListener.onVendorSpecificInfoFrame(), stop()|
