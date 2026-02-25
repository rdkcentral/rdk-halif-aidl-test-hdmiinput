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
 * @defgroup HDMI_INPUT_HALTEST_L3 HDMI INPUT HAL Tests L3 File
 * @{
 * @parblock
 *
 * ### L3 Test Cases for HDMI INPUT HAL :
 *
 * This module includes Level 3 functional tests (success and failure scenarios).
 * This is to ensure that the API meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:** @n
 * **Dependencies:** None@n
 *
 * @endparblock
 *
 */

/**
 * @file vts_hdmiinput_l3_tests.cpp
 *
 */
#include "vts_hdmiinput.h"
#include "vts_hdmiinput_parse_hfp.h"
#include "vts_hdmiinput_parse_testconfig.h"

static const char *moduleName = "hdmiinput";
static const char *testGroupName = "L3";

/* Test class registration with UT_Core */
UT_ADD_TEST_TO_GROUP(VTS_L3_HDMIINPUT, UT_TESTS_L3)

class VTS_L3_HDMIINPUT : public UTCore
{
public:
    VTS_L3_HDMIINPUT() : UTCore() {}

    ~VTS_L3_HDMIINPUT() override = default;

    vector<tuple<IHDMIInput::Id, sp<IHDMIInput>, sp<HDMIInputControllerListener>, sp<HDMIInputEventListener>, sp<hdmiinput::IHDMIInputController>, hdmiinput::Capabilities *>> mHDMIInputList;

    void SetUp() override
    {
        std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

        android::ProcessState::self()->startThreadPool();

        /* Get the HDMI INPUT IDs */
        HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

        for (const auto& id : hdmiInputIds)
        {
            android::sp<hdmiinput::IHDMIInput> hdmiInput;
            HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESSS(id, hdmiInput);

            sp<HDMIInputControllerListener> controllerListener = new HDMIInputControllerListener();
            sp<HDMIInputEventListener> eventListener = new HDMIInputEventListener();
            hdmiinput::Capabilities *capabilities = new hdmiinput::Capabilities();
            android::sp<hdmiinput::IHDMIInputController> controller;

            HDMIINPUT_GET_CAPABILITIES(hdmiInput, capabilities);

            // Register an event listener
            HDMIINPUT_REGISTEREVENT_SUCCESS(hdmiInput, eventListener);

            // Open the HDMI input port
            HDMIINPUT_OPEN_SUCCESS(hdmiInput, controllerListener, controller, true);

            mHDMIInputList.push_back({id, hdmiInput, controllerListener, eventListener, controller, capabilities});
        }
    }

    void TearDown() override
    {
        for (auto& input : mHDMIInputList)
        {
            auto& hdmiInput = std::get<1>(input);
            auto& eventListener = std::get<3>(input);
            auto& controller = std::get<4>(input);

            // Close the HDMI input port
            HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);

            // Unregister the event listener
            HDMIINPUT_UNREGISTEREVENT_SUCCESS(hdmiInput, eventListener);
        }
    }
};

/**
 * @brief Validate HDMI INPUT connection state transitions using configuration-driven test.
 *
 * This test parses a configuration file to select the HDMI input port and expected connection states.
 * For the specified port, it verifies the sequence of connection state changes reported by the controller listener,
 * including the initial state before open and all subsequent expected states.
 *
 * **Test Group ID:** Basic: 03@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:**@n
 * none@n
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
UT_ADD_TEST(VTS_L3_HDMIINPUT, ConnectionSignalStateTestOpen)
{
    const char *testName = "ConnectionSignalStateTestOpen";
    UT_status_t status = UT_STATUS_OK;
    hdmiinputConnectionStateOpenConfig_t connectionStateConfig;

    /* Parse the test configuration */
    status = vts_hdmiinput_parse_test_config(VTS_HDMIINPUT_TEST_CONFIG_PATH, moduleName, testGroupName, testName, &connectionStateConfig);

    UT_ASSERT_EQUAL_FATAL(status, UT_STATUS_OK);

    for (const auto& [_portId, hdmiInput, controlListener, eventListener, controller, capabilities] : mHDMIInputList)
    {
        bool connectionState;
        hdmiinput::SignalState signalState, lastState;

        if (_portId.value != connectionStateConfig.portId)
        {
            continue;
        }

        UT_ASSERT_TRUE(controlListener->getNextConnectionState(connectionState, chrono::milliseconds(connectionStateConfig.maxWaitTimeMs)));
        UT_ASSERT_EQUAL(connectionState, connectionStateConfig.connectionStateBeforeOpen);

        // verify getConnectionState
        HDMIINPUT_GETCONNECTIONSTATE(controller, connectionState);
        UT_ASSERT_EQUAL(connectionState, connectionStateConfig.connectionStateBeforeOpen);

        const auto timeout = chrono::milliseconds(connectionStateConfig.maxWaitTimeMs);

        for (size_t i = 0; i < connectionStateConfig.connectionStatesAfterOpen.size(); ++i)
        {
            auto expectedConnectionState =
                connectionStateConfig.connectionStatesAfterOpen[i];

            auto expectedSignalState =
                connectionStateConfig.signalStatesAfterOpen[i];

            UT_ASSERT_TRUE(controlListener->getNextConnectionState(connectionState, timeout));
            UT_ASSERT_EQUAL(connectionState, expectedConnectionState);

            UT_ASSERT_TRUE(controlListener->getNextSignalState(signalState, timeout));
            UT_ASSERT_EQUAL(signalState, expectedSignalState);

            lastState = signalState;
        }
    }
}

/**
 * @brief Validate HDMI INPUT connection state transitions using configuration-driven test.
 *
 * This test parses a configuration file to select the HDMI input port and expected connection states.
 * For the specified port, it verifies the sequence of connection state changes reported by the controller listener,
 * including the initial state before open and all subsequent expected states.
 *
 * **Test Group ID:** Basic: 03@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:**@n
 * none@n
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
UT_ADD_TEST(VTS_L3_HDMIINPUT, ConnectionSignalStateTestStart)
{
    const char *testName = "ConnectionSignalStateTestStart";
    UT_status_t status = UT_STATUS_OK;
    hdmiinputConnectionStateStartConfig_t connectionStateConfig;

    /* Parse the test configuration */
    status = vts_hdmiinput_parse_test_config(VTS_HDMIINPUT_TEST_CONFIG_PATH, moduleName, testGroupName, testName, &connectionStateConfig);

    UT_ASSERT_EQUAL_FATAL(status, UT_STATUS_OK);

    for (const auto& [_portId, hdmiInput, controlListener, eventListener, controller, capabilities] : mHDMIInputList)
    {
        bool connectionState;
        hdmiinput::SignalState signalState;

        if (_portId.value != connectionStateConfig.portId)
        {
            continue;
        }

        UT_ASSERT_TRUE(controlListener->getNextConnectionState(connectionState, chrono::milliseconds(connectionStateConfig.maxWaitTimeMs)));
        UT_ASSERT_EQUAL(connectionState, connectionStateConfig.connectionStateBeforeOpen);

        // verify getConnectionState
        HDMIINPUT_GETCONNECTIONSTATE(controller, connectionState);
        UT_ASSERT_EQUAL(connectionState, connectionStateConfig.connectionStateBeforeOpen);

        const auto timeout = chrono::milliseconds(connectionStateConfig.maxWaitTimeMs);

        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);

        UT_ASSERT_TRUE(controlListener->getNextSignalState(signalState, chrono::milliseconds(connectionStateConfig.maxWaitTimeMs)));
        UT_ASSERT_EQUAL(signalState, connectionStateConfig.signalStateBeforeOpen);

        for (size_t i = 0; i < connectionStateConfig.connectionStatesAfterStart.size(); ++i)
        {
            auto expectedConnectionState =
                connectionStateConfig.connectionStatesAfterStart[i];

            auto expectedSignalState =
                connectionStateConfig.signalStatesAfterStart[i];

            UT_ASSERT_TRUE(controlListener->getNextConnectionState(connectionState, timeout));
            UT_ASSERT_EQUAL(connectionState, expectedConnectionState);

            UT_ASSERT_TRUE(controlListener->getNextSignalState(signalState, timeout));
            UT_ASSERT_EQUAL(signalState, expectedSignalState);
        }

        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
    }
}

/** @} */ // End of HDMI_INPUT_HALTEST L3 File
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT Module
/** @} */ // End of HPK