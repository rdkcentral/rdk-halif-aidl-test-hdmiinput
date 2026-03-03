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

typedef struct hdmiinputConnectionConfig
{
    int32_t portId;
    hdmiinput::HDCPProtocolVersion sourceHdcpProtocolVersion;
    bool connectionStateBeforeOpen;
    hdmiinput::SignalState signalStateBeforeOpen;
    std::vector<bool> connectionStates;
    std::vector<hdmiinput::SignalState> signalStates;
    uint32_t maxWaitTimeMs;
} hdmiinputConnectionConfig_t;

typedef struct hdmiinputInfoFramesTestConfig
{
    int32_t portId;
    std::vector<std::string> infoFrameType;
    std::vector<uint8_t> aviInfoData;
    std::vector<uint8_t> audioInfoData;
    std::vector<uint8_t> vsiInfoData;
    std::vector<uint8_t> spdInfoData;
    hdmiinput::VIC vic;
    bool vrrActive;
    bool mConst;
    bool fastVActive;
    double frameRate;
    uint32_t maxWaitTimeMs;
} hdmiinputInfoFramesTestConfig_t;

typedef struct hdmiinputEdidTestConfig
{
    int32_t portId;
    std::vector<uint8_t> edidData;
} hdmiinputEdidTestConfig_t;

UT_status_t vts_hdmiinput_parse_connectionTest(char *configurationFile, const char *module, const char *testGroup,
                                               const char *testName, std::vector<hdmiinputConnectionConfig_t>& connectionStateConfigs);

UT_status_t vts_hdmiinput_parse_infoFramesTestConfig(char *configurationFile, const char *module, const char *testGroup,
                                                      const char *testName, std::vector<hdmiinputInfoFramesTestConfig_t>& infoFramesTestConfig);

UT_status_t vts_hdmiinput_parse_edidTestConfig(char *configurationFile, const char *module, const char *testGroup,
                                               const char *testName, std::vector<hdmiinputEdidTestConfig_t>& edidTestConfig);

#endif // __VTS_HDMIINPUT_PARSE_TESTCONFIG_H__