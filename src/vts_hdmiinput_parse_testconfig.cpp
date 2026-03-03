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

UT_status_t vts_hdmiinput_parse_connectionTest(char *configurationFile, const char *module, const char *testGroup,
                                               const char *testName, std::vector<hdmiinputConnectionConfig_t>& connectionStateConfigs)
{
    char key[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subkey[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subValue[UT_KVP_MAX_ELEMENT_SIZE];

    void *testconfig = hdmiinputkvp_create_instance(configurationFile);
    ut_kvp_instance_t* kvpTestInstance = (ut_kvp_instance_t*)testconfig;

    if (!kvpTestInstance)
        return UT_STATUS_FAILURE;

    const std::string testListString = string(module) + "." + string(testGroup) + "." + string(testName);
    char *testList = const_cast<char*>(testListString.c_str());

    // ports array
    snprintf(key, sizeof(key), "%s.ports", testList);
    int numPorts = ut_kvp_getListCount(kvpTestInstance, key);
    for (int i = 0; i < numPorts; ++i)
    {
        hdmiinputConnectionConfig_t config;
        // id
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.id", testList, i);
        config.portId = ut_kvp_getUInt32Field(kvpTestInstance, subkey);

        // source_HDCP_Version
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.source_HDCP_Version", testList, i);
        ut_kvp_getStringField(kvpTestInstance, subkey, subValue, sizeof(subValue));
        config.sourceHdcpProtocolVersion = (hdmiinput::HDCPProtocolVersion)UT_Control_GetMapValue(hdcpProtocolVersion_mapTable, subValue, (int32_t)hdmiinput::HDCPProtocolVersion::UNDEFINED);

        // connection_state_before_open
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.connection_state_before_open", testList, i);
        config.connectionStateBeforeOpen = ut_kvp_getBoolField(kvpTestInstance, subkey);

        // signal_state_before_open
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.signal_state_before_open", testList, i);
        ut_kvp_getStringField(kvpTestInstance, subkey, subValue, sizeof(subValue));
        config.signalStateBeforeOpen = (hdmiinput::SignalState)UT_Control_GetMapValue(signalState_mapTable, subValue, (int32_t)hdmiinput::SignalState::UNKNOWN);

        // connection_state (array)
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.connection_state", testList, i);
        int numStates = ut_kvp_getListCount(kvpTestInstance, subkey);
        config.connectionStates.clear();
        for (int j = 0; j < numStates; ++j)
        {
            char elemkey[2 * UT_KVP_MAX_ELEMENT_SIZE];
            snprintf(elemkey, sizeof(elemkey), "%s.ports.%d.connection_state.%d", testList, i, j);
            config.connectionStates.push_back(ut_kvp_getBoolField(kvpTestInstance, elemkey));
        }

        // signal_state (array)
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.signal_state", testList, i);
        int numSignalStates = ut_kvp_getListCount(kvpTestInstance, subkey);
        config.signalStates.clear();
        for (int j = 0; j < numSignalStates; ++j)
        {
            char elemkey[2 * UT_KVP_MAX_ELEMENT_SIZE];
            snprintf(elemkey, sizeof(elemkey), "%s.ports.%d.signal_state.%d", testList, i, j);
            ut_kvp_getStringField(kvpTestInstance, elemkey, subValue, sizeof(subValue));
            config.signalStates.push_back((hdmiinput::SignalState)UT_Control_GetMapValue(signalState_mapTable, subValue, (int32_t)hdmiinput::SignalState::UNKNOWN));
        }

        // max_wait_time_ms
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.max_wait_time_ms", testList, i);
        config.maxWaitTimeMs = ut_kvp_getUInt32Field(kvpTestInstance, subkey);

        connectionStateConfigs.push_back(config);
    }
    hdmiinputkvp_destroy_instance(kvpTestInstance);
    return UT_STATUS_OK;
}

UT_status_t  vts_hdmiinput_parse_infoFramesTestConfig(char *configurationFile, const char *module, const char *testGroup,
                                                      const char *testName, std::vector<hdmiinputInfoFramesTestConfig_t>& infoFramesTestConfig)
{
    char key[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subkey[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subValue[UT_KVP_MAX_ELEMENT_SIZE];

    void *testconfig = hdmiinputkvp_create_instance(configurationFile);
    ut_kvp_instance_t* kvpTestInstance = (ut_kvp_instance_t*)testconfig;

    if (!kvpTestInstance)
        return UT_STATUS_FAILURE;

    const std::string testListString = string(module) + "." + string(testGroup) + "." + string(testName);
    char *testList = const_cast<char*>(testListString.c_str());

    // ports array
    snprintf(key, sizeof(key), "%s.ports", testList);
    int numPorts = ut_kvp_getListCount(kvpTestInstance, key);

    for (int i = 0; i < numPorts; ++i)
    {
        hdmiinputInfoFramesTestConfig_t config;
        // id
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.id", testList, i);
        config.portId = ut_kvp_getUInt32Field(kvpTestInstance, subkey);

        // info_frame_type (array of strings)
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.info_frame_type", testList, i);
        int numTypes = ut_kvp_getListCount(kvpTestInstance, subkey);
        config.infoFrameType.clear();

        for (int j = 0; j < numTypes; ++j)
        {
            char elemkey[2 * UT_KVP_MAX_ELEMENT_SIZE];
            snprintf(elemkey, sizeof(elemkey), "%s.ports.%d.info_frame_type.%d", testList, i, j);
            ut_kvp_getStringField(kvpTestInstance, elemkey, subValue, sizeof(subValue));
            config.infoFrameType.push_back(std::string(subValue));

            if(std::string(subValue) == "VIC")
            {
                // Read VIC value for this info frame
                char vicKey[2 * UT_KVP_MAX_ELEMENT_SIZE];
                snprintf(vicKey, sizeof(vicKey), "%s.ports.%d.frame_data.%d.0", testList, i, j);
                ut_kvp_getStringField(kvpTestInstance, vicKey, subValue, sizeof(subValue));
                config.vic = (hdmiinput::VIC)UT_Control_GetMapValue(vic_mapTable, subValue, (int32_t)hdmiinput::VIC::VIC0_UNAVAILABLE);
            }
            else if(std::string(subValue) == "VRR")
            {
                // Read VRR parameters for this info frame
                char vrrKey[2 * UT_KVP_MAX_ELEMENT_SIZE];

                // vrrActive
                snprintf(vrrKey, sizeof(vrrKey), "%s.ports.%d.frame_data.%d.0", testList, i, j);
                config.vrrActive = ut_kvp_getBoolField(kvpTestInstance, vrrKey);

                // mConst
                snprintf(vrrKey, sizeof(vrrKey), "%s.ports.%d.frame_data.%d.1", testList, i, j);
                config.mConst = ut_kvp_getBoolField(kvpTestInstance, vrrKey);

                // fastVActive
                snprintf(vrrKey, sizeof(vrrKey), "%s.ports.%d.frame_data.%d.2", testList, i, j);
                config.fastVActive = ut_kvp_getBoolField(kvpTestInstance, vrrKey);

                // frameRate
                snprintf(vrrKey, sizeof(vrrKey), "%s.ports.%d.frame_data.%d.3", testList, i, j);
                config.frameRate = ut_kvp_getDoubleField(kvpTestInstance, vrrKey);
            }
            else if(std::string(subValue) == "AUDIO")
            {
                // frame_data (array of arrays)
                snprintf(subkey, sizeof(subkey), "%s.ports.%d.frame_data.%d", testList, i, j);
                int32_t numBytes = ut_kvp_getListCount(kvpTestInstance, subkey);

                for (int k = 0; k < numBytes; ++k)
                {
                    char bytekey[2 * UT_KVP_MAX_ELEMENT_SIZE];
                    snprintf(bytekey, sizeof(bytekey), "%s.ports.%d.frame_data.%d.%d", testList, i, j, k);
                    uint32_t byteValue = ut_kvp_getUInt8Field(kvpTestInstance, bytekey);
                    config.audioInfoData.push_back(static_cast<uint8_t>(byteValue));
                }
            }
            else if(std::string(subValue) == "AVI")
            {
                // frame_data (array of arrays)
                snprintf(subkey, sizeof(subkey), "%s.ports.%d.frame_data.%d", testList, i, j);
                int32_t numBytes = ut_kvp_getListCount(kvpTestInstance, subkey);

                for (int k = 0; k < numBytes; ++k)
                {
                    char bytekey[2 * UT_KVP_MAX_ELEMENT_SIZE];
                    snprintf(bytekey, sizeof(bytekey), "%s.ports.%d.frame_data.%d.%d", testList, i, j, k);
                    uint32_t byteValue = ut_kvp_getUInt8Field(kvpTestInstance, bytekey);
                    config.aviInfoData.push_back(static_cast<uint8_t>(byteValue));
                }
            }
            else if(std::string(subValue) == "VSIF")
            {
                // frame_data (array of arrays)
                snprintf(subkey, sizeof(subkey), "%s.ports.%d.frame_data.%d", testList, i, j);
                int32_t numBytes = ut_kvp_getListCount(kvpTestInstance, subkey);

                for (int k = 0; k < numBytes; ++k)
                {
                    char bytekey[2 * UT_KVP_MAX_ELEMENT_SIZE];
                    snprintf(bytekey, sizeof(bytekey), "%s.ports.%d.frame_data.%d.%d", testList, i, j, k);
                    uint32_t byteValue = ut_kvp_getUInt8Field(kvpTestInstance, bytekey);
                    config.vsiInfoData.push_back(static_cast<uint8_t>(byteValue));
                }
            }
            else if(std::string(subValue) == "SPD")
            {
                // frame_data (array of arrays)
                snprintf(subkey, sizeof(subkey), "%s.ports.%d.frame_data.%d", testList, i, j);
                int32_t numBytes = ut_kvp_getListCount(kvpTestInstance, subkey);

                for (int k = 0; k < numBytes; ++k)
                {
                    char bytekey[2 * UT_KVP_MAX_ELEMENT_SIZE];
                    snprintf(bytekey, sizeof(bytekey), "%s.ports.%d.frame_data.%d.%d", testList, i, j, k);
                    uint32_t byteValue = ut_kvp_getUInt8Field(kvpTestInstance, bytekey);
                    config.spdInfoData.push_back(static_cast<uint8_t>(byteValue));
                }
            }
        }

        // max_wait_time_ms
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.max_wait_time_ms", testList, i);
        config.maxWaitTimeMs = ut_kvp_getUInt32Field(kvpTestInstance, subkey);

        infoFramesTestConfig.push_back(config);
    }

    hdmiinputkvp_destroy_instance(kvpTestInstance);

    return UT_STATUS_OK;
}


UT_status_t vts_hdmiinput_parse_edidTestConfig(char *configurationFile, const char *module, const char *testGroup,
                                               const char *testName, std::vector<hdmiinputEdidTestConfig_t>& edidTestConfig)
{
    char key[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subkey[2 * UT_KVP_MAX_ELEMENT_SIZE];

    void *testconfig = hdmiinputkvp_create_instance(configurationFile);
    ut_kvp_instance_t* kvpTestInstance = (ut_kvp_instance_t*)testconfig;

    if (!kvpTestInstance)
        return UT_STATUS_FAILURE;

    const std::string testListString = string(module) + "." + string(testGroup) + "." + string(testName);
    char *testList = const_cast<char*>(testListString.c_str());

    // ports array
    snprintf(key, sizeof(key), "%s.ports", testList);
    int numPorts = ut_kvp_getListCount(kvpTestInstance, key);
    for (int i = 0; i < numPorts; ++i)
    {
        hdmiinputEdidTestConfig_t config;
        // id
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.id", testList, i);
        config.portId = ut_kvp_getUInt32Field(kvpTestInstance, subkey);

        // edid (array of bytes)
        snprintf(subkey, sizeof(subkey), "%s.ports.%d.edid", testList, i);
        int numBytes = ut_kvp_getListCount(kvpTestInstance, subkey);
        config.edidData.clear();
        for (int j = 0; j < numBytes; ++j)
        {
            char bytekey[2 * UT_KVP_MAX_ELEMENT_SIZE];
            snprintf(bytekey, sizeof(bytekey), "%s.ports.%d.edid.%d", testList, i, j);
            uint32_t byteValue = ut_kvp_getUInt32Field(kvpTestInstance, bytekey);
            config.edidData.push_back(static_cast<uint8_t>(byteValue));
        }

        edidTestConfig.push_back(config);
    }
    hdmiinputkvp_destroy_instance(kvpTestInstance);
    return UT_STATUS_OK;
}