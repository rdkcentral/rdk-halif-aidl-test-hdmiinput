/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**
 * @addtogroup HPK Hardware Porting Kit
 * @{
 *
 */

/**
 * @addtogroup HDMI_INPUT HDMI INPUT Module
 * @{
 *
 */

/**
 * @defgroup HDMI_INPUT_HALTEST HDMI INPUT HAL Tests
 * @{
 *
 */

/**
 * @defgroup HDMI_INPUT_HALTEST_L2 HDMI INPUT HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Test Cases for HDMI INPUT HAL :
 *
 * This module includes Level 2 functional tests (success and failure scenarios).
 * This is to ensure that the API meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:** @n
 * **Dependencies:** None@n
 *
 * @endparblock
 *
 */

/**
 * @file vts_hdmiinput_l2_tests.cpp
 *
 */
#include "vts_hdmiinput.h"
#include "vts_hdmiinput_parse_hfp.h"

/* Test class registration with UT_Core */
UT_ADD_TEST_TO_GROUP(VTS_L2_HDMIINPUT, UT_TESTS_L2)

class VTS_L2_HDMIINPUT : public UTCore
{
public:
    VTS_L2_HDMIINPUT() : UTCore() {}

    ~VTS_L2_HDMIINPUT() override = default;

    void SetUp() override
    {
        android::ProcessState::self()->startThreadPool();
    }

    void TearDown() override
    {
    }
};

/**
 * @brief Validate HDMI INPUT interface state transitions and event notifications.
 *
 * This test verifies the state machine behavior of the HDMI INPUT interface by exercising all major state transitions:
 * CLOSED → OPENING → READY → STARTING → STARTED → STOPPING → READY → CLOSING → CLOSED.
 * It ensures that each transition occurs as expected and that the event listener receives
 * the correct onStateChanged notifications for each step.
 *
 * **Test Group ID:** Basic: 02@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:**@n
 * - No external HDMI devices connected to the DUT@n
 * - HDMI INPUT interface must be available in State::CLOSED initially@n
 *
 * **Dependencies:**
 * - A valid gHDMIInputHandle must be available
 *
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * Refer to UT specification documentation [hdmiinput_High-Level_TestSpecification.md](../docs/pages/hdmiinput_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L2_HDMIINPUT, StateTransitions)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    pair<hdmiinput::State, hdmiinput::State> stateChanges;

    // Retrieve all HDMI input IDs
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        // Retrieve the HDMI Input object
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<hdmiinput::IHDMIInputControllerListener> controlListener = new HDMIInputControllerListener();
        android::sp<HDMIInputEventListener> eventListener = new HDMIInputEventListener();
        android::sp<hdmiinput::IHDMIInputController> controller;
        hdmiinput::State state;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESSS(id, hdmiInput);

        // Check initial state is CLOSED
        HDMIINPUT_GET_STATE(hdmiInput, state);
        UT_ASSERT_EQUAL(state, hdmiinput::State::CLOSED);

        // Register an event listener
        HDMIINPUT_REGISTEREVENT_SUCCESS(hdmiInput, eventListener);

        // Open the HDMI input port
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, controlListener, controller, true);

        // Verify onStateChanged for CLOSED→OPENING
        UT_ASSERT_TRUE(eventListener->getNextStateChanges(stateChanges));
        UT_ASSERT_EQUAL(stateChanges.first, hdmiinput::State::CLOSED);
        UT_ASSERT_EQUAL(stateChanges.second, hdmiinput::State::OPENING);

        // Verify onStateChanged for OPENING→READY
        UT_ASSERT_TRUE(eventListener->getNextStateChanges(stateChanges));
        UT_ASSERT_EQUAL(stateChanges.first, hdmiinput::State::OPENING);
        UT_ASSERT_EQUAL(stateChanges.second, hdmiinput::State::READY);

        // Start the HDMI input port
        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);

        // Verify onStateChanged for READY→STARTING
        UT_ASSERT_TRUE(eventListener->getNextStateChanges(stateChanges));
        UT_ASSERT_EQUAL(stateChanges.first, hdmiinput::State::READY);
        UT_ASSERT_EQUAL(stateChanges.second, hdmiinput::State::STARTING);

        // Verify onStateChanged for STARTING→STARTED
        UT_ASSERT_TRUE(eventListener->getNextStateChanges(stateChanges));
        UT_ASSERT_EQUAL(stateChanges.first, hdmiinput::State::STARTING);
        UT_ASSERT_EQUAL(stateChanges.second, hdmiinput::State::STARTED);

        // Stop the HDMI input port
        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);

        // Verify onStateChanged for STARTED→STOPPING
        UT_ASSERT_TRUE(eventListener->getNextStateChanges(stateChanges));
        UT_ASSERT_EQUAL(stateChanges.first, hdmiinput::State::STARTED);
        UT_ASSERT_EQUAL(stateChanges.second, hdmiinput::State::STOPPING);

        // Verify onStateChanged for STOPPING→READY
        UT_ASSERT_TRUE(eventListener->getNextStateChanges(stateChanges));
        UT_ASSERT_EQUAL(stateChanges.first, hdmiinput::State::STOPPING);
        UT_ASSERT_EQUAL(stateChanges.second, hdmiinput::State::READY);

        // Close the HDMI input port
        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);

        // Verify onStateChanged for READY→CLOSING
        UT_ASSERT_TRUE(eventListener->getNextStateChanges(stateChanges));
        UT_ASSERT_EQUAL(stateChanges.first, hdmiinput::State::READY);
        UT_ASSERT_EQUAL(stateChanges.second, hdmiinput::State::CLOSING);

        // Verify onStateChanged for CLOSING→CLOSED
        UT_ASSERT_TRUE(eventListener->getNextStateChanges(stateChanges));
        UT_ASSERT_EQUAL(stateChanges.first, hdmiinput::State::CLOSING);
        UT_ASSERT_EQUAL(stateChanges.second, hdmiinput::State::CLOSED);

        // Unregister the event listener
        HDMIINPUT_UNREGISTEREVENT_SUCCESS(hdmiInput, eventListener);
    }
}

/**
 * @brief Verify default callbacks during HDMI INPUT state transitions.
 *
 * This test verifies that the default callbacks onConnectionStateChanged() and onSignalStateChanged()
 * are fired with the correct values during state transitions. For each supported HDMI input port,
 * the test opens the port and ensures onConnectionStateChanged() is fired with false, then starts the port
 * and ensures onSignalStateChanged() is fired with NO_SIGNAL.
 *
 * **Test Group ID:** Basic: 02@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:**@n
 * - No external HDMI devices connected to the DUT@n
 * - HDMI INPUT interface must be available in State::CLOSED initially@n
 *
 * **Dependencies:**@n
 * - A valid gHDMIInputHandle must be available@n
 *
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * Refer to UT specification documentation [hdmiinput_High-Level_TestSpecification.md](../docs/pages/hdmiinput_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L2_HDMIINPUT, DefaultCallbacks)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    // Retrieve all HDMI input IDs
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        // Retrieve the HDMI Input object
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<HDMIInputControllerListener> controlListener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;
        bool connectionState = true;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESSS(id, hdmiInput);

        // Open the HDMI input port and check onConnectionStateChanged(false)
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, controlListener, controller, true);

        // verify onConnectionStateChanged(false)
        UT_ASSERT_TRUE(controlListener->getNextConnectionState(connectionState));
        UT_ASSERT_FALSE(connectionState);

        // verify getConnectionState also returns false
        HDMIINPUT_GETCONNECTIONSTATE(controller, connectionState);
        UT_ASSERT_FALSE(connectionState);

        // Start the HDMI input port and check onSignalStateChanged(NO_SIGNAL)
        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);
        hdmiinput::SignalState signalState = hdmiinput::SignalState::NO_SIGNAL;
        UT_ASSERT_TRUE(controlListener->getNextSignalState(signalState));
        UT_ASSERT_EQUAL(signalState, hdmiinput::SignalState::NO_SIGNAL);

        // Stop and close the HDMI input port for cleanup
        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Verify Set and Get EDID functionality for HDMI INPUT.
 *
 * This test verifies that getEDID() returns the same bytes as getDefaultEDID() by default, and that setEDID()
 * updates the EDID correctly in the READY state. It also verifies that the onEDIDChange() callback is fired
 * and that getEDID() returns the updated value after setEDID().
 *
 * **Test Group ID:** Basic: 02@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:**@n
 * - No external HDMI devices connected to the DUT@n
 * - HDMI INPUT interface must be available in State::CLOSED initially@n
 *
 * **Dependencies:**@n
 * - A valid gHDMIInputHandle must be available@n
 *
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * Refer to UT specification documentation [hdmiinput_High-Level_TestSpecification.md](../docs/pages/hdmiinput_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L2_HDMIINPUT, SetandGetEDID)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    std::vector<uint8_t> testEdidData_1 =
    {
        0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x04,0x69,0x88,0x88,0x01,0x00,0x00,0x00,
        0x01,0x1E,0x01,0x04,0xA5,0x34,0x20,0x78,0x2A,0xEE,0x95,0xA3,0x54,0x4C,0x99,0x26,
        0x0F,0x50,0x54,0xA5,0x4B,0x00,0x81,0x80,0x81,0x40,0x81,0x99,0xA9,0xC0,0xB3,0x00,
        0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,
        0x45,0x00,0xFD,0x1E,0x11,0x00,0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x38,0x4C,0x1E,
        0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x46,
        0x61,0x6C,0x63,0x6F,0x6E,0x20,0x44,0x31,0x20,0x4D,0x6F,0x6E,0x0A,0x20,0x00,0x00,
        0x00,0xFF,0x00,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x0A,0x01,0x7A,
        0x02,0x03,0x1F,0xF1,0x47,0x90,0x05,0x04,0x03,0x02,0x07,0x16,0x01,0x23,0x09,0x07,
        0x07,0x83,0x01,0x00,0x00,0x65,0x03,0x0C,0x00,0x10,0x00,0x38,0x3C,0x20,0x00,0x60,
        0x01,0x02,0x03,0x67,0xD8,0x5D,0xC4,0x01,0x78,0x80,0x03,0xE3,0x05,0xC0,0x00,0xE3,
        0x06,0x05,0x01,0x5A,0x80,0xB0,0x58,0x2A,0x40,0x30,0x20,0x35,0x00,0xFD,0x1E,0x11,
        0x00,0x00,0x1E,0x8C,0x0A,0xD0,0x8A,0x20,0xE0,0x2D,0x10,0x10,0x3E,0x96,0x00,0xFD,
        0x1E,0x11,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC8
    };

    std::vector<uint8_t> testEdidData_2 =
    {
        0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x10,0xAC,0xD1,0xA0,0x4C,0x30,0x30,0x30,
        0x1C,0x20,0x01,0x04,0xB5,0x3C,0x22,0x78,0x3A,0xEE,0x95,0xA3,0x54,0x4C,0x99,0x26,
        0x0F,0x50,0x54,0xA5,0x4B,0x00,0x81,0x80,0x81,0x40,0x81,0x99,0xA9,0xC0,0xB3,0x00,
        0xD1,0xC0,0x01,0x01,0x01,0x01,0x56,0x5E,0x00,0xA0,0xA0,0xA0,0x29,0x50,0x30,0x20,
        0x35,0x00,0x55,0x50,0x21,0x00,0x00,0x1A,0x00,0x00,0x00,0xFD,0x00,0x30,0x90,0x1E,
        0x78,0x3C,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x46,
        0x61,0x6C,0x63,0x6F,0x6E,0x20,0x44,0x31,0x20,0x34,0x4B,0x0A,0x20,0x20,0x00,0x00,
        0x00,0xFF,0x00,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x31,0x32,0x33,0x01,0x6F,
        0x02,0x03,0x26,0xF1,0x4F,0x90,0x1F,0x04,0x13,0x05,0x14,0x03,0x12,0x20,0x21,0x22,
        0x23,0x09,0x07,0x07,0x83,0x01,0x00,0x00,0x6D,0x03,0x0C,0x00,0x10,0x00,0x38,0x3C,
        0x20,0x00,0x60,0x01,0x02,0x03,0x67,0xD8,0x5D,0xC4,0x01,0x78,0x80,0x03,0xE2,0x00,
        0xC0,0x00,0xE3,0x06,0x05,0x01,0x00,0x00,0xE2,0x00,0x4F,0xE3,0x05,0x03,0x01,0x00,
        0x00,0xE2,0x00,0x0F,0x67,0xD8,0x5D,0xC4,0x01,0x78,0x80,0x03,0xE2,0x00,0xC0,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA4
    };

    // Retrieve all HDMI input IDs
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        // Retrieve the HDMI Input object
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<HDMIInputEventListener> eventListener = new HDMIInputEventListener();
        android::sp<hdmiinput::IHDMIInputControllerListener> controlListener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;
        hdmiinput::Capabilities caps;
        std::vector<uint8_t> changedEDID;
        std::vector<uint8_t> verifyEDID;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESSS(id, hdmiInput);

        HDMIINPUT_GET_CAPABILITIES(hdmiInput, caps);

        UT_ASSERT_FALSE(caps.supportedVersions.empty());

        auto maxHDCP = caps.supportedVersions[0];
        for (const auto& hdcpVersion : caps.supportedVersions)
        {
            if (static_cast<int>(hdcpVersion) > static_cast<int>(maxHDCP)) {
                maxHDCP = hdcpVersion;
            }
        }
        // Register an event listener
        HDMIINPUT_REGISTEREVENT_SUCCESS(hdmiInput, eventListener);

        // Open the port and transition to READY state
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, controlListener, controller, true);

        // Call getEDID() and getDefaultEDID() and verify they match
        std::vector<uint8_t> edidData, defaultEDID;
        HDMIINPUT_GETEDID_SUCCESS(hdmiInput, edidData);
        HDMIINPUT_GETDEFAULTEDID_SUCCESS(hdmiInput, maxHDCP, defaultEDID);
        UT_ASSERT_EQUAL(edidData.size(), defaultEDID.size());
        for (size_t i = 0; i < edidData.size(); ++i)
        {
            UT_ASSERT_EQUAL(edidData[i], defaultEDID[i]);
        }

        // Call setEDID() with new EDID bytes and verify onEDIDChange() is fired with the new bytes
        HDMIINPUT_SETEDID_SUCCESS(controller, testEdidData_1);

        // Wait for and verify onEDIDChange() callback
        UT_ASSERT_TRUE(eventListener->getNextEDIDChange(changedEDID));
        UT_ASSERT_EQUAL(changedEDID.size(), testEdidData_1.size());
        for (size_t i = 0; i < changedEDID.size(); ++i)
        {
            UT_ASSERT_EQUAL(changedEDID[i], testEdidData_1[i]);
        }

        // Verify getEDID() returns the new value
        HDMIINPUT_GETEDID_SUCCESS(hdmiInput, verifyEDID);
        UT_ASSERT_EQUAL(verifyEDID.size(), testEdidData_1.size());
        for (size_t i = 0; i < verifyEDID.size(); ++i)
        {
            UT_ASSERT_EQUAL(verifyEDID[i], testEdidData_1[i]);
        }

        // Start and Stop the port to ensure EDID remains consistent across state transitions
        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);
        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);

        // Call setEDID() with new EDID bytes and verify onEDIDChange() is fired with the new bytes
        HDMIINPUT_SETEDID_SUCCESS(controller, testEdidData_2);

        // Wait for and verify onEDIDChange() callback
        UT_ASSERT_TRUE(eventListener->getNextEDIDChange(changedEDID));
        UT_ASSERT_EQUAL(changedEDID.size(), testEdidData_2.size());
        for (size_t i = 0; i < changedEDID.size(); ++i)
        {
            UT_ASSERT_EQUAL(changedEDID[i], testEdidData_2[i]);
        }

        // Verify getEDID() returns the new value
        HDMIINPUT_GETEDID_SUCCESS(hdmiInput, verifyEDID);
        UT_ASSERT_EQUAL(verifyEDID.size(), testEdidData_2.size());
        for (size_t i = 0; i < verifyEDID.size(); ++i)
        {
            UT_ASSERT_EQUAL(verifyEDID[i], testEdidData_2[i]);
        }

        // Cleanup: Unregister event listener, stop and close port
        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
        HDMIINPUT_UNREGISTEREVENT_SUCCESS(hdmiInput, eventListener);
    }
}

/**
 * @brief Verify maximumConcurrentStartedPorts platform capability for HDMI INPUT.
 *
 * This test verifies that the platform enforces the maximum number of concurrently started HDMI input ports
 * as specified by the maximumConcurrentStartedPorts capability. It opens and starts one port, then attempts
 * to open and start a second port, expecting the second start() to fail if the maximum is 1.
 *
 * **Test Group ID:** Basic: 02@n
 * **Test Case ID:** 004@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:**@n
 * - No external HDMI devices connected to the DUT@n
 * - HDMI INPUT interface must be available in State::CLOSED initially@n
 *
 * **Dependencies:**@n
 * - A valid gHDMIInputHandle must be available@n
 *
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * Refer to UT specification documentation [hdmiinput_High-Level_TestSpecification.md](../docs/pages/hdmiinput_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L2_HDMIINPUT, MaxConcurrentPorts)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    hdmiinput::PlatformCapabilities caps;

    // Retrieve all HDMI input IDs
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    // Retrieve platform capabilities
    HDMIINPUT_MANAGER_GETCAPABILITIES(caps);
    UT_ASSERT_TRUE(caps.maximumConcurrentStartedPorts >= 1);
    UT_ASSERT_TRUE(caps.maximumConcurrentStartedPorts <= hdmiInputIds.size());

    // Open and start up to maximumConcurrentStartedPorts ports
    size_t maxPorts = static_cast<size_t>(caps.maximumConcurrentStartedPorts);
    std::vector<android::sp<hdmiinput::IHDMIInput>> hdmiInputs;
    std::vector<android::sp<hdmiinput::IHDMIInputControllerListener>> listeners;
    std::vector<android::sp<hdmiinput::IHDMIInputController>> controllers;

    for (size_t i = 0; i < maxPorts; ++i)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESSS(hdmiInputIds[i], hdmiInput);
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);
        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);
        hdmiInputs.push_back(hdmiInput);
        listeners.push_back(listener);
        controllers.push_back(controller);
    }

    if(maxPorts < hdmiInputIds.size())
    {
        // Try to open and start one more port, which should fail
        size_t failIndex = maxPorts;
        android::sp<hdmiinput::IHDMIInput> hdmiInputFail;
        android::sp<hdmiinput::IHDMIInputControllerListener> listenerFail = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controllerFail;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESSS(hdmiInputIds[failIndex], hdmiInputFail);
        HDMIINPUT_OPEN_SUCCESS(hdmiInputFail, listenerFail, controllerFail, true);
        HDMIINPUT_START_FAILURE(hdmiInputFail, controllerFail, hdmiinput::State::READY, Status::EX_ILLEGAL_STATE);
        HDMIINPUT_CLOSE_SUCCESS(hdmiInputFail, controllerFail, true);
    }

    // Cleanup: stop and close all opened ports
    for (size_t i = 0; i < controllers.size(); ++i)
    {
        HDMIINPUT_STOP_SUCCESS(hdmiInputs[i], controllers[i], true);
        HDMIINPUT_CLOSE_SUCCESS(hdmiInputs[i], controllers[i], true);
    }
}

/** @} */ // End of HDMI_INPUT_HALTEST L2 File
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT Module
/** @} */ // End of HPK