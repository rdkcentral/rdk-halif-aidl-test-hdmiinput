# HDMI INPUT L2 Low Level Test Specification

## Table of Contents

- [Overview](#overview)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [References](#references)
- [Level 2 Test Specification](#level-2-test-specification)

## Overview

This document describes the level 2 low level test specification for the `HDMI INPUT` module.

### Acronyms, Terms and Abbreviations

- `HAL`    - Hardware Abstraction Layer
- `VTS`    - Vendor Test Suite
- `HDMI`   - High Definition Multimedia Interface
- `L1`     - Level 1 Testing
- `L2`     - Level 2 Testing
- `L3`     - Level 3 Testing
- `DUT`    - Device Under Test
- `RAFT`   - Rapid Automation Framework for Testing
- `YAML`   - YAML Ain't Markup Language
- `AIDL`   - Android Interface Definition Language

### References

- `High Level Test Specification` - [hdmiinput_High-Level_TestSpecification.md](hdmiinput_High-Level_TestSpecification.md)

## Level 2 Test Specification

The following functions are intended to test the `HDMI INPUT` `HAL` module according to the `L2` Test specification.

### Test 1

| Title | Details |
| -- | -- |
| Function Name | `StateTransitions` |
| Description | This test validates the state machine behaviour of the HDMI INPUT interface. It ensures that the interface transitions correctly between CLOSED, READY, STARTED, and other states, and that state change notifications are received by the event listener |
| Test Group | 02 |
| Test Case ID | 001 |
| Priority | High |

**Pre-Conditions :**
- No external HDMI devices connected to the DUT
- HDMI INPUT interface must be available in State::CLOSED initially

**Dependencies :**
- None

**User Interaction :**
None

**Test Procedure :**

| Variation / Steps | Description | Test Data | Expected Result | Notes |
|-------------------|-------------|-----------|----------------|-------|
| 1 | Register Event Listeners | None | Register the event listener |
| 2 | Get the State before Open | None | State should be CLOSED |
| 3 | Open the HDMI INPUT with event listener | event listener handle | Function should return valid controller handle |
| 4 | Get the State | None | State should be OPENING |
| 5 | Check the event listener callback | | Previous State should be CLOSED and current state should be OPENING |
| 6 | Get the State | None | State should be READY |
| 7 | Check the event listener callback | | Previous State should be OPENING and current state should be READY |
| 8 | Start the HDMI INPUT | | Function should return `True` |
| 9 | Get the State | None | State should be STARTING |
| 10 | Check the event listener callback | | Previous State should be READY and current state should be STARTING |
| 11 | Get the State | None | State should be STARTED |
| 12 | Check the event listener callback | | Previous State should be STARTING and current state should be STARTED |
| 13 | Stop the HDMI INPUT | | Function should return `True` |
| 14 | Get the State | None | State should be STOPPING |
| 15 | Check the event listener callback | | Previous State should be STARTED and current state should be STOPPING |
| 16 | Get the State | None | State should be READY |
| 17 | Check the event listener callback | | Previous State should be STOPPING and current state should be READY |
| 18 | Close the HDMI INPUT | | Function should return `True` |
| 19 | Get the State | None | State should be CLOSING |
| 20 | Check the event listener callback | | Previous State should be READY and current state should be CLOSING |
| 21 | Get the State | None | State should be CLOSED |
| 22 | Check the event listener callback | | Previous State should be CLOSING and current state should be CLOSED |
| 23 | Unregister Event Listeners | | Function should return `True` |

### Test 2

| Title | Details |
| -- | -- |
| Function Name | `DefaultCallbacks` |
| Description | This test verifies that the default callbacks onConnectionStateChanged() and onSignalStateChanged() are fired with the correct values during state transitions. It ensures onConnectionStateChanged(false) is fired after opening, and onSignalStateChanged(NO_SIGNAL) is fired after starting |
| Test Group | 02 |
| Test Case ID | 002 |
| Priority | High |

**Pre-Conditions :**
- No external HDMI devices connected to the DUT
- HDMI INPUT interface must be available in State::CLOSED initially

**Dependencies :**
- None

**User Interaction :**
None

**Test Procedure :**

| Variation / Steps | Description | Test Data | Expected Result | Notes |
|-------------------|-------------|-----------|----------------|-------|
| 1 | Open the HDMI INPUT with event listener | event listener handle | Function should return valid controller handle |
| 2 | Check onConnectionStateChanged callback | | Callback should be fired with false |
| 3 | Get connection state | None | Should return false |
| 4 | Start the HDMI INPUT | | Function should return `True` |
| 5 | Check onSignalStateChanged callback | | Callback should be fired with NO_SIGNAL |
| 6 | Stop and close the HDMI INPUT | | Function should return `True` |

### Test 3

| Title | Details |
| -- | -- |
| Function Name | `SetandGetEDID` |
| Description | This test verifies that getEDID() returns the same bytes as getDefaultEDID() by default, and that setEDID() updates the EDID correctly in the READY state. It also verifies that the onEDIDChange() callback is fired and that getEDID() returns the updated value after setEDID() |
| Test Group | 02 |
| Test Case ID | 003 |
| Priority | High |

**Pre-Conditions :**
- No external HDMI devices connected to the DUT
- HDMI INPUT interface must be available in State::CLOSED initially

**Dependencies :**
- None

**User Interaction :**
None

**Test Procedure :**

| Variation / Steps | Description | Test Data | Expected Result | Notes |
|-------------------|-------------|-----------|----------------|-------|
| 1 | Open the HDMI INPUT and transition to READY | event listener handle | Function should return valid controller handle |
| 2 | Get EDID and DefaultEDID | None | Should match |
| 3 | Set EDID with new bytes | EDID bytes | onEDIDChange callback should be fired with new bytes |
| 4 | Get EDID | None | Should return new value |
| 5 | Start and Stop the HDMI INPUT | | EDID should remain consistent |
| 6 | Set EDID with another value | EDID bytes | onEDIDChange callback should be fired with new bytes |
| 7 | Get EDID | None | Should return new value |
| 8 | Cleanup: Unregister event listener, stop and close port | | Function should return `True` |

### Test 4

| Title | Details |
| -- | -- |
| Function Name | `MaxConcurrentPorts` |
| Description | This test verifies that the platform enforces the maximum number of concurrently started HDMI input ports as specified by the maximumConcurrentStartedPorts capability. It opens and starts up to the maximum, then attempts to start one more port, expecting failure |
| Test Group | 02 |
| Test Case ID | 004 |
| Priority | High |

**Pre-Conditions :**
- No external HDMI devices connected to the DUT
- HDMI INPUT interface must be available in State::CLOSED initially

**Dependencies :**
- None

**User Interaction :**
None

**Test Procedure :**

| Variation / Steps | Description | Test Data | Expected Result | Notes |
|-------------------|-------------|-----------|----------------|-------|
| 1 | Get platform capabilities | None | maximumConcurrentStartedPorts should be >= 1 |
| 2 | Open and start up to maximumConcurrentStartedPorts ports | event listener handle | Function should return valid controller handle |
| 3 | Try to open and start one more port | event listener handle | Start should fail if maximum reached |
| 4 | Cleanup: stop and close all opened ports | | Function should return `True` |

