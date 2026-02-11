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
* vts_hdmiinput_parse_hfp.cpp
*
* Parse hfp file
*/

#include "vts_hdmiinput_parse_hfp.h"
#include "vts_hdmiinput_helper_functions.h"

hdmiinputHFP_t gHdmiInput_hfp;

UT_status_t vts_hdmiinput_parse_hfp()
{
    char key[2 * UT_KVP_MAX_ELEMENT_SIZE];
    char subkey[2 *UT_KVP_MAX_ELEMENT_SIZE];
    char subValue[UT_KVP_MAX_ELEMENT_SIZE];

    const std::string baseKey = "hdmiinput";
    const std::string platformCapabilitiesKey = baseKey + ".platformCapabilities";
    const std::string portsKey = baseKey + ".ports";

    // Parse ports
    int numPorts = UT_KVP_PROFILE_GET_LIST_COUNT(portsKey.c_str());
    gHdmiInput_hfp.ports.clear();

    for (int i = 0; i < numPorts; ++i)
    {
        hdmiinputHFP_t::PortConfig port;

        // id
        const std::string portBaseKey = portsKey + "." + std::to_string(i);
        snprintf(key, sizeof(key), "%s.id", portBaseKey.c_str());
        port.id = UT_KVP_PROFILE_GET_UINT32(key);

        // supportedVersions
        snprintf(key, sizeof(key), "%s.supportedVersions", portBaseKey.c_str());
        int numSupportedVersions = UT_KVP_PROFILE_GET_LIST_COUNT(key);
        port.supportedVersions.clear();
        for (int j = 0; j < numSupportedVersions; ++j)
        {
            snprintf(subkey, sizeof(subkey), "%s.supportedVersions.%d", portBaseKey.c_str(), j);
            UT_KVP_PROFILE_GET_STRING(subkey, subValue);
            HDMIVersion version = (HDMIVersion)UT_Control_GetMapValue(hdmiVersion_mapTable, subValue, (int32_t)HDMIVersion::HDMI_1_3);
            port.supportedVersions.push_back(version);

            // Read EDID bytes for this version
            snprintf(key, sizeof(key), "%s.edidBytes.%s", portBaseKey.c_str(), subValue);
            int32_t edid_size = UT_KVP_PROFILE_GET_LIST_COUNT(key);
            std::vector<uint8_t> edidBytes;
            for(int32_t edidIndex = 0; edidIndex < edid_size; edidIndex++)
            {
                snprintf(subkey, sizeof(subkey), "%s.edidBytes.%s.%d", portBaseKey.c_str(), subValue, edidIndex);
                uint8_t edidData = UT_KVP_PROFILE_GET_UINT8(subkey);
                edidBytes.push_back(edidData);
            }
            port.defaultEDID[version] = edidBytes;
        }

        // supportedVICs
        snprintf(key, sizeof(key), "%s.supportedVICs", portBaseKey.c_str());
        int numSupportedVICs = UT_KVP_PROFILE_GET_LIST_COUNT(key);
        port.supportedVICs.clear();
        for (int j = 0; j < numSupportedVICs; ++j)
        {
            snprintf(subkey, sizeof(subkey), "%s.supportedVICs.%d", portBaseKey.c_str(), j);
            UT_KVP_PROFILE_GET_STRING(subkey, subValue);
            port.supportedVICs.push_back((VIC)UT_Control_GetMapValue(vic_mapTable, subValue, (int32_t)VIC::VIC0_UNAVAILABLE));
        }

        // supportedHDCPProtocolVersions
        snprintf(key, sizeof(key), "%s.supportedHDCPProtocolVersions", portBaseKey.c_str());
        int numSupportedHDCPVersions = UT_KVP_PROFILE_GET_LIST_COUNT(key);
        port.supportedHDCPProtocolVersions.clear();
        for (int j = 0; j < numSupportedHDCPVersions; ++j)
        {
            snprintf(subkey, sizeof(subkey), "%s.supportedHDCPProtocolVersions.%d", portBaseKey.c_str(), j);
            UT_KVP_PROFILE_GET_STRING(subkey, subValue);
            port.supportedHDCPProtocolVersions.push_back((HDCPProtocolVersion)UT_Control_GetMapValue(hdcpProtocolVersion_mapTable, subValue, (int32_t)HDCPProtocolVersion::UNDEFINED));
        }

        // Booleans
        snprintf(key, sizeof(key), "%s.supports3D", portBaseKey.c_str());
        port.supports3D = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.supportsFRL", portBaseKey.c_str());
        port.supportsFRL = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.supportsVRR", portBaseKey.c_str());
        port.supportsVRR = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.supportsFreeSync", portBaseKey.c_str());
        port.supportsFreeSync = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.supportsQMS", portBaseKey.c_str());
        port.supportsQMS = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.supportsALLM", portBaseKey.c_str());
        port.supportsALLM = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.supportsQFT", portBaseKey.c_str());
        port.supportsQFT = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.supportsARC", portBaseKey.c_str());
        port.supportsARC = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.supportsEARC", portBaseKey.c_str());
        port.supportsEARC = UT_KVP_PROFILE_GET_BOOL(key);
        snprintf(key, sizeof(key), "%s.edidBytes", portBaseKey.c_str());

        gHdmiInput_hfp.ports.push_back(port);
    }

    // platformCapabilities
    gHdmiInput_hfp.platformCapabilities.maximumConcurrentStartedPorts = UT_KVP_PROFILE_GET_UINT32((platformCapabilitiesKey + ".maximumConcurrentStartedPorts").c_str());
    UT_KVP_PROFILE_GET_STRING((platformCapabilitiesKey + ".freeSync").c_str(), subValue);
    gHdmiInput_hfp.platformCapabilities.freeSync = (FreeSync)UT_Control_GetMapValue(freeSync_mapTable, subValue, (int32_t)FreeSync::UNSUPPORTED);

    return UT_STATUS_OK;
}