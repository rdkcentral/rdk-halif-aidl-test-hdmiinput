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
* vts_hdmiinput_parse_hfp.h
*
* Parse hfp file
*/

#ifndef __VTS_HDMIINPUT_PARSE_HFP_H__
#define __VTS_HDMIINPUT_PARSE_HFP_H__

#include <ut.h>
#include <ut_log.h>
#include "ut_kvp_profile.h"
#include <vector>
#include <string>

#include "com/rdk/hal/hdmiinput/HDCPProtocolVersion.h"
#include "com/rdk/hal/hdmiinput/VIC.h"
#include "com/rdk/hal/hdmiinput/HDMIVersion.h"
#include "com/rdk/hal/hdmiinput/FreeSync.h"

using namespace com::rdk::hal::hdmiinput;

typedef struct hdmiinputHFP
{
    struct PortConfig {
        int32_t id;
        std::vector<HDMIVersion> supportedVersions;
        std::vector<VIC> supportedVICs;
        std::vector<HDCPProtocolVersion> supportedHDCPProtocolVersions;
        bool supports3D;
        bool supportsFRL;
        bool supportsVRR;
        bool supportsFreeSync;
        bool supportsQMS;
        bool supportsALLM;
        bool supportsQFT;
        bool supportsARC;
        bool supportsEARC;
        std::map<HDMIVersion, std::vector<uint8_t>> defaultEDID;
    };
    std::vector<PortConfig> ports;
    struct PlatformCapabilities {
        int maximumConcurrentStartedPorts;
        FreeSync freeSync;
    } platformCapabilities;
} hdmiinputHFP_t;

extern hdmiinputHFP_t gHdmiInput_hfp;

UT_status_t vts_hdmiinput_parse_hfp();

#endif // __VTS_HDMIINPUT_PARSE_HFP_H__
