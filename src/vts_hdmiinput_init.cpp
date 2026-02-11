/**
 *  If not stated otherwise in this file or this component's LICENSE
 *  file the following copyright and licenses apply:
 *
 *  Copyright 2024 RDK Management
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
 * @addtogroup HDMI_INPUT HDMI Input Module
 * @{
 */

/**
 * @addtogroup HDMI_INPUT_HALTEST HDMI Input HAL Tests
 * @{
 */

/**
 * @defgroup HDMI_INPUT_HALTEST_INIT HDMI Input HAL Tests helper functions
 * @{
 * @parblock
 *
 * ### Initialization functions for VTS tests :
 *
 * Initialization functions required for the module across all vendors.
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_hdmiinput_init.cpp
 *
 */

#include <ut.h>
#include <ut_log.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/Status.h>
#include <utils/String16.h>

#include "com/rdk/hal/hdmiinput/IHDMIInputManager.h"
#include "com/rdk/hal/hdmiinput/BpHDMIInputControllerListener.h"
#include "com/rdk/hal/hdmiinput/BpHDMIInputEventListener.h"
#include "com/rdk/hal/hdmiinput/BpHDMIInput.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputController.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputControllerListener.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputEventListener.h"

#include "vts_hdmiinput_parse_hfp.h"
#include "vts_hdmiinput_init.h"

using android::sp;
using android::String16;
using android::defaultServiceManager;
using android::binder::Status;
using namespace std;
using namespace std::chrono;


using namespace com::rdk::hal;
using namespace com::rdk::hal::hdmiinput;

android::sp<::com::rdk::hal::hdmiinput::IHDMIInputManager> gHdmiInputManager;

static UT_status_t vts_hdmiinput_getService()
{
    gHdmiInputManager = android::interface_cast<IHDMIInputManager>(
            defaultServiceManager()->getService(String16(IHDMIInputManager::serviceName().c_str())));
    if (gHdmiInputManager == nullptr)
    {
        UT_LOG_ERROR("AudioDecoder Service not available");
        return UT_STATUS_FAILURE;
    }
    return UT_STATUS_OK;
}

UT_status_t VTS_HDMIINPUT_init()
{
    if(vts_hdmiinput_getService() != UT_STATUS_OK)
    {
        UT_LOG_ERROR("Failed to get HDMIInput Service");
        return UT_STATUS_FAILURE;
    }
    if(vts_hdmiinput_parse_hfp() != UT_STATUS_OK)
    {
        UT_LOG_ERROR("Failed to process the HDMIInput HFP file");
    }

    return UT_STATUS_OK;
}

void VTS_HDMIINPUT_destroy()
{
}

/** @} */ // End of HDMI_INPUT_HALTEST_INIT
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT
/** @} */ // End of HPK