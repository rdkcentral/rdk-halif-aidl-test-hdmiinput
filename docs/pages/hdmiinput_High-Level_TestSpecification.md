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

- Hdmi Input Interface Definitions - [Hdmi input HAL](https://github.com/rdkcentral/rdk-halif-aidl/tree/0.12.0/hdmiinput/current/com/rdk/hal/hdmiinput)

- Hdmi Input HFP - [Hdmi input Overview]()


## L2 Test Specification

| Test | Description | HAL APIs |
|------|-------------|----------|
|Verifies the state of HDMI input state|<ul><li>Verifies the current HDMI input state when IHDMIInputEventListener.onStateChanged() changes</li><li> It Read supported input ports using IHDMIInputManager class</li><li> Loop through supported input port </li><li> Register event listeners using registerEventListener </li><li>Ensure state transitions (open -> start -> stop -> close) work as expected using getState</li> <li> Validate event listener notifications using onStateChanged</ul>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li>IHDMIInput - open(), getState(), registerEventListener(),onStateChanged(), unregisterEventListener(), close()</li><li>IHDMIInputController - start(), stop()</li></ul>|
|Verifies Set and Get EDID before start |<ul><li> It Read supported input ports using IHDMIInputManager class</li><li> Loop through supported input ports</li><li> Set the EDID byte for each port and read the EDID byte before start no listener callbacks will be received </li></ul>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li>IHDMIInput - open(),setEDID(), getEDID() close</li></ul>|
|Verifies Set and Get EDID after start |<ul><li> It Read supported input ports using IHDMIInputManager class</li><li> Loop through supported input ports</li><li> Set the EDID byte for each port and read the EDID byte after starting the input port and receives the IHDMIInputControllerListener.onConnectionStateChanged() for EDID set </li></ul>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li>IHDMIInput - open(),setEDID(), getEDID(), close()</li><li>IHDMIInputController - start(),getConnectionState(), stop()</li></ul><li>IHDMIInputConIHDMIInputControllerListenertroller - onConnectionStateChanged()</li>|
|Verify Connection state of each HDMI input |<ul><li>Verifies current connection state for a device attached to the HDMI input port.</li><li> connectionState  will FALSE is port is not opened </li></ul> |<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li>IHDMIInput - open(),close()</li><li>IHDMIInputController - start(),getConnectionState() stop()</li></ul>|
|Verify HDCP version |<ul><li>Reads the HDCP version for each port </li><li>If HDCP has not yet been authenticated then `HDCPProtocolVersion.UNDEFINED` is returned</li></ul> |<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li>IHDMIInput - open(), close()<li>IHDMIInputController - getHDCPCurrentVersion()</li></ul>|


## L3 Test Specification

| Test | Description | HAL APIs used |
|------|-------------|----------|
|Verify Connection state of each HDMI input |<ul><li>Verifies current connection state for a device attached to the HDMI input port.</li><li> connectionState  will TRUE is port is  opened </li><li>Connect the input- Validate the state by getConnectionState()  and listener callback for connection change by onConnectionStateChanged()</li><li>Disconnect the input - Validate the state by getConnectionState() and listener callback for connection change by onConnectionStateChanged()  </li></ul> |<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li>IHDMIInput - open(),close()</li><li>IHDMIInputController - start(),getConnectionState() stop()</li><li> IHDMIInputControllerListener - onConnectionStateChanged()</li></ul>|
|Verify Signal state |<ul><li>checks the Signal state of each input port when video from the HDMI input port can be mapped to a video plane and audio is mixed. </li><li>on successful HDMI transitions signal state will be notified as `LOCKED` by onSignalStateChanged() </li> <li> when HDMI source is inactive or in standby state will be notified as `NO_SIGNAL` by onSignalStateChanged() </li><li> if the HDMI incoming signal is out of HDMI IN  capaport capabilities then signal state will be notified as `NOT_SUPPORTED` by onSignalStateChanged()  </li></ul> |<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li> <li> IHDMIInputController - start() , stop()</li><li> IHDMIInputControllerListener - onSignalStateChanged()</li></ul>|
|Verifies the VIC(Video ID code) change event|<ul><li> Reads the video format change event from the source to HDMI input </li><li>The VIC (Video ID Code) change event is notified when the HDMI source changes the video format being transmitted, which results in a different VIC value in the AVI InfoFrame.</li> onAVIInfoFrame() change event also will be notified if any change in VIC <li></li>  </ul>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li> <li> IHDMIInputController - start(), stop()</li><li> IHDMIInputControllerListener - onVideoFormatChanged(),onAVIInfoFrame()<</li></ul>|
|Verifies the VRR change event |<ul><li>Reads the VRR event change when VRR enabled/disabled </li><li>if HDMI input port supports 2.1 and above then VRR change event callback will be received, otherwise no callback will be received for VRR change</li><li>VRR change event occurs when VRR Range Update or source adjusts the refresh rate dynamically within the VRR range or After hot-plug or resolution change</li></ul>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li><li> IHDMIInputController - start(), stop()</li><li> IHDMIInputControllerListener - onVRRChanged()</li></ul>|
|Verifies the AVI Info frame change event |<ul><li>Reads the event change when there is a change in the video-related parameters transmitted by the HDMI source </li><li>this change event occurs Any changes in <ul>Resolution</ul> <ul>Refresh Rate</ul><ul> Color Space </ul><ul> Aspect Ratio</ul> <ul>Scan Information</ul> Active Format Information</ul><ul> Active Format Aspect Ratio</ul><ul>Active video region aspect ratio</ul><ul> Video Id code(VIC)</ul><ul>Pixel Repetition</ul><ul>Content Type</ul><>ul>extended colorimetry</ul><ul>Quantization Range</ul></li></ul>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li><li> IHDMIInputController - start(), stop()</li><li> IHDMIInputControllerListener - onAVIInfoFrame()</li></ul>|
|Verifies the Audio InfoFrame event change|<ul><li> Reads event change received for Audio InfoFrame in HDMI is a packet that carries audio-related metadata from the source device</li><li>first frame receives when HDMI start, then then Audio format change or Sample rate or bit depth change or Source switches between compressed and uncompressed audio </ul></li>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li><li> IHDMIInputController - start(), stop()</li><li> IHDMIInputControllerListener - onAudioInfoFrame()</li></ul>|
|Verify SPD InfoFrame |<ul><li>Reads the SPD info change event for each port </li><li>If the source device changes its payload info,it updates and sends SPD infoframe</li></ul>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li><li> IHDMIInputController - start() getSPDInfoFrame(), stop()</li><li> IHDMIInputControllerListener - onSPDInfoFrame()</li></ul>|
|Verifies the Dynamic Range and Mastering (DRM) InfoFrame event change |<ul><li> Reads event change for DRM info frame which carries HDR-related metadata(HDR10, HLG or PQ) to the sink</li><li> this the event occurs when swich from SDR to HDR or the metadata change</li></ul>|<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li> <li> IHDMIInput - open(), close()</li><li> IHDMIInputController - start(), stop()</li><li> IHDMIInputControllerListener - onDRMInfoFrame()</li></ul>|
|Verifies the Vendor Specific InfoFrame (VSIF) event change|<ul><li>Reads the event change received VSIF InfoFrame HDMI-defined packet used to transmit vendor-specific or HDMI Forum-specific features</li><li>this event change occurs when HDMI input receives VSIF has been added, removed, or its payload has changed in the HDMI stream.</li></ul> |<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li> <li> IHDMIInputController - start(), stop()</li><li> IHDMIInputControllerListener - onVendorSpecificInfoFrame()</li></ul>|
|Verify HDCP Status |<ul><li>Reads the HDCP status change event for each port </li><li>For connected input port returns the HDCP status</li> <li>if HDCP state is changed can be notified through IHDMIInputControllerListener.onHDCPStatusChanged()</li></ul> |<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li><li> IHDMIInputController - start() getHDCPStatus(), stop()</li><li> IHDMIInputControllerListener - onHDCPStatusChanged()</li></ul>|
|Stress test  |<ul><li> Connect and Disconnect the HDMI port (5 times) to receive <ul> HDCP Aunthentication events should be notified through onHDCPStatusChanged </ul><ul>Signal states should be notified through onSignalStateChanged()</ul><ul>Video related paremeter change through onAVIInfoFrame()</ul><ul> audio paremeter related changes through onAudioInfoFrame() </ul> <ul> Source vendor related changes through onSPDInfoFrame()</ul><ul> HDR metadate changes through onDRMInfoFrame()</ul><ul> Vendor specific changes are notified through onVendorSpecificInfoFrame()</ul></li></ul> |<ul><li>IHDMIInputManager - getHDMIInputIds, getHDMIInput</li><li> IHDMIInput - open(), close()</li><li> IHDMIInputController - start() getHDCPStatus(),getSPDInfoFrame() stop()</li><li> IHDMIInputControllerListener - onHDCPStatusChanged(),onSignalStateChanged(),onAVIInfoFrame(),onAudioInfoFrame(),onSPDInfoFrame(),onDRMInfoFrame(), onVendorSpecificInfoFrame()</li></ul>|






