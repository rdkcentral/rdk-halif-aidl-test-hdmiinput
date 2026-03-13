/**
 *  If not stated otherwise in this file or this component's LICENSE
 *  file the following copyright and licenses apply:
 *
 *  Copyright 2026 RDK Management
 *
 *  Licensed under the Apache License, Version 2.0 (the License);
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an AS IS BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * @addtogroup HPK Hardware Porting Kit
 * @{
 * @par The Hardware Porting Kit
 * HPK is the next evolution of the well-defined Hardware Abstraction Layer
 * (HAL), but augmented with more comprehensive documentation and test suites
 * that OEM or SOC vendors can use to self-certify their ports before taking
 * them to RDKM for validation or to an operator for final integration and
 * deployment. The Hardware Porting Kit effectively enables an OEM and/or SOC
 * vendor to self-certify their own Video Accelerator devices, with minimal RDKM
 * assistance.
 *
 */

/**
 * @addtogroup HDMI_INPUT HDMI INPUT Module
 * @{
 */

/**
 * @addtogroup HDMI_INPUT_HALTEST HDMI INPUT HAL Tests
 * @{
 */

/**
 * @defgroup HDMI_INPUT_HALTEST_HELPER_HEADER HDMI INPUT HAL Tests helper functions Header File
 * @{
 * @parblock
 *
 * ### Helper functions for VTS tests :
 *
 * Helper functions required for the module across all vendors.
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_hdmiinput_helper_functions.h
 *
 */

#ifndef __VTS_HDMIINPUT_HELPER_H__
#define __VTS_HDMIINPUT_HELPER_H__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/Status.h>
#include <utils/String16.h>

#include "com/rdk/hal/hdmiinput/BnHDMIInputControllerListener.h"
#include "com/rdk/hal/hdmiinput/BnHDMIInputEventListener.h"
#include "com/rdk/hal/hdmiinput/IHDMIInput.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputController.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputControllerListener.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputEventListener.h"

#include "com/rdk/hal/hdmiinput/HDCPProtocolVersion.h"
#include "com/rdk/hal/hdmiinput/VIC.h"
#include "com/rdk/hal/hdmiinput/HDMIVersion.h"
#include "com/rdk/hal/hdmiinput/FreeSync.h"

using android::sp;
using android::String16;
using android::defaultServiceManager;
using android::binder::Status;
using namespace std;
using namespace std::chrono;

using namespace com::rdk::hal;
using namespace com::rdk::hal::hdmiinput;

extern const ut_control_keyStringMapping_t bool_mapTable_hdmiinput[];
extern const ut_control_keyStringMapping_t statusHdmiInputException_mapTable[];
extern const ut_control_keyStringMapping_t hdcpProtocolVersion_mapTable[];
extern const ut_control_keyStringMapping_t vic_mapTable[];
extern const ut_control_keyStringMapping_t hdmiVersion_mapTable[];
extern const ut_control_keyStringMapping_t freeSync_mapTable[];
extern const ut_control_keyStringMapping_t signalState_mapTable[];

std::string hdmiinputVecIntToString(std::vector<int32_t>& addrs);
std::string hdmiinputVecByteToHexString(const std::vector<uint8_t>& data);

#endif //__VTS_HDMIINPUT_HELPER_H__

/** @} */ // End of HDMI_INPUT_HALTEST_HELPER_HEADER
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT
/** @} */ // End of HPK