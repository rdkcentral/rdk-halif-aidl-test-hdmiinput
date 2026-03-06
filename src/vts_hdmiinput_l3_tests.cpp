#include <functional>
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

            HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

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

    void StartPort(int32_t portId)
    {
        // Find the matching tuple in mHDMIInputList
        for (const auto& hdmiInputTuple : mHDMIInputList)
        {
            auto& _portId = std::get<0>(hdmiInputTuple);
            if (_portId.value != portId)
                continue;
            auto& hdmiInput = std::get<1>(hdmiInputTuple);
            auto& controller = std::get<4>(hdmiInputTuple);
            HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);
        }
    }

    void StopPort(int32_t portId)
    {
        // Find the matching tuple in mHDMIInputList
        for (const auto& hdmiInputTuple : mHDMIInputList)
        {
            auto& _portId = std::get<0>(hdmiInputTuple);
            if (_portId.value != portId)
                continue;
            auto& hdmiInput = std::get<1>(hdmiInputTuple);
            auto& controller = std::get<4>(hdmiInputTuple);
            HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
        }
    }
    // Helper function to validate connection, signal, and HDCP states for a specific port
    void validateConnectionAndHdcpStates(
        const hdmiinputConnectionConfig_t& connectionStateConfig,
        bool isStarted,
        const IHDMIInput::Id& portId,
        const sp<hdmiinput::IHDMIInput>& hdmiInput,
        const sp<HDMIInputControllerListener>& controlListener,
        const sp<HDMIInputEventListener>& eventListener,
        const sp<hdmiinput::IHDMIInputController>& controller)
    {
        bool connectionState;
        hdmiinput::SignalState signalState;
        size_t i = 0;

        const auto timeout = chrono::milliseconds(connectionStateConfig.maxWaitTimeMs);

        UT_ASSERT_TRUE(controlListener->getNextConnectionState(connectionState, timeout));
        UT_ASSERT_EQUAL(connectionState, connectionStateConfig.connectionStateBeforeOpen);

        // verify getConnectionState
        HDMIINPUT_GETCONNECTIONSTATE(controller, connectionState);
        UT_ASSERT_EQUAL(connectionState, connectionStateConfig.connectionStateBeforeOpen);

        if (isStarted)
        {
            UT_ASSERT_TRUE(controlListener->getNextSignalState(signalState, timeout));
            UT_ASSERT_EQUAL(signalState, connectionStateConfig.signalStateBeforeOpen);
        }

        controlListener->waitForConnectionCount(connectionStateConfig.connectionStates.size(), timeout);
        sleep(1); // Small delay to ensure all events are processed

        // Find the negotiated HDCP protocol versions for this port
        hdmiinput::HDCPProtocolVersion negotiatedHdcpVersion = connectionStateConfig.sourceHdcpProtocolVersion;
        hdmiinput::HDCPProtocolVersion maxDUTHdcpVersion = hdmiinput::HDCPProtocolVersion::UNDEFINED;
        for (const auto& port : gHdmiInput_hfp.ports)
        {
            if (port.id == connectionStateConfig.portId)
            {
                maxDUTHdcpVersion = port.maxHdcpVersion;
                break;
            }
        }

        if(static_cast<int>(maxDUTHdcpVersion) < static_cast<int>(negotiatedHdcpVersion))
        {
            negotiatedHdcpVersion = maxDUTHdcpVersion;
        }

        for (i = 0; i < connectionStateConfig.connectionStates.size(); ++i)
        {
            auto expectedConnectionState = connectionStateConfig.connectionStates[i];
            auto expectedSignalState = connectionStateConfig.signalStates[i];

            // Wait for and verify the next connection state and signal state changes
            UT_ASSERT_TRUE(controlListener->getNextConnectionState(connectionState, timeout));
            UT_ASSERT_EQUAL(connectionState, expectedConnectionState);

            UT_ASSERT_TRUE(controlListener->getNextSignalState(signalState, timeout));
            UT_ASSERT_EQUAL(signalState, expectedSignalState);

            // If the connection state is CONNECTED and a source HDCP protocol version is specified, verify HDCP authentication flow
            if(connectionState && signalState == hdmiinput::SignalState::LOCKED &&
               connectionStateConfig.sourceHdcpProtocolVersion != hdmiinput::HDCPProtocolVersion::UNDEFINED)
            {
                hdmiinput::HDCPProtocolVersion hdcpVersion;
                hdmiinput::HDCPStatus hdcpStatus;

                // Check that HDCP status callback is triggered with AUTHENTICATION_IN_PROGRESS
                UT_ASSERT_TRUE(controlListener->getNextHDCPStatus(hdcpStatus, hdcpVersion, timeout));
                UT_ASSERT_EQUAL(hdcpVersion, negotiatedHdcpVersion);
                UT_ASSERT_EQUAL(hdcpStatus, hdmiinput::HDCPStatus::AUTHENTICATION_IN_PROGRESS);

                // Check that HDCP status callback is triggered with AUTHENTICATED
                UT_ASSERT_TRUE(controlListener->getNextHDCPStatus(hdcpStatus, hdcpVersion, timeout));
                UT_ASSERT_EQUAL(hdcpVersion, negotiatedHdcpVersion);
                UT_ASSERT_EQUAL(hdcpStatus, hdmiinput::HDCPStatus::AUTHENTICATED);

            }
        } //connectionStates loop

        hdmiinput::HDCPProtocolVersion hdcpVersion;
        hdmiinput::HDCPStatus hdcpStatus;

        //check negotiated HDCP version via getHDCPCurrentVersion
        HDMIINPUT_GETHDCPCURRENTVERSION(hdmiInput, hdcpVersion);
        //check HDCP status via getHDCPStatus
        HDMIINPUT_GETHDCPSTATUS(hdmiInput, hdcpStatus);

        if(connectionStateConfig.connectionStates[i-1] == true &&
           connectionStateConfig.signalStates[i-1] == hdmiinput::SignalState::LOCKED &&
           connectionStateConfig.sourceHdcpProtocolVersion != hdmiinput::HDCPProtocolVersion::UNDEFINED)
        {
            UT_ASSERT_EQUAL(hdcpVersion, negotiatedHdcpVersion);
            UT_ASSERT_EQUAL(hdcpStatus, hdmiinput::HDCPStatus::AUTHENTICATED);
        }
        else
        {
            UT_ASSERT_EQUAL(hdcpVersion, hdmiinput::HDCPProtocolVersion::UNDEFINED);
            UT_ASSERT_EQUAL(hdcpStatus, hdmiinput::HDCPStatus::UNKNOWN);
        }
    }

    void validateInfoFrameCallbacks(
        const hdmiinputInfoFramesTestConfig_t& infoFramesTestConfig,
        const IHDMIInput::Id& portId,
        const sp<hdmiinput::IHDMIInput>& hdmiInput,
        const sp<HDMIInputControllerListener>& controlListener,
        const sp<HDMIInputEventListener>& eventListener,
        const sp<hdmiinput::IHDMIInputController>& controller)
    {
        bool connectionState;
        hdmiinput::SignalState signalState;
        size_t i = 0;

        const auto timeout = chrono::milliseconds(infoFramesTestConfig.maxWaitTimeMs);

        UT_ASSERT_TRUE(controlListener->getNextConnectionState(connectionState, timeout));
        UT_ASSERT_TRUE(connectionState);

        // verify getConnectionState
        HDMIINPUT_GETCONNECTIONSTATE(controller, connectionState);
        UT_ASSERT_TRUE(connectionState);

        UT_ASSERT_TRUE(controlListener->getNextSignalState(signalState, timeout));
        UT_ASSERT_EQUAL(signalState, hdmiinput::SignalState::LOCKED);

        for (const auto& frameType : infoFramesTestConfig.infoFrameType)
        {
            if (frameType == "AUDIO")
            {
                std::vector<uint8_t> audioInfoFrame;
                UT_ASSERT_TRUE(controlListener->getNextAudioInfoFrame(audioInfoFrame, timeout));
                UT_ASSERT_EQUAL(audioInfoFrame.size(), infoFramesTestConfig.audioInfoData.size());

                if (audioInfoFrame.size() == infoFramesTestConfig.audioInfoData.size())
                {
                    for (size_t j = 0; j < audioInfoFrame.size(); ++j)
                    {
                        UT_ASSERT_EQUAL(audioInfoFrame[j], infoFramesTestConfig.audioInfoData[j]);
                    }
                }
                i++;
            }
            else if (frameType == "AVI")
            {
                std::vector<uint8_t> aviInfoFrame;
                UT_ASSERT_TRUE(controlListener->getNextAVIInfoFrame(aviInfoFrame, timeout));
                UT_ASSERT_EQUAL(aviInfoFrame.size(), infoFramesTestConfig.aviInfoData.size());

                if (aviInfoFrame.size() == infoFramesTestConfig.aviInfoData.size())
                {
                    for (size_t j = 0; j < aviInfoFrame.size(); ++j)
                    {
                        UT_ASSERT_EQUAL(aviInfoFrame[j], infoFramesTestConfig.aviInfoData[j]);
                    }
                }
                i++;
            }
            else if (frameType == "SPD")
            {
                std::vector<uint8_t> spdInfoFrame;
                UT_ASSERT_TRUE(controlListener->getNextSPDInfoFrame(spdInfoFrame, timeout));
                UT_ASSERT_EQUAL(spdInfoFrame.size(), infoFramesTestConfig.spdInfoData.size());

                if (spdInfoFrame.size() == infoFramesTestConfig.spdInfoData.size())
                {
                    for (size_t j = 0; j < spdInfoFrame.size(); ++j)
                    {
                        UT_ASSERT_EQUAL(spdInfoFrame[j], infoFramesTestConfig.spdInfoData[j]);
                    }
                }
                i++;
            }
            else if (frameType == "VSIF")
            {
                std::vector<uint8_t> vsifInfoFrame;
                UT_ASSERT_TRUE(controlListener->getNextVendorSpecificInfoFrame(vsifInfoFrame, timeout));
                UT_ASSERT_EQUAL(vsifInfoFrame.size(), infoFramesTestConfig.vsiInfoData.size());

                if (vsifInfoFrame.size() == infoFramesTestConfig.vsiInfoData.size())
                {
                    for (size_t j = 0; j < vsifInfoFrame.size(); ++j)
                    {
                        UT_ASSERT_EQUAL(vsifInfoFrame[j], infoFramesTestConfig.vsiInfoData[j]);
                    }
                }
                i++;
            }
            else if (frameType == "VIC")
            {
                hdmiinput::VIC vic;
                UT_ASSERT_TRUE(controlListener->getNextVideoFormat(vic, timeout));
                UT_ASSERT_EQUAL(vic, infoFramesTestConfig.vic);
            }
            else if(frameType == "VRR")
            {
                bool vrrActive, M_CONST, fastVActive;
                double frameRate;
                UT_ASSERT_TRUE(controlListener->getNextVRRInfo(vrrActive, M_CONST, fastVActive, frameRate, timeout));
                UT_ASSERT_EQUAL(vrrActive, infoFramesTestConfig.vrrActive);
                UT_ASSERT_EQUAL(M_CONST, infoFramesTestConfig.mConst);
                UT_ASSERT_EQUAL(fastVActive, infoFramesTestConfig.fastVActive);
                UT_ASSERT_EQUAL(frameRate, infoFramesTestConfig.frameRate);
            }
        }
    }
};

/**
 * @brief Validate HDMI INPUT connection state transitions in READY state using configuration-driven test.
 *
 * This test parses a configuration file to select the HDMI input port and expected connection states.
 * For the specified port in READY state, it verifies the sequence of connection state changes reported by the controller listener,
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
 * none@n
 *
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * Refer to UT specification documentation [hdmiinput_High-Level_TestSpecification.md](../docs/pages/hdmiinput_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L3_HDMIINPUT, ConnectionTestAfterOpen)
{
    const char *testName = "ConnectionTestAfterOpen";
    UT_status_t status = UT_STATUS_OK;
    std::vector<hdmiinputConnectionConfig_t> connectionStateConfig;

    /* Parse the test configuration */
    status = vts_hdmiinput_parse_connectionTest(VTS_HDMIINPUT_TEST_CONFIG_PATH, moduleName, testGroupName, testName, connectionStateConfig);

    UT_ASSERT_EQUAL_FATAL(status, UT_STATUS_OK);

    // Loop over each configured port
    for (const auto& config : connectionStateConfig)
    {
        int32_t portId = config.portId;
        // Find the matching tuple in mHDMIInputList
        for (const auto& hdmiInputTuple : mHDMIInputList)
        {
            auto& _portId = std::get<0>(hdmiInputTuple);
            if (_portId.value != portId)
                continue;
            auto& hdmiInput = std::get<1>(hdmiInputTuple);
            auto& controlListener = std::get<2>(hdmiInputTuple);
            auto& eventListener = std::get<3>(hdmiInputTuple);
            auto& controller = std::get<4>(hdmiInputTuple);

            validateConnectionAndHdcpStates(config, false, _portId, hdmiInput, controlListener, eventListener, controller);
        }
    }
}

/**
 * @brief Validate HDMI INPUT connection and signal state transitions after starting, using configuration-driven test.
 *
 * This test parses a configuration file to select the HDMI input port and expected connection and signal states.
 * For the specified port, it verifies the sequence of connection state changes and signal state changes reported by the controller listener,
 * including the initial state before open, the state after start, and all subsequent expected states.
 * If HDCP protocol version is specified, it also verifies the HDCP authentication flow.
 *
 * **Pre-Conditions :**
 * None
 *
 * **Dependencies :**
 * None
 *
 * **User Interaction :**
 * None
 *
 * **Test Procedure:**@n
 * Refer to UT specification documentation [hdmiinput_High-Level_TestSpecification.md](../docs/pages/hdmiinput_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L3_HDMIINPUT, ConnectionTestAfterStart)
{
    const char *testName = "ConnectionTestAfterStart";
    UT_status_t status = UT_STATUS_OK;
    std::vector<hdmiinputConnectionConfig_t> connectionStateConfig;

    /* Parse the test configuration */
    status = vts_hdmiinput_parse_connectionTest(VTS_HDMIINPUT_TEST_CONFIG_PATH, moduleName, testGroupName, testName, connectionStateConfig);

    UT_ASSERT_EQUAL_FATAL(status, UT_STATUS_OK);

    for (const auto& config : connectionStateConfig)
    {
        StartPort(config.portId);
    }

    // Loop over each configured port
    for (const auto& config : connectionStateConfig)
    {
        int32_t portId = config.portId;
        for (const auto& hdmiInputTuple : mHDMIInputList)
        {
            auto& _portId = std::get<0>(hdmiInputTuple);
            if (_portId.value != portId)
                continue;
            auto& hdmiInput = std::get<1>(hdmiInputTuple);
            auto& controlListener = std::get<2>(hdmiInputTuple);
            auto& eventListener = std::get<3>(hdmiInputTuple);
            auto& controller = std::get<4>(hdmiInputTuple);

            validateConnectionAndHdcpStates(config, true, _portId, hdmiInput, controlListener, eventListener, controller);
        }
    }

    for (const auto& config : connectionStateConfig)
    {
        StopPort(config.portId);
    }
}

/**
 * @brief Validate HDMI INPUT info frame callbacks using configuration-driven test.
 *
 * This test parses a configuration file to select the HDMI input port and expected info frame types and data.
 * For each specified port, it verifies the sequence of info frame callbacks (AUDIO, AVI, SPD, VSIF, VIC, etc.) reported by the controller listener,
 * and checks that the received info frame data matches the expected values from the configuration file.
 *
 * **Pre-Conditions :**
 * None
 *
 * **Dependencies :**
 * None
 *
 * **User Interaction :**
 * None
 *
 * **Test Procedure:**@n
 * Refer to UT specification documentation [hdmiinput_High-Level_TestSpecification.md](../docs/pages/hdmiinput_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L3_HDMIINPUT, InfoFramesTest)
{
    const char *testName = "InfoFramesTest";
    UT_status_t status = UT_STATUS_OK;
    std::vector<hdmiinputInfoFramesTestConfig_t> infoFramesTestConfig;

    /* Parse the test configuration */
    status = vts_hdmiinput_parse_infoFramesTestConfig(VTS_HDMIINPUT_TEST_CONFIG_PATH, moduleName, testGroupName, testName, infoFramesTestConfig);

    UT_ASSERT_EQUAL_FATAL(status, UT_STATUS_OK);

    for (const auto& config : infoFramesTestConfig)
    {
        StartPort(config.portId);
    }

    // Loop over each configured port
    for (const auto& config : infoFramesTestConfig)
    {
        int32_t portId = config.portId;
        for (const auto& hdmiInputTuple : mHDMIInputList)
        {
            auto& _portId = std::get<0>(hdmiInputTuple);
            if (_portId.value != portId)
                continue;
            auto& hdmiInput = std::get<1>(hdmiInputTuple);
            auto& controlListener = std::get<2>(hdmiInputTuple);
            auto& eventListener = std::get<3>(hdmiInputTuple);
            auto& controller = std::get<4>(hdmiInputTuple);
            validateInfoFrameCallbacks(config, _portId, hdmiInput, controlListener, eventListener, controller);
        }
    }

    for (const auto& config : infoFramesTestConfig)
    {
        StopPort(config.portId);
    }
}

/**
 * @brief Validate HDMI INPUT info frame callbacks using configuration-driven test.
 *
 * This test parses a configuration file to select the HDMI input port and expected info frame types and data.
 * For each specified port, it verifies the sequence of info frame callbacks (AUDIO, AVI, SPD, VSIF, VIC, etc.) reported by the controller listener,
 * and checks that the received info frame data matches the expected values from the configuration file.
 *
 * **Pre-Conditions :**
 * None
 *
 * **Dependencies :**
 * None
 *
 * **User Interaction :**
 * None
 *
 * **Test Procedure:**@n
 * Refer to UT specification documentation [hdmiinput_High-Level_TestSpecification.md](../docs/pages/hdmiinput_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L3_HDMIINPUT, EDIDTest)
{
    const char *testName = "EDIDTest";
    UT_status_t status = UT_STATUS_OK;
    std::vector<hdmiinputEdidTestConfig_t> edidTestConfig;
    std::vector<uint8_t> changedEDID;

    /* Parse the test configuration */
    status = vts_hdmiinput_parse_edidTestConfig(VTS_HDMIINPUT_TEST_CONFIG_PATH, moduleName, testGroupName, testName, edidTestConfig);

    UT_ASSERT_EQUAL_FATAL(status, UT_STATUS_OK);

    // Loop over each configured port
    for (const auto& config : edidTestConfig)
    {
        int32_t portId = config.portId;
        // Find the matching tuple in mHDMIInputList
        for (const auto& hdmiInputTuple : mHDMIInputList)
        {
            auto& _portId = std::get<0>(hdmiInputTuple);
            if (_portId.value != portId)
                continue;
            auto& hdmiInput = std::get<1>(hdmiInputTuple);
            auto& controlListener = std::get<2>(hdmiInputTuple);
            auto& eventListener = std::get<3>(hdmiInputTuple);
            auto& controller = std::get<4>(hdmiInputTuple);

            HDMIINPUT_SETEDID_SUCCESS(controller, config.edidData);

            // Wait for and verify onEDIDChange() callback
            UT_ASSERT_TRUE(eventListener->getNextEDIDChange(changedEDID));
            UT_ASSERT_EQUAL(changedEDID.size(), config.edidData.size());
            for (size_t i = 0; i < changedEDID.size(); ++i)
            {
                UT_ASSERT_EQUAL(changedEDID[i], config.edidData[i]);
            }

            changedEDID.clear();

            // Verify getEDID() returns the new value
            HDMIINPUT_GETEDID_SUCCESS(hdmiInput, changedEDID);
            UT_ASSERT_EQUAL(changedEDID.size(), config.edidData.size());
            for (size_t i = 0; i < changedEDID.size(); ++i)
            {
                UT_ASSERT_EQUAL(changedEDID[i], config.edidData[i]);
            }
        }
    }
}
/** @} */ // End of HDMI_INPUT_HALTEST L3 File
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT Module
/** @} */ // End of HPK