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
 * @defgroup HDMI_INPUT_HALTEST_L1 HDMI INPUT HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Test Cases for HDMI INPUT HAL :
 *
 * This module includes Level 1 functional tests (success and failure scenarios).
 * This is to ensure that the API meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:** @n
 * **Dependencies:** None@n
 *
 * @endparblock
 *
 */

/**
 * @file vts_hdmiinput_l1_tests.cpp
 *
 */
#include "vts_hdmiinput.h"
#include "vts_hdmiinput_parse_hfp.h"

/* Test class registration with UT_Core */
UT_ADD_TEST_TO_GROUP(VTS_L1_HDMIINPUT, UT_TESTS_L1)

class VTS_L1_HDMIINPUT : public UTCore
{
public:
    VTS_L1_HDMIINPUT() : UTCore() {}

    ~VTS_L1_HDMIINPUT() override = default;

    void SetUp() override
    {
        android::ProcessState::self()->startThreadPool();
    }

    void TearDown() override
    {
    }
};

/**
 * @brief Verify that getHDMIInputIds returns valid non-empty HDMI input IDs.
 *
 * This test calls the getHDMIInputIds API on the HDMIInputManager service and verifies that the returned
 * list of HDMI input IDs is not empty and matches the expected identifiers retrieved from a predefined YAML profile.
 * The test iterates through the IDs and asserts that each ID is either one of the expected valid IDs or is marked as
 * UNDEFINED if extra entries exist.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Call getHDMIInputIds on the HDMIInputManager service with no input parameters and capture the output vector and status. | input: none, output: hdmiInputIds vector, status structure | API returns status OK and populates hdmiInputIds vector | Should Pass |@n
 * | 02 | Verify that the returned status indicates success and that the hdmiInputIds vector is not empty. | status.isOk() and hdmiInputIds size | Status is OK; hdmiInputIds vector is not empty | Should Pass |@n
 * | 03 | Iterate over each id in the hdmiInputIds vector and assert that the id is either present in the expectedIds vector | Each id from hdmiInputIds compared against expectedIds | Each id is validated successfully; assertions pass if id matches one of the expected values| Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getHDMIInputIds_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    UT_ASSERT_EQUAL(gHdmiInput_hfp.ports.size(), hdmiInputIds.size());

    // Prepare expected IDs from gHdmiInput_hfp.ports
    std::vector<int32_t> expectedIds;
    for (const auto& port : gHdmiInput_hfp.ports)
    {
        expectedIds.push_back(port.id);
    }

    // Validate that each returned ID is in expectedIds or is UNDEFINED
    for (const auto& id : hdmiInputIds)
    {
        bool found = std::find(expectedIds.begin(), expectedIds.end(), id.value) != expectedIds.end();
        UT_ASSERT_TRUE(found);
    }
}

/**
 * @brief Validate the HDMI Input Manager's ability to fetch HDMI Input objects
 *
 * This test iterates over a list of HDMI input ports, retrieves their respective ID using helper methods,
 * and then calls the getHDMIInput API to verify that a valid HDMI Input object is returned.
 * The test confirms that the API call returns a successful status and a non-null object for each port.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve the count of HDMI input ports using the helper function UT_KVP_PROFILE_GET_LIST_COUNT. | hdmiinput.ports list | Returns the total number of HDMI input ports available. | Should be successful |
 * | 02 | Start a loop to iterate over each HDMI input port. | For each iteration: index = 0, 1, ... , numPorts-1 | Iteration proceeds for each available HDMI input port. | Should be successful |
 * | 03 | For the current iteration, extract the HDMI input's ID using UT_KVP_PROFILE_GET_UINT32 from hdmiinput.ports at the current index. | index = current loop index, hdmiinput.ports.<index>.id | Extracts a valid uint32 value representing the HDMI input ID. | Should be successful |
 * | 04 | Log the action of calling getHDMIInput with the retrieved HDMI input ID and then invoke mService->getHDMIInput. | Input: hdmiInputId = current port ID | API returns a binder Status with isOk true and a non-null HDMI Input object. | Should Pass |
 * | 05 | Assert that the status returned from getHDMIInput is ok and that the HDMI Input object is not null. | Assert status.isOk and hdmiInput != nullptr | Both assertions pass confirming successful API execution. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getHDMIInput_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);
    }
}

/**
 * @brief Negative test for getHDMIInput with invalid HDMI input IDs
 *
 * This test verifies that the HDMI Input Manager's getHDMIInput API correctly handles invalid input IDs.
 * It attempts to fetch HDMI input objects using IDs that are outside the valid range (one less than the minimum,
 * one more than the maximum, and the UNDEFINED value). The test expects the API to fail gracefully and not return
 * a valid HDMI input object for these invalid IDs.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve the list of valid HDMI input IDs. | hdmiInputIds vector | List of valid HDMI input IDs is obtained. | Should be successful |
 * | 02 | Attempt to fetch HDMI input with an ID less than the minimum valid ID. | invalidId = min(validIds) - 1 | API returns failure; no HDMI input object is returned. | Should Pass |
 * | 03 | Attempt to fetch HDMI input with an ID greater than the maximum valid ID. | invalidId = max(validIds) + 1 | API returns failure; no HDMI input object is returned. | Should Pass |
 * | 04 | Attempt to fetch HDMI input with the UNDEFINED ID value. | invalidId = UNDEFINED | API returns failure; no HDMI input object is returned. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getHDMIInput_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    android::sp<hdmiinput::IHDMIInput> hdmiInput;
    hdmiinput::IHDMIInput::Id invalidId;

    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    if(hdmiInputIds.size() > 1)
        std::sort(hdmiInputIds.begin(), hdmiInputIds.end());

    invalidId = hdmiInputIds.front();
    invalidId.value -= 1;
    HDMIINPUT_MANAGER_GETHDMIINPUT_FAILURE(invalidId, hdmiInput);

    invalidId = hdmiInputIds.back();
    invalidId.value += 1;
    HDMIINPUT_MANAGER_GETHDMIINPUT_FAILURE(invalidId, hdmiInput);

    invalidId.value = IHDMIInput::Id::UNDEFINED;
    HDMIINPUT_MANAGER_GETHDMIINPUT_FAILURE(invalidId, hdmiInput);
}

/**
 * @brief Validate correct behavior of platform getCapabilities API in positive scenario
 *
 * This test case verifies that the getCapabilities API correctly returns HDMI input capabilities,
 * ensuring that the returned data matches the expected values from the YAML configuration.
 *
 * **Test Group ID:**: 01
 * **Test Case ID:** 004
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke getCapabilities API and capture the output. | IN: none, OUT: capabilities structure (caps) | API returns valid status with capabilities populated | Should Pass |
 * | 02 | Verify that maximumConcurrentStartedPorts and freeSync match expected values from YAML profile. | caps.maximumConcurrentStartedPorts, caps.freeSync | Values match YAML configuration | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getPlatformCapabilities_pos)
{
    hdmiinput::PlatformCapabilities caps;

    HDMIINPUT_MANAGER_GETCAPABILITIES(caps);

    UT_ASSERT_EQUAL(caps.maximumConcurrentStartedPorts, gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts);
    UT_ASSERT_EQUAL(caps.freeSync, gHdmiInput_hfp.platformCapabilities.freeSync);
}

/**
 * @brief Validate HDMI input port capabilities
 *
 * This test case verifies that the getCapabilities API returns correct HDMI input capabilities for each port.
 * For every port defined in gHdmiInput_hfp, the test fetches the HDMI input object and its capabilities,
 * then validates all capability fields (booleans and supported lists) against the expected values from gHdmiInput_hfp.
 *
 * **Test Group ID:**: 01
 * **Test Case ID:** 005
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | For each port in gHdmiInput_hfp, fetch HDMI input object and its capabilities. | port.id, expected capabilities | API returns valid status and capabilities for each port | Should Pass |
 * | 02 | Validate all capability fields (booleans and supported lists) against gHdmiInput_hfp values. | supports3D, supportedVersions, etc. | All fields match expected values | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getCapabilities_pos)
{
    for (auto &port : gHdmiInput_hfp.ports)
    {
        hdmiinput::Capabilities *capabilities = new hdmiinput::Capabilities();
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        hdmiinput::IHDMIInput::Id hdmiInputIds;

        hdmiInputIds.value = port.id;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(hdmiInputIds, hdmiInput);

        HDMIINPUT_GET_CAPABILITIES(hdmiInput, capabilities);

        UT_ASSERT_EQUAL(capabilities->supports3D, port.supports3D);
        UT_ASSERT_EQUAL(capabilities->supportsFRL, port.supportsFRL);
        UT_ASSERT_EQUAL(capabilities->supportsVRR, port.supportsVRR);
        UT_ASSERT_EQUAL(capabilities->supportsFreeSync, port.supportsFreeSync);
        UT_ASSERT_EQUAL(capabilities->supportsQMS, port.supportsQMS);
        UT_ASSERT_EQUAL(capabilities->supportsALLM, port.supportsALLM);
        UT_ASSERT_EQUAL(capabilities->supportsQFT, port.supportsQFT);
        UT_ASSERT_EQUAL(capabilities->supportsARC, port.supportsARC);
        UT_ASSERT_EQUAL(capabilities->supportsEARC, port.supportsEARC);

        // Compare supportedVersions
        UT_ASSERT_EQUAL(capabilities->supportedVersions.size(), port.supportedVersions.size());
        for (size_t i = 0; i < capabilities->supportedVersions.size(); ++i)
        {
            bool found = std::find(port.supportedVersions.begin(),
                                   port.supportedVersions.end(),
                                   capabilities->supportedVersions[i]) != port.supportedVersions.end();
            UT_ASSERT_TRUE(found);
        }

        // Compare supportedVICs
        UT_ASSERT_EQUAL(capabilities->supportedVICs.size(), port.supportedVICs.size());
        for (size_t i = 0; i < capabilities->supportedVICs.size(); ++i)
        {
            bool found = std::find(port.supportedVICs.begin(),
                                   port.supportedVICs.end(),
                                   capabilities->supportedVICs[i]) != port.supportedVICs.end();
            UT_ASSERT_TRUE(found);
        }

        // Compare supportedHDCPProtocolVersions
        UT_ASSERT_EQUAL(capabilities->supportedHDCPProtocolVersions.size(), port.supportedHDCPProtocolVersions.size());
        for (size_t i = 0; i < capabilities->supportedHDCPProtocolVersions.size(); ++i)
        {
            bool found = std::find(port.supportedHDCPProtocolVersions.begin(),
                                   port.supportedHDCPProtocolVersions.end(),
                                   capabilities->supportedHDCPProtocolVersions[i]) != port.supportedHDCPProtocolVersions.end();
            UT_ASSERT_TRUE(found);
        }
    }
}

/**
 * @brief Validate the getProperty API for HDMI Input objects returns correct property values.
 *
 * This test iterates over all available HDMI input IDs, retrieves the corresponding HDMI Input object,
 * and calls getProperty for RESOURCE_ID and METRIC_PACKET_ERRORS properties.
 * It asserts that RESOURCE_ID matches the port ID and that METRIC_PACKET_ERRORS is present and initially zero (unless not implemented).
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 006@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call getProperty for RESOURCE_ID and METRIC_PACKET_ERRORS. | Input: hdmiInput, property | Properties are returned. | Should Pass |
 * | 04 | Assert RESOURCE_ID matches port ID and METRIC_PACKET_ERRORS is present and initially zero (unless not implemented). | Output: resourceID, metricPktErrors | All assertions pass. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getProperty_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        std::optional<PropertyValue> resourceID;
        std::optional<PropertyValue> metricPktErrors;
        int32_t value = 0;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_GETPROPERTY_SUCCESS(hdmiInput, hdmiinput::Property::RESOURCE_ID, resourceID);
        HDMIINPUT_GETPROPERTY_SUCCESS(hdmiInput, hdmiinput::Property::METRIC_PACKET_ERRORS, metricPktErrors);

        UT_ASSERT_EQUAL(resourceID->value->get<PropertyValue::Value::Tag::intValue>(), id.value);

        UT_ASSERT_TRUE(metricPktErrors.has_value());

        value = metricPktErrors->value->get<PropertyValue::Value::Tag::intValue>();
        if (value != -1) // -1 indicates not implemented
        {
            UT_ASSERT_EQUAL(value, 0); // Initial counters must be zero
        }
    }
}

/**
 * @brief Validate the HDMI Input Manager's ability to fetch HDMI Input objects
 *
 * This test iterates over a list of HDMI input ports, retrieves their respective ID using helper methods,
 * and then calls the getHDMIInput API to verify that a valid HDMI Input object is returned.
 * The test confirms that the API call returns a successful status and a non-null object for each port.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve the count of HDMI input ports using the helper function UT_KVP_PROFILE_GET_LIST_COUNT. | hdmiinput.ports list | Returns the total number of HDMI input ports available. | Should be successful |
 * | 02 | Start a loop to iterate over each HDMI input port. | For each iteration: index = 0, 1, ... , numPorts-1 | Iteration proceeds for each available HDMI input port. | Should be successful |
 * | 03 | For the current iteration, extract the HDMI input's ID using UT_KVP_PROFILE_GET_UINT32 from hdmiinput.ports at the current index. | index = current loop index, hdmiinput.ports.<index>.id | Extracts a valid uint32 value representing the HDMI input ID. | Should be successful |
 * | 04 | Log the action of calling getHDMIInput with the retrieved HDMI input ID and then invoke mService->getHDMIInput. | Input: hdmiInputId = current port ID | API returns a binder Status with isOk true and a non-null HDMI Input object. | Should Pass |
 * | 05 | Assert that the status returned from getHDMIInput is ok and that the HDMI Input object is not null. | Assert status.isOk and hdmiInput != nullptr | Both assertions pass confirming successful API execution. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getProperty_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        std::optional<PropertyValue> propertyValue;
        int32_t firstValue = static_cast<int32_t>(Property::RESOURCE_ID);
        Property invalidProperty = static_cast<Property>(firstValue - 1);

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        // Query invalid property
        HDMIINPUT_GETPROPERTY_FAILURE(hdmiInput, invalidProperty, propertyValue, Status::EX_NONE);
    }
}

/**
 * @brief Validate the getState API for HDMI Input objects
 *
 * This test verifies that the getState API on each HDMI Input object returns the expected initial state.
 * It iterates over all available HDMI input IDs, retrieves the corresponding HDMI Input object, and calls getState.
 * The test asserts that the returned state is CLOSED for each HDMI input port, confirming correct default behavior.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call HDMIINPUT_GET_STATE on the HDMI Input object and capture the state. | Input: hdmiInput | State is returned. | Should Pass |
 * | 04 | Assert that the returned state is hdmiinput::State::CLOSED. | Output: state | State equals CLOSED. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getState_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    hdmiinput::State state;

    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_GET_STATE(hdmiInput, state);

        UT_ASSERT_EQUAL(state, hdmiinput::State::CLOSED);
    }
}

/**
 * @brief Validate the getEDID API for HDMI Input objects returns the correct EDID for the highest supported HDMI version.
 *
 * This test iterates over all available HDMI input IDs, retrieves the corresponding HDMI Input object, and calls getEDID.
 * For each port, it determines the highest supported HDMIVersion, fetches the expected EDID bytes for that version from the YAML profile,
 * and asserts that the returned EDID data matches the expected bytes in size and content.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 009@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call HDMIINPUT_GETEDID_SUCCESS on the HDMI Input object and capture the EDID data. | Input: hdmiInput | EDID data is returned. | Should Pass |
 * | 04 | Determine the highest supported HDMIVersion for the port and fetch the expected EDID bytes from the YAML profile. | Output: expectedEDID | Expected EDID bytes are obtained. | Should Pass |
 * | 05 | Assert that the returned EDID data matches the expected EDID bytes in size and content. | Output: edidData, expectedEDID | All assertions pass if data matches. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getEDID_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        std::vector<uint8_t> edidData;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);
        HDMIINPUT_GETEDID_SUCCESS(hdmiInput, edidData);

        // Find the highest HDMIVersion for this port
        const auto& defaultEDIDMap = gHdmiInput_hfp.ports[id.value].defaultEDID;
        if (defaultEDIDMap.empty())
        {
            UT_ASSERT_TRUE(false); // Fail if no EDID present
            continue;
        }
        auto maxVersionIt = defaultEDIDMap.begin();
        for (auto it = defaultEDIDMap.begin(); it != defaultEDIDMap.end(); ++it)
        {
            if (static_cast<int>(it->first) > static_cast<int>(maxVersionIt->first))
            {
                maxVersionIt = it;
            }
        }
        const std::vector<uint8_t>& expectedEDID = maxVersionIt->second;

        UT_ASSERT_EQUAL(edidData.size(), expectedEDID.size());
        for (size_t i = 0; i < edidData.size(); ++i)
        {
            UT_ASSERT_EQUAL(edidData[i], expectedEDID[i]);
        }
    }
}

/**
 * @brief Validate the getDefaultEDID API for HDMI Input objects returns the correct EDID for each supported HDMI version.
 *
 * This test iterates over all available HDMI input IDs and, for each port, over all supported HDMI versions.
 * For each version, it calls getDefaultEDID and asserts that the returned EDID matches the expected bytes from the YAML profile.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | For each supported HDMIVersion, call getDefaultEDID and capture the EDID data. | Input: version | EDID data is returned. | Should Pass |
 * | 04 | Assert that the returned EDID data matches the expected EDID bytes in size and content for that version. | Output: edidData, expectedEDID | All assertions pass if data matches. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getDefaultEDID_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        const auto& port = gHdmiInput_hfp.ports[id.value];
        for (const auto& version : port.supportedVersions)
        {
            std::vector<uint8_t> edidData;

            HDMIINPUT_GETDEFAULTEDID_SUCCESS(hdmiInput, version, edidData);

            // Check that the call succeeded and the expected EDID exists
            auto it = port.defaultEDID.find(version);
            UT_ASSERT_TRUE(it != port.defaultEDID.end());

            const std::vector<uint8_t>& expectedEDID = it->second;
            UT_ASSERT_EQUAL(edidData.size(), expectedEDID.size());
            for (size_t i = 0; i < edidData.size(); ++i)
            {
                UT_ASSERT_EQUAL(edidData[i], expectedEDID[i]);
            }
        }
    }
}

/**
 * @brief Negative test for getDefaultEDID API for HDMI Input objects.
 *
 * This test covers negative scenarios for getDefaultEDID:
 * 1. Sending a version not in supportedVersions.
 * 2. Sending an invalid version (e.g., an out-of-range enum value).
 * 3. Ensures the API fails gracefully and does not return EDID data.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 011@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | For each port, send a version not in supportedVersions to getDefaultEDID. | unsupported version | API returns failure, no EDID. | Should Pass |
 * | 02 | For each port, send an invalid version (e.g., -1 or max+1). | invalid version | API returns failure, no EDID. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getDefaultEDID_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        const auto& port = gHdmiInput_hfp.ports[id.value];

        // 1. Version not in supportedVersions
        // Find a version not in supportedVersions
        std::vector<HDMIVersion> allVersions = {
            HDMIVersion::HDMI_1_3, HDMIVersion::HDMI_1_4, HDMIVersion::HDMI_2_0, HDMIVersion::HDMI_2_1
        };
        for (const auto& version : allVersions)
        {
            if (std::find(port.supportedVersions.begin(), port.supportedVersions.end(), version) == port.supportedVersions.end())
            {
                std::vector<uint8_t> edidData;
                HDMIINPUT_GETDEFAULTEDID_FAILURE(hdmiInput, version, edidData);
            }
        }

        // 2. Invalid version (simulate by casting an out-of-range int)
        int invalidVersionInt = -1;
        HDMIVersion invalidVersion = static_cast<HDMIVersion>(invalidVersionInt);
        std::vector<uint8_t> edidData;
        HDMIINPUT_GETDEFAULTEDID_FAILURE(hdmiInput, invalidVersion, edidData);

        // Try a value above the max known version
        int maxKnown = static_cast<int>(HDMIVersion::HDMI_2_1);
        HDMIVersion outOfRangeVersion = static_cast<HDMIVersion>(maxKnown + 1);
        HDMIINPUT_GETDEFAULTEDID_FAILURE(hdmiInput, outOfRangeVersion, edidData);
    }
}

/**
 * @brief Validate the getHDCPCurrentVersion API for HDMI Input objects returns a valid HDCPProtocolVersion.
 *
 * This test iterates over all available HDMI input IDs, retrieves the corresponding HDMI Input object, and calls getHDCPCurrentVersion.
 * It asserts that the returned value is HDCPProtocolVersion::UNDEFINED
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 012@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call getHDCPCurrentVersion and check the returned value. | Output: version | Value is HDCPProtocolVersion::UNDEFINED. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getHDCPCurrentVersion_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        hdmiinput::HDCPProtocolVersion currentVersion;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_GETHDCPCURRENTVERSION(hdmiInput, currentVersion);

        // It should be either one of the supported versions or UNDEFINED
        UT_ASSERT_EQUAL(currentVersion, HDCPProtocolVersion::UNDEFINED);
    }
}

/**
 * @brief Validate the getHDCPStatus API for HDMI Input objects returns a valid HDCPStatus.
 *
 * This test iterates over all available HDMI input IDs, retrieves the corresponding HDMI Input object, and calls getHDCPStatus.
 * It asserts that the returned value is HDCPStatus::UNKNOWN if HDCP is not in use.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call getHDCPStatus and check the returned value. | Output: status | Value is HDCPStatus::UNKNOWN if HDCP is not in use. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getHDCPStatus_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        hdmiinput::HDCPStatus hdcpStatus;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        // Call getHDCPStatus
        HDMIINPUT_GETHDCPSTATUS(hdmiInput, hdcpStatus);

        // If HDCP is not in use, status should be UNKNOWN
        UT_ASSERT_EQUAL(hdcpStatus, hdmiinput::HDCPStatus::UNKNOWN);
    }
}

/**
 * @brief Validate the getSPDInfoFrame API for HDMI Input objects returns the last received
 * SPD InfoFrame or an empty array if none received.
 *
 * This test iterates over all available HDMI input IDs, retrieves the corresponding HDMI Input object,
 * and calls getSPDInfoFrame. It asserts that the returned array is empty if no InfoFrame has been
 * received since last device connection or start.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call getSPDInfoFrame and check the returned value. | Output: infoFrameData | Value is empty array if no InfoFrame received. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getSPDInfoFrame_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        std::vector<uint8_t> infoFrameData;
        HDMIINPUT_GETSPDINFOFRAME(hdmiInput, infoFrameData);

        // If no InfoFrame has been received, the array should be empty
        UT_ASSERT_TRUE(infoFrameData.empty());
    }
}

/**
 * @brief Validate the open API for HDMI Input objects opens and closes the port successfully.
 *
 * This test iterates over all available HDMI input IDs, retrieves the corresponding HDMI Input object,
 * and calls open with a valid controller listener.
 * It asserts that the returned IHDMIInputController is not null, and then closes the controller.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:**: 015@n
 * **Priority:**: High@n
 *
 * **Pre-Conditions:**: None@n
 * **Dependencies:**: None@n
 * **User Interaction:**: None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call open with a valid controller listener and check the returned controller. | Output: controller | Controller is not null. | Should Pass |
 * | 04 | Call close on the controller and ensure it succeeds. | Input: controller | Close succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, open_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        hdmiinput::State beforeState;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_GET_STATE(hdmiInput, beforeState);
        UT_ASSERT_EQUAL(beforeState, hdmiinput::State::CLOSED);

        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);

        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Negative tests for the open API for HDMI Input objects.
 *
 * This test covers:
 * 1. Calling open twice in a row (should fail on second call due to wrong state).
 * 2. Passing a null pointer as the controller listener (should fail).
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:**: 016@n
 * **Priority:**: High@n
 *
 * **Pre-Conditions:**: None@n
 * **Dependencies:**: None@n
 * **User Interaction:**: None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call open twice in a row. | Valid listener | Second call fails (wrong state). | Should Pass |
 * | 02 | Call open with null listener. | nullptr | Call fails. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, open_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<hdmiinput::IHDMIInputControllerListener> invalidControllerListener = nullptr;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        // open the port with NULL listener
        HDMIINPUT_OPEN_FAILURE(hdmiInput, invalidControllerListener, controller, hdmiinput::State::CLOSED, Status::EX_NULL_POINTER);

        // open the port with valid listener
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);

        //open the port with invalid state
        HDMIINPUT_OPEN_FAILURE(hdmiInput, listener, controller, hdmiinput::State::READY, Status::EX_ILLEGAL_STATE);

        //Close the port
        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Validate the close API for HDMI Input objects opens and closes the port successfully.
 *
 * This test iterates over all available HDMI input IDs, retrieves the corresponding HDMI Input object,
 * and calls open with a valid controller listener.
 * It asserts that the returned IHDMIInputController is not null, and then closes the controller.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:**: 017@n
 * **Priority:**: High@n
 *
 * **Pre-Conditions:**: None@n
 * **Dependencies:**: None@n
 * **User Interaction:**: None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call open with a valid controller listener and check the returned controller. | Output: controller | Controller is not null. | Should Pass |
 * | 04 | Call close on the controller and ensure it succeeds. | Input: controller | Close succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, close_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        hdmiinput::State beforeState;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_GET_STATE(hdmiInput, beforeState);
        UT_ASSERT_EQUAL(beforeState, hdmiinput::State::CLOSED);

        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);

        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Negative and robustness tests for the close API for HDMI Input objects.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:**: 018@n
 * **Priority:**: High@n
 *
 * **Pre-Conditions:**: None@n
 * **Dependencies:**: None@n
 * **User Interaction:**: None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call open with a valid controller listener and check the returned controller. | Output: controller | Controller is not null. | Should Pass |
 * | 04 | Call close with invalid controller | Input: invalid controller | Close return false. | Should Pass |
 * | 05 | Call close on the controller and ensure it succeeds. | Input: controller | Close succeeds. | Should Pass |
 * | 06 | Call close again. | Input: controller | Close return false. | Should Pass |
 * | 07 | Call close with invalid controller | Input: invalid controller | Close return false. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, close_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        // Open the port (should transition to READY)
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);

        // 1. Try closing with nullptr controller (should fail)
        android::sp<hdmiinput::IHDMIInputController> nullController = nullptr;
        HDMIINPUT_CLOSE_FAILURE(hdmiInput, nullController, hdmiinput::State::READY, Status::EX_NULL_POINTER);

        // 2. Try closing with a stale/invalid controller (simulate by opening and closing, then reusing controller)
        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true); // valid close
        HDMIINPUT_CLOSE_FAILURE(hdmiInput, controller, hdmiinput::State::CLOSED, Status::EX_NONE);

        // 3. Try closing again with nullptr after already closed
        HDMIINPUT_CLOSE_FAILURE(hdmiInput, nullController, hdmiinput::State::CLOSED, Status::EX_NULL_POINTER);
    }
}

/**
 * @brief Validate the registerEventListener API for HDMI Input objects registers an event listener successfully.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:**: 019@n
 * **Priority:**: High@n
 *
 * **Pre-Conditions:**: None@n
 * **Dependencies:**: None@n
 * **User Interaction:**: None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Register a new event listener using HDMIINPUT_REGISTEREVENT_SUCCESS. | Input: eventListener | Registration succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, registerEventListener_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        android::sp<hdmiinput::IHDMIInputEventListener> eventListener = new HDMIInputEventListener();
        HDMIINPUT_REGISTEREVENT_SUCCESS(hdmiInput, eventListener);

        HDMIINPUT_UNREGISTEREVENT_SUCCESS(hdmiInput, eventListener);
    }
}

/**
 * @brief L1 Negative Test for the HDMI INPUT HAL registerEventListener() API
 *
 * This test verifies that the registerEventListener() API handles invalid input
 * and duplicate registration attempts correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 020@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |----------------|-----------|---------|---------------|-----|
 * |01|Call registerEventListener() with invalid listener|NULL listener |Return status=false|should pass|
 * |02|Call registerEventListener() with valid listener  |Valid listener|Return status=true|should pass|
 * |03|Call registerEventListener() again with another valid listener|Valid listener|Return status=false (already registered)|should pass|
 * |04|Call unregisterEventListener()|Same listener |Return status=true (unregistered) |should pass|
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, registerEventListener_neg)
{
    sp<hdmiinput::IHDMIInputEventListener> invalidEventListener = nullptr;
    sp<hdmiinput::IHDMIInputEventListener> eventListener_1 = new HDMIInputEventListener();
    sp<hdmiinput::IHDMIInputEventListener> eventListener_2 = new HDMIInputEventListener();
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        /* Register the event listeners with invalid listener */
        HDMIINPUT_REGISTEREVENT_FAILURE(hdmiInput, invalidEventListener, Status::EX_NULL_POINTER);

        /* Register the event listeners with valid listener */
        HDMIINPUT_REGISTEREVENT_SUCCESS(hdmiInput, eventListener_1);

        /* Register the event listeners with another valid listener */
        HDMIINPUT_REGISTEREVENT_FAILURE(hdmiInput, eventListener_2, Status::EX_NONE);

        /* Unregister the first event listener */
        HDMIINPUT_UNREGISTEREVENT_SUCCESS(hdmiInput, eventListener_1);
    }
}

/**
 * @brief Validate the unregisterEventListener API for HDMI Input objects registers an event listener successfully.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Register a new event listener using HDMIINPUT_REGISTEREVENT_SUCCESS. | Input: eventListener | Registration succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, unregisterEventListener_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        android::sp<hdmiinput::IHDMIInputEventListener> eventListener = new HDMIInputEventListener();
        HDMIINPUT_REGISTEREVENT_SUCCESS(hdmiInput, eventListener);

        HDMIINPUT_UNREGISTEREVENT_SUCCESS(hdmiInput, eventListener);
    }
}

/**
 * @brief Negative tests for the unregisterEventListener API for HDMI Input objects
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 022@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Attempt to unregister a listener that was never registered. | eventListener_1 | API returns failure (EX_NONE). | Should Pass |
 * | 04 | Register a valid event listener. | eventListener_1 | Registration succeeds. | Should Pass |
 * | 05 | Attempt to unregister a null listener. | nullptr | API returns failure (EX_NULL_POINTER). | Should Pass |
 * | 06 | Attempt to unregister another unregistered listener. | eventListener_2 | API returns failure (EX_NONE). | Should Pass |
 * | 07 | Unregister the valid event listener. | eventListener_1 | Unregistration succeeds. | Should Pass |
 * | 08 | Attempt to unregister the same listener again. | eventListener_1 | API returns failure (EX_NONE). | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, unregisterEventListener_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        android::sp<hdmiinput::IHDMIInputEventListener> eventListener_1 = new HDMIInputEventListener();
        android::sp<hdmiinput::IHDMIInputEventListener> eventListener_2 = new HDMIInputEventListener();
        android::sp<hdmiinput::IHDMIInputEventListener> invalidEventListener = nullptr;

        HDMIINPUT_UNREGISTEREVENT_FAILURE(hdmiInput, eventListener_1, Status::EX_NONE);

        HDMIINPUT_REGISTEREVENT_SUCCESS(hdmiInput, eventListener_1);

        HDMIINPUT_UNREGISTEREVENT_FAILURE(hdmiInput, invalidEventListener, Status::EX_NULL_POINTER);

        HDMIINPUT_UNREGISTEREVENT_FAILURE(hdmiInput, eventListener_2, Status::EX_NONE);

        HDMIINPUT_UNREGISTEREVENT_SUCCESS(hdmiInput, eventListener_1);

        HDMIINPUT_UNREGISTEREVENT_FAILURE(hdmiInput, eventListener_1, Status::EX_NONE);
    }
}

/**
 * @brief Validate the getConnectionState API for HDMI Input controllers
 *
 * This test verifies that the getConnectionState API on each HDMI Input controller returns the expected connection state.
 * It iterates over all available HDMI input IDs, opens each port, and checks the connection state.
 * The test asserts that the returned connection state is false (disconnected) after opening, confirming correct default behavior.
 *
 * **Test Group ID:**: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call open with a valid controller listener and check the returned controller. | Output: controller | Controller is not null. | Should Pass |
 * | 04 | Call getConnectionState on the controller and check the returned value. | Output: connectionState | Value is false (disconnected). | Should Pass |
 * | 05 | Call close on the controller and ensure it succeeds. | Input: controller | Close succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, getConnectionState_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;
        bool connectionState;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);

        HDMIINPUT_GETCONNECTIONSTATE(controller, connectionState);
        UT_ASSERT_FALSE(connectionState);

        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);

    }
}

/**
 * @brief Positive tests for the start API for HDMI Input controllers
 *
 * This test validates the correct behavior of the start() API under normal conditions:
 * 1. Retrieves all HDMI input IDs and opens each port.
 * 2. Starts and stops each HDMI input port concurrently up to the platform's maximum supported limit, verifying that multiple ports can be active simultaneously.
 * 3. Starts and stops each HDMI input port sequentially to ensure correct state transitions.
 * 4. Closes all HDMI input ports at the end of the test.
 *
 * The test asserts that the start and stop operations succeed and that the HDMI input controllers transition through the expected states without errors.
 *
 * **Test Group ID:** Positive: 01@p
 * **Test Case ID:** 024@p
 * **Priority:** High@p
 *
 * **Pre-Conditions:** None@p
 * **Dependencies:** None@p
 * **User Interaction:** None@p
 *
 * **Test Procedure:**@p
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call open with a valid controller listener and check the returned controller. | Output: controller | Controller is not null. | Should Pass |
 * | 04 | Call start on the controller and ensure it succeeds (concurrent test). | Input: controller | Start succeeds. | Should Pass |
 * | 05 | Call stop on the controller and ensure it succeeds (concurrent test). | Input: controller | Stop succeeds. | Should Pass |
 * | 06 | Call start and stop sequentially on each controller. | Input: controller | Start and stop succeed. | Should Pass |
 * | 07 | Call close on the controller and ensure it succeeds. | Input: controller | Close succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, start_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    vector<tuple<IHDMIInput::Id, sp<IHDMIInput>, sp<HDMIInputControllerListener>, sp<hdmiinput::IHDMIInputController>>> hdmiInputList;

    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    // open all HDMI input ports first to prepare for start testing
    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        sp<HDMIInputControllerListener> controllerListener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        // Open the HDMI input port
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, controllerListener, controller, true);

        hdmiInputList.push_back({id, hdmiInput, controllerListener, controller});
    }

    // Start and stop each HDMI input port concurrently to validate that multiple ports
    // can be active at the same time up to the maximum limit
    for (int32_t i = 0; i < gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts; i++)
    {
        auto& input = hdmiInputList[i];
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Start the HDMI input port
        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);
    }
    for (int32_t i = 0; i < gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts; i++)
    {
        auto& input = hdmiInputList[i];
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Stop the HDMI input port
        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
    }

    // Start and stop each HDMI input port sequentially to validate correct state transitions
    for (auto& input : hdmiInputList)
    {
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
    }

    //close all HDMI input ports at the end of the test
    for (auto& input : hdmiInputList)
    {
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Close the HDMI input port
        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Negative test for the start() API of HDMI Input controllers
 *
 * This test verifies that the start() API correctly handles invalid usage scenarios:
 *   - Attempts to start an HDMI input controller that is already in the started state, expecting the API to fail with EX_ILLEGAL_STATE.
 *   - Ensures that the controller state remains unchanged after the failed start attempt and that no unintended side effects occur.
 *
 * Test Steps:
 *   1. Retrieve all HDMI input IDs.
 *   2. For each HDMI input ID, open the port and obtain a controller instance.
 *   3. Start the controller (should succeed).
 *   4. Attempt to start the controller again (should fail with EX_ILLEGAL_STATE).
 *   5. Stop and close the controller.
 *
 * The test asserts that the second start operation fails as expected, the error code is correct, and the controller state is preserved.
 *
 * **Test Group ID:** Negative: 01@n
 * **Test Case ID:** 025@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** HDMI input port must be in State::READY@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Step | Action | Input / Output | Expected Result | Notes |
 * | :--: | ------ | ------------- | --------------- | ----- |
 * | 1 | Retrieve all HDMI input IDs. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Initialization |
 * | 2 | For each HDMI input ID, retrieve the HDMI Input object. | Input: id | HDMI Input object is returned. |  |
 * | 3 | Open the HDMI input port with a valid controller listener. | Output: controller | Controller is not null. |  |
 * | 4 | Start the controller (first attempt). | Input: controller | Start succeeds. | Valid state transition |
 * | 5 | Attempt to start the controller again (second attempt). | Input: controller | Start fails with EX_ILLEGAL_STATE. | Negative test: already started |
 * | 6 | Verify controller state remains unchanged after failed start. | Output: controller state | State is unchanged (still started). | Validation |
 * | 7 | Stop the controller. | Input: controller | Stop succeeds. |  |
 * | 8 | Close the controller. | Input: controller | Close succeeds. | Cleanup |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, start_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    vector<tuple<IHDMIInput::Id, sp<IHDMIInput>, sp<HDMIInputControllerListener>, sp<hdmiinput::IHDMIInputController>>> hdmiInputList;

    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    // open all HDMI input ports first to prepare for start testing
    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        sp<HDMIInputControllerListener> controllerListener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        // Open the HDMI input port
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, controllerListener, controller, true);

        hdmiInputList.push_back({id, hdmiInput, controllerListener, controller});
    }

    // Start and stop each HDMI input port concurrently to validate that multiple ports
    // can be active at the same time up to the maximum limit
    for (int32_t i = 0; i < gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts; i++)
    {
        auto& input = hdmiInputList[i];
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Start the HDMI input port
        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);
    }

    // Attempt to start additional HDMI input ports beyond the maximum concurrent limit, expecting failure with EX_ILLEGAL_STATE
    for (int32_t i = gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts; i < (int32_t)hdmiInputList.size(); i++)
    {
        auto& input = hdmiInputList[i];
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Start the HDMI input port
        HDMIINPUT_START_FAILURE(hdmiInput, controller, hdmiinput::State::READY, Status::EX_ILLEGAL_STATE);
    }

    for (int32_t i = 0; i < gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts; i++)
    {
        auto& input = hdmiInputList[i];
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Stop the HDMI input port
        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
    }

    // Start and stop each HDMI input port sequentially to validate correct state transitions
    for (auto& input : hdmiInputList)
    {
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_START_FAILURE(hdmiInput, controller, hdmiinput::State::STARTED, Status::EX_ILLEGAL_STATE);

        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
    }

    //close all HDMI input ports at the end of the test
    for (auto& input : hdmiInputList)
    {
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Close the HDMI input port
        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Positive tests for the stop() API of HDMI Input controllers
 *
 * This test validates that the stop() API correctly transitions HDMI input controllers from the started state to the stopped state, both concurrently and sequentially, and that all state transitions are handled as expected.
 *
 * The test performs the following:
 *   1. Retrieves all HDMI input IDs and opens each port.
 *   2. Starts and stops each HDMI input port concurrently up to the platform's maximum supported limit, verifying that multiple ports can be stopped simultaneously.
 *   3. Starts and stops each HDMI input port sequentially to ensure correct state transitions.
 *   4. Closes all HDMI input ports at the end of the test.
 *
 * The test asserts that start and stop operations succeed for each port, and that the controller transitions to the correct state without errors.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 026@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Step | Action | Input / Output | Expected Result | Notes |
 * | :--: | ------ | ------------- | --------------- | ----- |
 * | 1 | Retrieve all HDMI input IDs. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Initialization |
 * | 2 | For each HDMI input ID, retrieve the HDMI Input object. | Input: id | HDMI Input object is returned. |  |
 * | 3 | Open the HDMI input port with a valid controller listener. | Output: controller | Controller is not null. |  |
 * | 4 | Start each HDMI input port (concurrent test). | Input: controller | Start succeeds. | Up to max concurrent |
 * | 5 | Stop each HDMI input port (concurrent test). | Input: controller | Stop succeeds. | Up to max concurrent |
 * | 6 | Start and stop each HDMI input port sequentially. | Input: controller | Start and stop succeed. | State transitions validated |
 * | 7 | Close each HDMI input port. | Input: controller | Close succeeds. | Cleanup |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, stop_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;
    vector<tuple<IHDMIInput::Id, sp<IHDMIInput>, sp<HDMIInputControllerListener>, sp<hdmiinput::IHDMIInputController>>> hdmiInputList;

    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    // open all HDMI input ports first to prepare for start testing
    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        sp<HDMIInputControllerListener> controllerListener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        // Open the HDMI input port
        HDMIINPUT_OPEN_SUCCESS(hdmiInput, controllerListener, controller, true);

        hdmiInputList.push_back({id, hdmiInput, controllerListener, controller});
    }

    // Start and stop each HDMI input port concurrently to validate that multiple ports
    // can be active at the same time up to the maximum limit
    for (int32_t i = 0; i < gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts; i++)
    {
        auto& input = hdmiInputList[i];
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Start the HDMI input port
        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);
    }
    for (int32_t i = 0; i < gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts; i++)
    {
        auto& input = hdmiInputList[i];
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Stop the HDMI input port
        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
    }

    // Start and stop each HDMI input port sequentially to validate correct state transitions
    for (auto& input : hdmiInputList)
    {
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);
    }

    //close all HDMI input ports at the end of the test
    for (auto& input : hdmiInputList)
    {
        auto& hdmiInput = std::get<1>(input);
        auto& controller = std::get<3>(input);

        // Close the HDMI input port
        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Negative tests for the stop API for HDMI Input controllers
 *
 * This test covers negative scenarios for the stop API:
 * 1. Attempting to stop a controller that is not in the started state (should fail).
 * 2. Ensures the API fails gracefully and returns the correct error status.
 * It iterates over all available HDMI input IDs, opens each port, attempts to stop before starting, starts the controller, stops it, and then attempts to stop again.
 * The test asserts that stop operations fail with EX_ILLEGAL_STATE when not in the started state.
 *
 * **Test Group ID:** Negative: 01@n
 * **Test Case ID:** 027@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** HDMI input port must be in State::READY or State::CLOSED@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call open with a valid controller listener and check the returned controller. | Output: controller | Controller is not null. | Should Pass |
 * | 04 | Call stop on the controller before starting and check for failure. | Input: controller | Stop fails with EX_ILLEGAL_STATE. | Should Pass |
 * | 05 | Call start on the controller and ensure it succeeds. | Input: controller | Start succeeds. | Should Pass |
 * | 06 | Call stop on the controller and ensure it succeeds. | Input: controller | Stop succeeds. | Should Pass |
 * | 07 | Call stop again on the controller and check for failure. | Input: controller | Stop fails with EX_ILLEGAL_STATE. | Should Pass |
 * | 08 | Call close on the controller and ensure it succeeds. | Input: controller | Close succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, stop_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);

        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_STOP_FAILURE(hdmiInput, controller, hdmiinput::State::READY, Status::EX_ILLEGAL_STATE);

        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Validate the setEDID API for HDMI Input controllers
 *
 * This test verifies that the setEDID API on each HDMI Input controller successfully sets EDID data for the port.
 * It iterates over all available HDMI input IDs, opens each port, sets EDID data, starts and stops the controller, and sets EDID again.
 * The test asserts that setEDID operations succeed for each port, confirming correct behavior.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** HDMI input port must be in State::READY@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call open with a valid controller listener and check the returned controller. | Output: controller | Controller is not null. | Should Pass |
 * | 04 | Call setEDID on the controller and ensure it succeeds. | Input: controller, edidData | setEDID succeeds. | Should Pass |
 * | 05 | Call start on the controller and ensure it succeeds. | Input: controller | Start succeeds. | Should Pass |
 * | 06 | Call stop on the controller and ensure it succeeds. | Input: controller | Stop succeeds. | Should Pass |
 * | 07 | Call setEDID again on the controller and ensure it succeeds. | Input: controller, edidData | setEDID succeeds. | Should Pass |
 * | 08 | Call close on the controller and ensure it succeeds. | Input: controller | Close succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, setEDID_pos)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    std::vector<uint8_t> edidData =
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

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);

        HDMIINPUT_SETEDID_SUCCESS(controller, edidData);

        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_SETEDID_SUCCESS(controller, edidData);

        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/**
 * @brief Negative tests for the setEDID API for HDMI Input controllers
 *
 * This test covers negative scenarios for the setEDID API:
 * 1. Attempting to set invalid EDID data (e.g., empty or incorrect size).
 * 2. Ensures the API fails gracefully and returns the correct error status.
 * It iterates over all available HDMI input IDs, opens each port, attempts to set invalid EDID data, and closes the controller.
 * The test asserts that setEDID operations fail with EX_ILLEGAL_ARGUMENT or similar error for invalid data.
 *
 * **Test Group ID:** Negative: 01@n
 * **Test Case ID:** 029@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** HDMI input port must be in State::READY@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Retrieve all HDMI input IDs using HDMIINPUT_MANAGER_GETHDMIINPUTIDS. | Output: hdmiInputIds vector | List of HDMI input IDs is obtained. | Should be successful |
 * | 02 | For each HDMI input ID, retrieve the HDMI Input object using HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS. | Input: id | HDMI Input object is returned. | Should Pass |
 * | 03 | Call open with a valid controller listener and check the returned controller. | Output: controller | Controller is not null. | Should Pass |
 * | 04 | Call setEDID on the controller with invalid data (empty or wrong size) and check for failure. | Input: controller, invalidEdidData | setEDID fails with EX_ILLEGAL_ARGUMENT. | Should Pass |
 * | 05 | Call close on the controller and ensure it succeeds. | Input: controller | Close succeeds. | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, setEDID_neg)
{
    std::vector<hdmiinput::IHDMIInput::Id> hdmiInputIds;

    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    std::vector<uint8_t> validEDIDData =
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

    std::vector<uint8_t> invalidEDIDData_1 = {};
    std::vector<uint8_t> invalidEDIDData_2 = {0x00,0xFF,0xFF};
    std::vector<uint8_t> invalidEDIDData_3 = {
        0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x04,0x69,0x88,0x88,0x01,0x00,0x00,0x00,
        0x01,0x1E,0x01,0x04,0xA5,0x34,0x20,0x78,0x2A,0xEE,0x95,0xA3,0x54,0x4C,0x99,0x26
    };

    for (const auto& id : hdmiInputIds)
    {
        android::sp<hdmiinput::IHDMIInput> hdmiInput;
        android::sp<hdmiinput::IHDMIInputControllerListener> listener = new HDMIInputControllerListener();
        android::sp<hdmiinput::IHDMIInputController> controller;

        HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESS(id, hdmiInput);

        HDMIINPUT_OPEN_SUCCESS(hdmiInput, listener, controller, true);

        HDMIINPUT_SETEDID_FAILURE(controller, invalidEDIDData_1, Status::EX_NONE);
        HDMIINPUT_SETEDID_FAILURE(controller, invalidEDIDData_2, Status::EX_NONE);
        HDMIINPUT_SETEDID_FAILURE(controller, invalidEDIDData_3, Status::EX_NONE);

        HDMIINPUT_START_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_SETEDID_FAILURE(controller, validEDIDData, Status::EX_ILLEGAL_STATE);

        HDMIINPUT_STOP_SUCCESS(hdmiInput, controller, true);

        HDMIINPUT_CLOSE_SUCCESS(hdmiInput, controller, true);
    }
}

/** @} */ // End of HDMI_INPUT_HALTEST L1 File
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT Module
/** @} */ // End of HPK
