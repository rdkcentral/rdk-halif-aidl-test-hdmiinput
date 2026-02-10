
# HDMI Input High-Level Test Specification Document


## Table of Contents

- [Acronyms, Terms, and Abbreviations](#acronyms-terms-and-abbreviations)
- [Introduction](#introduction)
- [L2 Test Specification](#l2-test-specification)
- [L3 Test Specification](#l3-test-specification)


## Acronyms, Terms, and Abbreviations

- `HAL`    - Hardware Abstraction Layer
- `API`    - Application Programming Interface
- `AIDL`   - Android Interface Definition Language
- `L2`     - Level 2 Testing
- `L3`     - Level 3 Testing
- `HDMI`   - High Definition Multimedia Interface
- `HFP`    - Hardware feature profile
- `EDID`   - Extended Display Identification Data
- `HDCP`   - High-bandwidth Digital Content Protection
- `DUT`    - Device Under Test
- `NA`     - Not Applicable


## Introduction
This document provides a comprehensive high-level test specification for the `HDMI` Input `HAL`. It details the `L2` and `L3` test cases designed to validate the functionality, reliability, and compliance of the `HDMI` Input `HAL` according to the defined interface and `HFP`.

- **HDMI Input Interface Definitions:** [HDMI Input HAL](https://github.com/rdkcentral/rdk-halif-aidl/tree/0.13.1/hdmiinput/current/com/rdk/hal/hdmiinput)
- **HDMI Input HFP:** [HFP](https://github.com/rdkcentral/rdk-halif-aidl/blob/0.13.1/hdmiinput/current/hfp-hdmiinput.yaml)



## L2 Test Specification

| Test | Description | HAL APIs |
|------|-------------|----------|
|Verify the state of `HDMI` Input|<ul><li>Loop through supported input ports.</li><li>Ensure state transitions: open (OPENING, READY) → start (STARTING, STARTED) → stop (STOPPING, READY) → close (CLOSING, CLOSED) work as expected using `getState()` and `onStateChanged()` callbacks from `IHDMIInputEventListener` and `IHDMIInputControllerListener`.</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `getState()`, `registerEventListener()`, `unregisterEventListener()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputControllerListener`: `onStateChanged()`</li><li>`IHDMIInputEventListener`: `onStateChanged()`</li></ul>|
|Verify the default callbacks during state transitions|<ul><li>Loop through supported input ports.</li><li>Call `open()` on the port and ensure `onConnectionStateChanged()` is fired with `false`.</li><li>Call `start()` on the port and ensure `onSignalStateChanged()` is fired with `NO_SIGNAL`.</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputControllerListener`: `onConnectionStateChanged()`, `onSignalStateChanged()`</li></ul>|
|Verify Set and Get `EDID`|<ul><li>Loop through supported input ports.</li><li>Get `EDID` bytes using `getEDID()` and verify they match `getDefaultEDID()`.</li><li>Set the `EDID` bytes using `setEDID()` in the `READY` state and verify using `getEDID()` and `onEDIDChange()` callback.</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `registerEventListener()`, `unregisterEventListener()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputEventListener`: `onEDIDChange()`</li></ul>|


## L3 Test Specification

| Test | Description | HAL APIs |
|------|-------------|----------|
|Verify connection state|<ul><li>Loop through supported input ports.</li><li>Connect an external device to the input port.</li><li>Call `open()` on the input port and ensure `onConnectionStateChanged()` is fired with `true`.</li><li>Validate the connection state using `getConnectionState()`.</li><li>Disconnect and reconnect the cable, ensuring the callbacks and `getConnectionState()` provide the correct connection state information in both `READY` and `STARTED` states.</li><li>If the `DUT` supports multiple concurrent starts (`maximumConcurrentStartedPorts` > 1), repeat the test in concurrent mode</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`, `getConnectionState()`</li><li>`IHDMIInputControllerListener`: `onConnectionStateChanged()`</li></ul>|
|Verify signal state|<ul><li>Loop through supported input ports.</li><li>Connect an external device to the input port.</li><li>Call `open()` and `start()` on the input port and ensure `onSignalStateChanged()` is fired with `LOCKED`.</li><li>Disconnect and reconnect the cable, ensuring the callback notifies the correct signal state information in both `READY` and `STARTED` states.</li><li>Put the `HDMI` source in an inactive or standby state and ensure `onSignalStateChanged()` is fired with `NO_SIGNAL`.</li><li>If the `DUT` supports multiple concurrent starts (`maximumConcurrentStartedPorts` > 1), repeat the test in concurrent mode</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputControllerListener`: `onSignalStateChanged()`</li></ul>|
|Verify `EDID` at source device|<ul><li>Loop through supported input ports.</li><li>Connect an external device to the input port.</li><li>Set the `EDID` bytes using `setEDID()` in the `READY` state and verify using `getEDID()` and `onEDIDChange()` callback.</li><li>Call `start()` on the input port.</li><li>Ensure that the set `EDID` data is reflected at the source device.</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `registerEventListener()`, `unregisterEventListener()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputEventListener`: `onEDIDChange()`</li></ul>|
|Verify `HDCP` status|<ul><li>Loop through supported input ports.</li><li>Connect an external device to the input port.</li><li>Call `open()` and `start()` on the input port and ensure `onHDCPStatusChanged()` is fired with `HDCP` status `AUTHENTICATION_IN_PROGRESS` and `AUTHENTICATED`.</li><li>Verify the status using `getHDCPStatus()` and the negotiated `HDCP` version using `getHDCPCurrentVersion()`.</li><li>If the `DUT` supports multiple concurrent starts (`maximumConcurrentStartedPorts` > 1), repeat the test in concurrent mode</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `getHDCPCurrentVersion()`, `getHDCPStatus()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputControllerListener`: `onHDCPStatusChanged()`</li></ul>|
|Verify different `HDCP` protocol versions|<ul><li>Loop through supported input ports.</li><li>Loop through different external devices with various `HDCP` protocol versions (including NO `HDCP`).</li><li>Call `open()` and `start()` on the input port and ensure `onHDCPStatusChanged()` is fired with `HDCP` status `AUTHENTICATION_IN_PROGRESS` and `AUTHENTICATED`.</li><li>Verify the status using `getHDCPStatus()` and the negotiated `HDCP` version using `getHDCPCurrentVersion()`.</li><li>If the `DUT` supports multiple concurrent starts (`maximumConcurrentStartedPorts` > 1), repeat the test in concurrent mode</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `getHDCPCurrentVersion()`, `getHDCPStatus()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputControllerListener`: `onHDCPStatusChanged()`</li></ul>|
|Verify the infoFrame callbacks|<ul><li>Loop through supported input ports.</li><li>Connect an external device to the input port.</li><li>Call `open()` and `start()` on the input port.</li><li>Send various infoFrames from the external device and ensure the corresponding callbacks (`onVideoFormatChanged()`, `onVRRChanged()`, `onAVIInfoFrame()`, `onAudioInfoFrame()`, `onSPDInfoFrame()`, `onDRMInfoFrame()`, `onVendorSpecificInfoFrame()`) are fired with the correct information.</li><li>If the `DUT` supports multiple concurrent starts (`maximumConcurrentStartedPorts` > 1), repeat the test in concurrent mode</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `getHDCPCurrentVersion()`, `getHDCPStatus()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputControllerListener`: `onVideoFormatChanged()`, `onVRRChanged()`, `onAVIInfoFrame()`, `onAudioInfoFrame()`, `onSPDInfoFrame()`, `onDRMInfoFrame()`, `onVendorSpecificInfoFrame()`</li></ul>|
|Stress test|<ul><li>Loop through supported input ports.</li><li>Call `open()` and `start()` on the input port.</li><li>Connect and disconnect the HDMI port multiple times.</li><li>Validate that `onConnectionStateChanged()`, `onSignalStateChanged()`, and `onHDCPStatusChanged()` are fired with the correct status.</li><li>If the `DUT` supports multiple concurrent starts (`maximumConcurrentStartedPorts` > 1), repeat the test in concurrent mode</li></ul>|<ul><li>`IHDMIInputManager`: `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput`: `open()`, `getHDCPCurrentVersion()`, `getHDCPStatus()`, `close()`</li><li>`IHDMIInputController`: `start()`, `stop()`</li><li>`IHDMIInputControllerListener`: `onConnectionStateChanged()`, `onSignalStateChanged()`, `onHDCPStatusChanged()`</li></ul>|

**Note:** `setProperty` and `setPropertyMulti` are not covered, as the `HAL` specification defines no read-write properties
