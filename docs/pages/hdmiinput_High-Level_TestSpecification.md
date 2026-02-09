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
- `HDMI`   - High Definition Multimedia Interface
- `HFP`    - Hardware feature profile
- `EDID`   - Extended Display Identification Data
- `HDCP`   - High-bandwidth Digital Content Protection
- `NA`     - Not Applicable

## Introduction
This document provides a comprehensive high-level test specification for the `HDMI` Input `HAL`. It details the `L2` and `L3` test cases designed to validate the functionality, reliability, and compliance of the `HDMI` Input `HAL` as per the defined interface and `HFP`.

- `HDMI` Input Interface Definitions - [HDMI Input HAL](https://github.com/rdkcentral/rdk-halif-aidl/tree/0.13.1/hdmiinput/current/com/rdk/hal/hdmiinput)

- `HDMI` Input `HFP` - [HFP](https://github.com/rdkcentral/rdk-halif-aidl/blob/0.13.1/hdmiinput/current/hfp-hdmiinput.yaml)


## L2 Test Specification

| Test | Description | HAL APIs |
|------|-------------|----------|
|Verifies the state of `HDMI` input state|<ul><li> Loop through supported input ports </li><li>Ensure state transitions open(OPENING, READY) -> start(STARTING, STARTED) -> stop(STOPPING, READY) -> close(CLOSING, CLOSED) work as expected using `getState()` and `onStateChanged()` callback from `IHDMIInputEventListener` and `IHDMIInputControllerListener`</li></ul>|<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `getState()`, `registerEventListener()`, `unregisterEventListener()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`</li><li>`IHDMIInputControllerListener` - `onStateChanged`()</li> <li>`IHDMIInputEventListener` - `onStateChanged()`</li></ul>|
|Verifies the default callbacks during the state transitions|<ul><li> Loop through supported input ports </li><li>`open()` the port and ensure `onConnectionStateChanged()` is fired with `false`</li><li>`start()` the port and ensure `onSignalStateChanged()` is fired with `NO_SIGNAL`</li></ul>|<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`</li><li>`IHDMIInputControllerListener` - `onConnectionStateChanged`()</li> <li>`IHDMIInputControllerListener` - `onSignalStateChanged()`</li></ul>|
|Verifies Set and Get `EDID`|<ul><li>Loop through supported input ports</li><li>Get `EDID` bytes using `getEDID()` and should match with `getDefaultEDID()` </li><li> Set the `EDID` bytes using `setEDID()` in `READY` state and verify using `getEDID()` and `onEDIDChange()` callback</li><li> Set the `EDID` bytes using `setEDID()` in `STARTED` state and verify using `getEDID()` and `onEDIDChange()` callback</li></ul>|<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `registerEventListener()`, `unregisterEventListener()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`</li><li>`IHDMIInputEventListener` - `onEDIDChange()`</li></ul>|

## L3 Test Specification

| Test | Description | HAL APIs |
|------|-------------|----------|
|Verify Connection state|<ul><li>Loop through supported input ports</li><li>Connect the external device to input port</li><li>`open()` the input port and ensure `onConnectionStateChanged()` is fired with `true`</li><li>Validate the connection state using `getConnectionState()`</li><li>Disconnect and Connect the cable and ensure the callbacks and `getConnectionState()` gives the correct connection state information in `READY` and `STARTED` states</li></ul> |<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`, `getConnectionState()`</li><li>`IHDMIInputControllerListener` - `onConnectionStateChanged()`</li></ul>|
|Verify Signal state |<ul><li>Loop through supported input ports</li><li>Connect the external device to input port</li><li>`open()` and `start()` the input port and ensure `onSignalStateChanged()` is fired with `LOCKED`</li><li>Disconnect and Connect the cable and ensure the callback notifies the correct signal state information in `READY` and `STARTED` states</li><li>Put the `HDMI` source in inactive or in standby state and ensure `onSignalStateChanged()` is fired with `NO_SIGNAL` </li></ul>|<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li> `IHDMIInput` - `open()`, `close()`</li><li> `IHDMIInputController` - `start()`, `stop()`</li><li> `IHDMIInputControllerListener` - `onSignalStateChanged()`</li></ul>|
|Verify `EDID` at source device|<ul><li>Loop through supported input ports</li><li>Connect the external device to input port</li><li> Set the `EDID` bytes using `setEDID()` in `READY` state and verify using `getEDID()` and `onEDIDChange()` callback</li><li>`start()` the input port</li><li>Ensure that the set `EDID` data is reflecting at source device</li></ul>|<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `registerEventListener()`, `unregisterEventListener()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`</li><li>`IHDMIInputEventListener` - `onEDIDChange()`</li></ul>|
|Verify `HDCP` Status |<ul><li>Loop through supported input ports</li><li>Connect the external device to input port</li><li>`open()` and `start()` the input port and ensure `onHDCPStatusChanged()` is fired with `HDCP` status `AUTHENTICATION_IN_PROGRESS` and `AUTHENTICATED`</li><li>Verify status using `getHDCPStatus()` and negotiated `HDCP` version using `getHDCPCurrentVersion()`</li></ul> |<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `getHDCPCurrentVersion()`, `getHDCPStatus()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`</li><li>`IHDMIInputControllerListener` - `onHDCPStatusChanged()`</li></ul>|
|Verify different `HDCP` Protocol versions|<ul><li>Loop through supported input ports</li><li>Loop through different external device with different `HDCP` protocol versions</li><li>`open()` and `start()` the input port and ensure `onHDCPStatusChanged()` is fired with `HDCP` status `AUTHENTICATION_IN_PROGRESS` and `AUTHENTICATED`</li><li>Verify status using `getHDCPStatus()` and negotiated `HDCP` version using `getHDCPCurrentVersion()`</li></ul> |<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `getHDCPCurrentVersion()`, `getHDCPStatus()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`</li><li>`IHDMIInputControllerListener` - `onHDCPStatusChanged()`</li></ul>|
|Verify the infoFrame callbacks|<ul><li>Loop through supported input ports</li><li>Connect the external device to input port</li><li>`open()` and `start()` the input port</li><li>Send various infoFrames from the external device and ensure corresponding callbacks (`onVideoFormatChanged()`, `onVRRChanged()`, `onAVIInfoFrame()`, `onAudioInfoFrame()`, `onSPDInfoFrame()`, `onDRMInfoFrame()`, `onVendorSpecificInfoFrame()`) fires with correct information</li></ul>|<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `getHDCPCurrentVersion()`, `getHDCPStatus()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`</li><li>`IHDMIInputControllerListener` - `onVideoFormatChanged()`, `onVRRChanged()`, `onAVIInfoFrame()`, `onAudioInfoFrame()`, `onSPDInfoFrame()`, `onDRMInfoFrame()`, `onVendorSpecificInfoFrame()`</li></ul>|
|Stress test|<ul><li>Loop through supported input ports</li><li>`open()` and `start()` the input port</li><li>Connect and Disconnect the HDMI port multiple times</li><li>Validate `onConnectionStateChanged()`, `onSignalStateChanged()`, `onHDCPStatusChanged()` fires with correct status</li></ul> |<ul><li>`IHDMIInputManager` - `getHDMIInputIds()`, `getHDMIInput()`</li><li>`IHDMIInput` - `open()`, `getHDCPCurrentVersion()`, `getHDCPStatus()`, `close()`</li><li>`IHDMIInputController` - `start()`, `stop()`</li><li>`IHDMIInputControllerListener` - `onConnectionStateChanged()`, `onSignalStateChanged()`, `onHDCPStatusChanged()`</li></ul>|






