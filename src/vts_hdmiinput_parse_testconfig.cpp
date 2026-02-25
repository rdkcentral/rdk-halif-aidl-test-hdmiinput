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

/** @brief
* vts_hdmiinput_parse_testconfig.cpp
*
* Parse test configuration file
*/

#include "vts_hdmiinput_helper_functions.h"
#include "vts_hdmiinput_parse_testconfig.h"

static void* hdmiinputkvp_create_instance(char *fileName)
{
    ut_kvp_instance_t* instance;
    ut_kvp_status_t result;

    instance = ut_kvp_createInstance();

    if(!instance)
    {
        UT_LOG_ERROR("[HDMIINPUT]Failed to create ut kvp instance");
        return nullptr;
    }

    result = ut_kvp_open(instance, fileName);

    if(result != UT_KVP_STATUS_SUCCESS)
    {
        UT_LOG_ERROR("[HDMIINPUT]Failed to open test configuration file");
        ut_kvp_destroyInstance(instance);
        return nullptr;
    }

    return (ut_kvp_instance_t*)instance;
}

static void hdmiinputkvp_destroy_instance(void *instance)
{
    ut_kvp_destroyInstance((ut_kvp_instance_t*)instance);
}

static void vts_hdmiinput_parse_connectionTestOpen(ut_kvp_instance_t* kvpTestInstance, const char* testList,
                                                   hdmiinputConnectionStateOpenConfig_t* connectionStateConfig)
{
    char key[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subkey[2 *UT_KVP_MAX_ELEMENT_SIZE];
    char subValue[UT_KVP_MAX_ELEMENT_SIZE];

    // portId
    snprintf(key, sizeof(key), "%s.portId", testList);
    connectionStateConfig->portId = ut_kvp_getUInt32Field(kvpTestInstance, key);

    // Connection state before Open
    snprintf(key, sizeof(key), "%s.connection_state_before_open", testList);
    connectionStateConfig->connectionStateBeforeOpen = ut_kvp_getBoolField(kvpTestInstance, key);

    // connectionStates
    snprintf(key, sizeof(key), "%s.connection_state_after_open", testList);
    int numConnectionStates = ut_kvp_getListCount(kvpTestInstance, key);
    connectionStateConfig->connectionStatesAfterOpen.clear();
    for (int i = 0; i < numConnectionStates; ++i)
    {
        snprintf(subkey, sizeof(subkey), "%s.connection_state_after_open.%d", testList, i);
        bool state = ut_kvp_getBoolField(kvpTestInstance, subkey);
        connectionStateConfig->connectionStatesAfterOpen.push_back(state);
    }

    // signalStateBeforeOpen
    snprintf(key, sizeof(key), "%s.signal_state_before_open", testList);
    ut_kvp_getStringField(kvpTestInstance, key, subValue, sizeof(subValue));
    connectionStateConfig->signalStateBeforeOpen = (hdmiinput::SignalState)UT_Control_GetMapValue(signalState_mapTable, subValue, (int32_t)hdmiinput::SignalState::UNKNOWN);

    // signalStates
    snprintf(key, sizeof(key), "%s.signal_state_after_open", testList);
    int numSignalStatesAfterOpen = ut_kvp_getListCount(kvpTestInstance, key);
    connectionStateConfig->signalStatesAfterOpen.clear();
    for (int i = 0; i < numSignalStatesAfterOpen; ++i)
    {
        snprintf(subkey, sizeof(subkey), "%s.signal_state_after_open.%d", testList, i);
        ut_kvp_getStringField(kvpTestInstance, subkey, subValue, sizeof(subValue));
        connectionStateConfig->signalStatesAfterOpen.push_back((hdmiinput::SignalState)UT_Control_GetMapValue(signalState_mapTable, subValue, (int32_t)hdmiinput::SignalState::UNKNOWN));
    }

    // maxWaitTimeMs
    snprintf(key, sizeof(key), "%s.max_wait_time_ms", testList);
    connectionStateConfig->maxWaitTimeMs = ut_kvp_getUInt32Field(kvpTestInstance, key);
}

static void vts_hdmiinput_parse_connectionTestStart(ut_kvp_instance_t* kvpTestInstance, const char* testList,
                                                    hdmiinputConnectionStateStartConfig_t* connectionStateConfig)
{
    char key[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subkey[2 *UT_KVP_MAX_ELEMENT_SIZE];
    char subValue[UT_KVP_MAX_ELEMENT_SIZE];

    // portId
    snprintf(key, sizeof(key), "%s.portId", testList);
    connectionStateConfig->portId = ut_kvp_getUInt32Field(kvpTestInstance, key);

    // Connection state before Open
    snprintf(key, sizeof(key), "%s.connection_state_before_open", testList);
    connectionStateConfig->connectionStateBeforeOpen = ut_kvp_getBoolField(kvpTestInstance, key);

    // connectionStatesAfterStart
    snprintf(key, sizeof(key), "%s.connection_state_after_start", testList);
    int numConnectionStatesAfterStart = ut_kvp_getListCount(kvpTestInstance, key);
    connectionStateConfig->connectionStatesAfterStart.clear();
    for (int i = 0; i < numConnectionStatesAfterStart; ++i)
    {
        snprintf(subkey, sizeof(subkey), "%s.connection_state_after_start.%d", testList, i);
        bool state = ut_kvp_getBoolField(kvpTestInstance, subkey);
        connectionStateConfig->connectionStatesAfterStart.push_back(state);
    }

    // signalStateBeforeOpen
    snprintf(key, sizeof(key), "%s.signal_state_before_open", testList);
    ut_kvp_getStringField(kvpTestInstance, key, subValue, sizeof(subValue));
    connectionStateConfig->signalStateBeforeOpen = (hdmiinput::SignalState)UT_Control_GetMapValue(signalState_mapTable, subValue, (int32_t)hdmiinput::SignalState::UNKNOWN);

    // signalStates
    snprintf(key, sizeof(key), "%s.signal_state_after_start", testList);
    int numSignalStatesAfterStart = ut_kvp_getListCount(kvpTestInstance, key);
    connectionStateConfig->signalStatesAfterStart.clear();
    for (int i = 0; i < numSignalStatesAfterStart; ++i)
    {
        snprintf(subkey, sizeof(subkey), "%s.signal_state_after_start.%d", testList, i);
        ut_kvp_getStringField(kvpTestInstance, subkey, subValue, sizeof(subValue));
        connectionStateConfig->signalStatesAfterStart.push_back((hdmiinput::SignalState)UT_Control_GetMapValue(signalState_mapTable, subValue, (int32_t)hdmiinput::SignalState::UNKNOWN));
    }

    // maxWaitTimeMs
    snprintf(key, sizeof(key), "%s.max_wait_time_ms", testList);
    connectionStateConfig->maxWaitTimeMs = ut_kvp_getUInt32Field(kvpTestInstance, key);
}

UT_status_t vts_hdmiinput_parse_test_config(char *configurationFile, const char *module, const char *testGroup,
                                            const char *testName, void *testConfig)
{
    void *testconfig = hdmiinputkvp_create_instance(configurationFile);
    ut_kvp_instance_t* kvpTestInstance = (ut_kvp_instance_t*)testconfig;

    const std::string testList = string(module) + "." + string(testGroup) + "." + string(testName);

    if(!strcmp(testName, "ConnectionSignalStateTestOpen"))
    {
        vts_hdmiinput_parse_connectionTestOpen(kvpTestInstance, testList.c_str(), (hdmiinputConnectionStateOpenConfig_t*)testConfig);
    }
    else if(!strcmp(testName, "ConnectionSignalStateTestStart"))
    {
        vts_hdmiinput_parse_connectionTestStart(kvpTestInstance, testList.c_str(), (hdmiinputConnectionStateStartConfig_t*)testConfig);
    }
    else
    {
        UT_LOG_ERROR("Unsupported test name: %s", testName);
        hdmiinputkvp_destroy_instance(kvpTestInstance);
        return UT_STATUS_FAILURE;
    }

    hdmiinputkvp_destroy_instance(kvpTestInstance);
    return UT_STATUS_OK;
}