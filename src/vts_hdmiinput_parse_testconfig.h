/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
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

/** @brief
* vts_hdmiinput_parse_testconfig.h
*
* Parse test configuration file
*/

#ifndef __VTS_HDMIINPUT_PARSE_TESTCONFIG_H__
#define __VTS_HDMIINPUT_PARSE_TESTCONFIG_H__

#include <string>
#include <vector>

#include <ut.h>
#include <ut_log.h>
#include "ut_kvp_profile.h"

using namespace std;

#define VTS_HDMIINPUT_TEST_CONFIG_PATH "./test_configuration/vts_l3_HdmiInput.yaml"

typedef struct hdmiinputConnectionStateOpenConfig
{
    int32_t portId;
    bool connectionStateBeforeOpen;
    vector<bool> connectionStatesAfterOpen;

    hdmiinput::SignalState signalStateBeforeOpen;
    std::vector<hdmiinput::SignalState> signalStatesAfterOpen;

    uint32_t maxWaitTimeMs;
} hdmiinputConnectionStateOpenConfig_t;

typedef struct hdmiinputConnectionStateStartConfig
{
    int32_t portId;
    bool connectionStateBeforeOpen;
    vector<bool> connectionStatesAfterStart;

    hdmiinput::SignalState signalStateBeforeOpen;
    std::vector<hdmiinput::SignalState> signalStatesAfterStart;

    uint32_t maxWaitTimeMs;
} hdmiinputConnectionStateStartConfig_t;

UT_status_t vts_hdmiinput_parse_test_config(char *configurationFile, const char *module, const char *testGroup,
                                            const char *testName, void *testConfig);
#endif // __VTS_HDMIINPUT_PARSE_TESTCONFIG_H__