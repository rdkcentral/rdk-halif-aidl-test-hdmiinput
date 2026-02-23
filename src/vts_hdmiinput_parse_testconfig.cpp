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

static void vts_hdmiinput_parse_connectionTest(ut_kvp_instance_t* kvpTestInstance, const char* testList,
                                               hdmiinputConnectionStateConfig_t* connectionStateConfig)
{
    char key[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subkey[2 *UT_KVP_MAX_ELEMENT_SIZE];
    char subValue[UT_KVP_MAX_ELEMENT_SIZE];

    // portId
    snprintf(key, sizeof(key), "%s.portId", testList);
    connectionStateConfig->portId = ut_kvp_getUInt32Field(kvpTestInstance, key);

    // stateBeforeOpen
    snprintf(key, sizeof(key), "%s.connection_state_before_open", testList);
    connectionStateConfig->stateBeforeOpen = ut_kvp_getBoolField(kvpTestInstance, key);

    // connectionStates
    snprintf(key, sizeof(key), "%s.connection_state_after_open", testList);
    int numConnectionStates = ut_kvp_getListCount(kvpTestInstance, key);
    connectionStateConfig->connectionStates.clear();
    for (int i = 0; i < numConnectionStates; ++i)
    {
        snprintf(subkey, sizeof(subkey), "%s.connection_state_after_open.%d", testList, i);
        bool state = ut_kvp_getBoolField(kvpTestInstance, subkey);
        connectionStateConfig->connectionStates.push_back(state);
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

    if(!strcmp(testName, "ConnectionTest"))
    {
        vts_hdmiinput_parse_connectionTest(kvpTestInstance, testList.c_str(), (hdmiinputConnectionStateConfig_t*)testConfig);
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