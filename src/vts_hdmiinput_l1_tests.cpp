/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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

extern android::sp<::com::rdk::hal::hdmiinput::IHDMIInputManager> gHdmiInputManager;

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
 * This test calls the getHDMIInputIds API on the HDMIInputManager service and verifies that the returned list of HDMI input IDs is not empty and matches the expected identifiers retrieved from a predefined YAML profile. The test iterates through the IDs and asserts that each ID is either one of the expected valid IDs or is marked as UNDEFINED if extra entries exist.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 059@n
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
 * | 03 | Retrieve expected HDMI input IDs count from the YAML profile and prepare the expectedIds vector based on the profile values. | YAML: hdmiinput.ports entries; expectedIds vector populated with each expected id value | expectedIds vector contains valid id values as per the YAML profile | Should Pass |@n
 * | 04 | Iterate over each id in the hdmiInputIds vector and assert that the id is either present in the expectedIds vector or is UNDEFINED for any extra entries. | Each id from hdmiInputIds compared against expectedIds and UNDEFINED value | Each id is validated successfully; assertions pass if id matches one of the expected values or is UNDEFINED | Should Pass |
 */
UT_ADD_TEST(VTS_L1_HDMIINPUT, HDMIInputIds_pos)
{
    std::vector<::com::rdk::hal::hdmiinput::IHDMIInput::Id> hdmiInputIds;

    /* Get the HDMI INPUT IDs */
    HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiInputIds);

    size_t yamlCount = UT_KVP_PROFILE_GET_LIST_COUNT("hdmiinput.ports");
    std::vector<int32_t> expectedIds;
    for (size_t i = 0; i < yamlCount; i++) {
        int32_t expId = 0;
        switch(i) {
            case 0:
                expId = UT_KVP_PROFILE_GET_UINT32("hdmiinput.ports.0.id");
                break;
            case 1:
                expId = UT_KVP_PROFILE_GET_UINT32("hdmiinput.ports.1.id");
                break;
            default:
                /* For any extra entries not explicitly defined, assume UNDEFINED as a valid edge value */
                expId = ::com::rdk::hal::hdmiinput::IHDMIInput::Id::UNDEFINED;
                break;
        }
        expectedIds.push_back(expId);
    }

    for (const auto &id : hdmiInputIds) {
        bool isExpected = (std::find(expectedIds.begin(), expectedIds.end(), id.value) != expectedIds.end());
        UT_ASSERT_TRUE(isExpected || (id.value == ::com::rdk::hal::hdmiinput::IHDMIInput::Id::UNDEFINED));
    }
}

/** @} */ // End of HDMI_INPUT_HALTEST L1 File
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT Module
/** @} */ // End of HPK
