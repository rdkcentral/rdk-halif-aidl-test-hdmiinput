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
 * @addtogroup HDMI_INPUT HDMI INPUT Module
 * @{
 */

/**
 * @addtogroup HDMI_INPUT_HALTEST HDMI INPUT HAL Tests
 * @{
 */

/**
 * @defgroup HDMI_INPUT_HALTEST_HEADER HDMI INPUT HAL Tests helper functions Header File
 * @{
 * @parblock
 *
 * ### Common functions/macros required for the hdmi input :
 *
 * Common functions/macros required for the hdmi cec .
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_hdmiinput.h
 *
 */

#ifndef __VTS_HDMIINPUT_H__
#define __VTS_HDMIINPUT_H__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>
#include <chrono>
#include <thread>

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/Status.h>
#include <utils/String16.h>

#include "com/rdk/hal/hdmiinput/BpHDMIInputControllerListener.h"
#include "com/rdk/hal/hdmiinput/BnHDMIInputEventListener.h"
#include "com/rdk/hal/hdmiinput/IHDMIInput.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputController.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputControllerListener.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputEventListener.h"
#include "com/rdk/hal/hdmiinput/IHDMIInputManager.h"

#include "vts_hdmiinput_helper_functions.h"

using android::sp;
using android::String16;
using android::defaultServiceManager;
using android::binder::Status;
using namespace std;
using namespace std::chrono;

using namespace com::rdk::hal;
using namespace com::rdk::hal::hdmiinput;

#define MAX_TRANSITORY_STATE_WAIT_MS  5
#define MAX_FINAL_STATE_WAIT_MS  500
#define MAX_MESSAGE_SENT_WAIT_MS  50

#define HDMIINPUT_MANAGER_GETHDMIINPUTIDS(hdmiinputIds) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling getHDMIInputIds(OUT:hdmiinputIds:[])"); \
        status = gHdmiInputManager->getHDMIInputIds(&hdmiinputIds); \
        UT_LOG_DEBUG("Result getHDMIInputIds(OUT:hdmiinputIds:[size: %d]), status:[isOK:%s, exception:%s]", \
                                static_cast<int>(hdmiinputIds.size()), UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                                UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE_FATAL(status.isOk()); \
        UT_ASSERT_FALSE_FATAL(hdmiinputIds.empty()); \
        UT_ASSERT_EQUAL_FATAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_MANAGER_GETHDMIINPUT_SUCCESSS(_hdmiinputId, hdmiinput) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling getHDMIInput(IN:_hdmiinputId:[%d], OUT:hdmiinput[])", _hdmiinputId.value); \
        status = gHdmiInputManager->getHDMIInput(_hdmiinputId, &hdmiinput); \
        UT_LOG_DEBUG("Result getHDMIInput(IN:_hdmiinputId:[%d], OUT:hdmiinput[0x%X]), status:[isOK:%s, exception:%s]", \
                                _hdmiinputId.value, hdmiinput.get(), UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                                UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE_FATAL(status.isOk());\
        UT_ASSERT_TRUE_FATAL(hdmiinput != nullptr); \
        UT_ASSERT_EQUAL_FATAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_MANAGER_GETHDMIINPUT_FAILURE(_hdmiinputId, hdmiinput) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling getHDMIInput(IN:_hdmiinputId:[%d], OUT:hdmiinput[])", _hdmiinputId.value); \
        status = gHdmiInputManager->getHDMIInput(_hdmiinputId, &hdmiinput); \
        UT_LOG_DEBUG("Result getHDMIInput(IN:_hdmiinputId:[%d], OUT:hdmiinput[0x%X]), status:[isOK:%s, exception:%s]", \
                                _hdmiinputId.value, hdmiinput.get(), UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                                UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE_FATAL(status.isOk());\
        UT_ASSERT_TRUE_FATAL(hdmiinput == nullptr); \
        UT_ASSERT_EQUAL_FATAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_MANAGER_GETCAPABILITIES(caps) \
     do { \
         android::binder::Status status; \
         UT_LOG_DEBUG("Calling getCapabilities(OUT:caps[])"); \
         status = gHdmiInputManager->getCapabilities(&caps); \
         UT_LOG_DEBUG("Result getCapabilities(OUT:caps[concurrentPorts: %d, Freesync: %s]), status:[isOK:%s, exception:%s]", \
            caps.maximumConcurrentStartedPorts,\
            toString(caps.freeSync).c_str(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
         UT_ASSERT_TRUE(status.isOk()); \
         UT_ASSERT_EQUAL(status.exceptionCode(), android::binder::Status::EX_NONE); \
     } while (0)

#define HDMIINPUT_GET_CAPABILITIES(hdmiinputHandle, caps) \
     do { \
         android::binder::Status status; \
         UT_LOG_DEBUG("Calling getCapabilities(OUT:caps[])"); \
         status = (hdmiinputHandle)->getCapabilities(caps); \
         UT_LOG_DEBUG("Result getCapabilities(OUT:caps), status:[isOK:%s, exception:%s]", \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
         UT_ASSERT_TRUE(status.isOk()); \
         UT_ASSERT_EQUAL(status.exceptionCode(), android::binder::Status::EX_NONE); \
     } while (0)

#define HDMIINPUT_GET_PROPERTY(hdmiinputHandle, property, propertyValue) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling getProperty(IN:property[%s], OUT:propertyValue[])", hdmiinput::toString(property).c_str()); \
        status = (hdmiinputHandle)->getProperty(property, &propertyValue); \
        if(propertyValue.has_value()) { \
            switch (propertyValue->value.value().getTag()) { \
                case hdmiinput::PropertyValue::Value::Tag::intValue: \
                    UT_LOG_DEBUG("Result getProperty(IN:property[%s], OUT:propertyValue[%d]), status:[isOK:%s, exception:%s]", \
                        hdmiinput::toString(property).c_str(), \
                        propertyValue->value->get<hdmiinput::PropertyValue::Value::Tag::intValue>(), \
                        UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                        UT_Control_GetMapString(statusHDMIInputException_mapTable, status.exceptionCode())); \
                    break; \
                case hdmiinput::PropertyValue::Value::Tag::stringValue: \
                    UT_LOG_DEBUG("Result getProperty(IN:property[%s], OUT:propertyValue[%s]), status:[isOK:%s, exception:%s]", \
                        hdmiinput::toString(property).c_str(), \
                        propertyValue->value->get<hdmiinput::PropertyValue::Value::Tag::stringValue>().string(), \
                        UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                        UT_Control_GetMapString(statusHDMIInputException_mapTable, status.exceptionCode())); \
                    break; \
                default: \
                    break; \
            } \
        } else { \
            UT_LOG_DEBUG("Result getProperty(IN:property[%s], OUT:propertyValue[]), status:[isOK:%s, exception:%s]", \
                hdmiinput::toString(property).c_str(), \
                UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                UT_Control_GetMapString(statusHDMIInputException_mapTable, status.exceptionCode())); \
            UT_ASSERT_TRUE(false); \
        } \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), android::binder::Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_GET_STATE(hdmiinputHandle, state) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling getState(OUT:state[]"); \
        status = (hdmiinputHandle)->getState(&state); \
        UT_LOG_DEBUG("Result getState(OUT:state[%s]), status:[isOK:%s, exception:%s]", hdmiinput::toString(state).c_str(), \
                UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), android::binder::Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_WAIT_FOR_STATE(hdmiinputHandle, desiredState, lastState) \
    do { \
        int32_t sleep_time = 0; \
        do \
        { \
            this_thread::sleep_for(chrono::milliseconds(MAX_TRANSITORY_STATE_WAIT_MS)); \
            sleep_time += MAX_TRANSITORY_STATE_WAIT_MS; \
            /* Check the state of the decoder */ \
            HDMIINPUT_GET_STATE((hdmiinputHandle), lastState); \
        } while (sleep_time < MAX_FINAL_STATE_WAIT_MS && lastState != desiredState); \
    } while (0)

#define HDMIINPUT_GETEDID_SUCCESS(hdmiinputHandle, edid)\
       do { \
        android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_INFO("Calling getEDID( OUT:edid:size:[])"); \
        status = (hdmiinputHandle)->getEDID(&edid,&_aidl_return);\
        UT_LOG_DEBUG("Result getEDID(OUT:edid:size[%zu] ,OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", edid.size(),\
            UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(edid.empty());  \
        UT_ASSERT_GREATER_EQUAL(edid.size(), static_cast<size_t>(128));\
        UT_ASSERT_EQUAL(0u, edid.size() % 128);\
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_TRUE(_aidl_return); \
        UT_ASSERT_EQUAL(status.exceptionCode(), android::binder::Status::EX_NONE); \
    }while (0)

#define HDMIINPUT_GETDEFAULTEDID_SUCCESS(hdmiinputHandle,version,edid)\
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_INFO("Calling getDefaultEDID(IN:version:[%s], OUT:edid:size:[])",hdmiinput::toString(version).c_str()); \
        status = (hdmiinputHandle)->getDefaultEDID(version, &edid, &_aidl_return);\
        UT_LOG_DEBUG("Result getDefaultEDID(IN:version:[%s], OUT:edid:size[%zu], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            hdmiinput::toString(version).c_str(), edid.size(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_TRUE(_aidl_return); \
        UT_ASSERT_FALSE(edid.empty());\
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    }while (0)

#define HDMIINPUT_GETDEFAULTEDID_FAILURE(hdmiinputHandle,version,edid)\
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_INFO("Calling getDefaultEDID(IN:version:[%s], OUT:edid:size:[])",hdmiinput::toString(version).c_str()); \
        status = (hdmiinputHandle)->getDefaultEDID(version, &edid, &_aidl_return);\
        UT_LOG_DEBUG("Result getDefaultEDID(IN:version:[%s], OUT:edid:size[%zu], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            hdmiinput::toString(version).c_str(), edid.size(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_FALSE(_aidl_return); \
        UT_ASSERT_TRUE(edid.empty());\
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    }while (0)

#define HDMIINPUT_GETHDCPCURRENTVERSION(hdmiinputHandle, hdcpVersion) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling getHDCPCurrentVersion(OUT:hdcpVersion[])"); \
        status = (hdmiinputHandle)->getHDCPCurrentVersion(&hdcpVersion); \
        UT_LOG_DEBUG("Result getHDCPCurrentVersion(OUT:hdcpVersion[%s]), status:[isOK:%s, exception:%s]", \
            hdmiinput::toString(hdcpVersion).c_str(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), android::binder::Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_GETHDCPSTATUS(hdmiinputHandle, hdcpStatus) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling getHDCPStatus(OUT:hdcpStatus[])"); \
        status = (hdmiinputHandle)->getHDCPStatus(&hdcpStatus); \
        UT_LOG_DEBUG("Result getHDCPStatus(OUT:hdcpStatus[%s]), status:[isOK:%s, exception:%s]", \
            hdmiinput::toString(hdcpStatus).c_str(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), android::binder::Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_GETSPDINFOFRAME(hdmiinputHandle, spdInfoFrame) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling getSPDInfoFrame(OUT:spdInfoFrame[])"); \
        status = (hdmiinputHandle)->getSPDInfoFrame(&spdInfoFrame); \
        UT_LOG_DEBUG("Result getSPDInfoFrame(OUT:spdInfoFrame[%s]), status:[isOK:%s, exception:%s]", \
            hdmiinputVecByteToHexString(spdInfoFrame).c_str(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), android::binder::Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_OPEN_SUCCESS(hdmiinputHandle, controllerListener, hdmiinputController, stateCheck) \
    do { \
        android::binder::Status status; \
        \
        /* Open the Decoder */ \
        UT_LOG_DEBUG("Calling open(IN:controllerListener[%p], OUT:hdmiinputController[])", controllerListener); \
        status = (hdmiinputHandle)->open(controllerListener, &hdmiinputController); \
        UT_LOG_DEBUG("Result open(IN:IN:controllerListener[%p], OUT:hdmiinputController[0x%X]), status:[isOK:%s, exception:%s]", \
            controllerListener, hdmiinputController.get(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE_FATAL(status.isOk()); \
        UT_ASSERT_NOT_EQUAL_FATAL(hdmiinputController.get(), nullptr); \
        UT_ASSERT_EQUAL_FATAL(status.exceptionCode(), Status::EX_NONE); \
        \
        if (stateCheck) \
        { \
            hdmiinput::State lastState; \
            HDMIINPUT_WAIT_FOR_STATE(hdmiinputHandle, State::READY, lastState); \
            UT_ASSERT_EQUAL_FATAL(lastState, State::READY); \
        } \
    } while (0)

#define HDMIINPUT_OPEN_FAILURE(hdmiinputHandle, controllerListener, hdmiinputController, expectedState, expectedException) \
    do { \
        android::binder::Status status; \
        hdmiinput::State state; \
        \
        /* Attempt to open the Decoder (expected to fail) */ \
        UT_LOG_DEBUG("Calling open(IN:controllerListener[%p], OUT:hdmiinputController[])", controllerListener); \
        status = (hdmiinputHandle)->open(controllerListener, &hdmiinputController); \
        UT_LOG_DEBUG("Result open(IN:controllerListener[%p], OUT:hdmiinputController[0x%X]), status:[isOk:%s, exception:%s]", \
            controllerListener, hdmiinputController.get(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        \
        if (expectedException == Status::EX_NONE) {\
            UT_ASSERT_TRUE(status.isOk()); \
            UT_ASSERT_EQUAL(hdmiinputController.get(), nullptr); \
        } \
        else {\
            UT_ASSERT_FALSE(status.isOk()); \
        } \
        UT_ASSERT_EQUAL(status.exceptionCode(), expectedException); \
        HDMIINPUT_GET_STATE(hdmiinputHandle, state); \
        UT_ASSERT_EQUAL_FATAL(state, expectedState); \
    } while (0)

#define HDMIINPUT_CLOSE_SUCCESS(hdmiinputHandle, hdmiinputController, stateCheck) \
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_DEBUG("Calling close(IN:hdmiinputController[%p], OUT:_aidl_return[])", hdmiinputController.get()); \
        status = hdmiinputHandle->close(hdmiinputController, &_aidl_return); \
        UT_LOG_DEBUG("Result close(IN:hdmiinputController[%p], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            hdmiinputController.get(), UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return),\
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE_FATAL(status.isOk()); \
        UT_ASSERT_TRUE_FATAL(_aidl_return); \
        UT_ASSERT_EQUAL_FATAL(status.exceptionCode(), Status::EX_NONE); \
        if (stateCheck) \
        { \
            hdmiinput::State lastState; \
            HDMIINPUT_WAIT_FOR_STATE(hdmiinputHandle, hdmiinput::State::CLOSED, lastState); \
            UT_ASSERT_EQUAL_FATAL(lastState, hdmiinput::State::CLOSED); \
        } \
    } while (0)

#define HDMIINPUT_CLOSE_FAILURE(hdmiinputHandle, hdmiinputController, expectedState, expectedException) \
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        hdmiinput::State state; \
        UT_LOG_DEBUG("Calling close(IN:hdmiinputController[%p], OUT:_aidl_return[])", hdmiinputController.get()); \
        status = hdmiinputHandle->close(hdmiinputController, &_aidl_return); \
        UT_LOG_DEBUG("Result close(IN:hdmiinputController[%p], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            hdmiinputController.get(), UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return),\
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        if (expectedException == Status::EX_NONE) { \
            UT_ASSERT_TRUE(status.isOk()); \
            UT_ASSERT_FALSE(_aidl_return); \
        } \
        else { \
            UT_ASSERT_FALSE(status.isOk()); \
        } \
        UT_ASSERT_EQUAL(status.exceptionCode(), expectedException); \
        HDMIINPUT_GET_STATE(hdmiinputHandle, state); \
        UT_ASSERT_EQUAL_FATAL(state, expectedState); \
    } while (0)

#define HDMIINPUT_GETPROPERTY_SUCCESS(hdmiinputHandle, property, propertyValue) \
    do { \
        ::android::binder::Status status; \
        UT_LOG_DEBUG("Calling getProperty(IN:property[%s], OUT:propertyValue[]", hdmiinput::toString(property).c_str()); \
        status = (hdmiinputHandle)->getProperty(property, &propertyValue); \
        if(propertyValue.has_value()) \
        { \
            switch (propertyValue->value.value().getTag()) \
            { \
                case PropertyValue::Value::Tag::intValue: \
                    UT_LOG_DEBUG("Result getProperty(IN:property[%s], OUT:propertyValue[%d]), status:[isOK:%s, exception:%s]", hdmiinput::toString(property).c_str(), \
                    propertyValue->value->get<PropertyValue::Value::Tag::intValue>(), \
                    UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                    UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
                    break; \
                default: \
                    break; \
            } \
        } \
        else \
        { \
            UT_LOG_DEBUG("Result getProperty(IN:property[%s], OUT:propertyValue[]), status:[isOK:%s, exception:%s]", hdmiinput::toString(property).c_str(), \
                    UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                    UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
            UT_ASSERT_TRUE(false); \
        } \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_GETPROPERTY_FAILURE(hdmiinputHandle, property, propertyValue, exception) \
    do { \
        ::android::binder::Status status; \
        UT_LOG_DEBUG("Calling getProperty(IN:property[%s], OUT:propertyValue[]", hdmiinput::toString(property).c_str()); \
        status = (hdmiinputHandle)->getProperty(property, &propertyValue); \
        UT_LOG_DEBUG("Result getProperty(IN:property[%s], OUT:propertyValue[]), status:[isOK:%s, exception:%s]", hdmiinput::toString(property).c_str(), \
                    UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
                    UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(propertyValue.has_value()); \
        if (exception == Status::EX_NONE) {\
            UT_ASSERT_TRUE(status.isOk()); \
            UT_ASSERT_FALSE(propertyValue.has_value()); \
        } \
        else { \
            UT_ASSERT_FALSE(status.isOk()); \
        } \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
    } while (0)

#define HDMIINPUT_REGISTEREVENT_SUCCESS(hdmiinputHandle, eventListener) \
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_DEBUG("Calling registerEventListener(IN:eventListener[%p], OUT:_aidl_return[])", eventListener.get()); \
        status = (hdmiinputHandle)->registerEventListener(eventListener, &_aidl_return); \
        UT_LOG_DEBUG("Result registerEventListener(IN:eventListener[%p], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            eventListener.get(), UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
        UT_ASSERT_TRUE(_aidl_return); \
    } while (0)

#define HDMIINPUT_REGISTEREVENT_FAILURE(hdmiinputHandle, eventListener, exception) \
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_DEBUG("Calling registerEventListener(IN:eventListener[%p], OUT:_aidl_return[])", eventListener.get()); \
        status = (hdmiinputHandle)->registerEventListener(eventListener, &_aidl_return); \
        UT_LOG_DEBUG("Result registerEventListener(IN:eventListener[%p], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            eventListener.get(), UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        if (exception == Status::EX_NONE) {\
            UT_ASSERT_TRUE(status.isOk()); \
            UT_ASSERT_FALSE(_aidl_return); \
        } \
        else { \
            UT_ASSERT_FALSE(status.isOk()); \
        } \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
    } while (0)

#define HDMIINPUT_UNREGISTEREVENT_SUCCESS(hdmiinputHandle, eventListener) \
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_DEBUG("Calling unregisterEventListener(IN:eventListener[%p], OUT:_aidl_return[])", eventListener.get()); \
        status = hdmiinputHandle->unregisterEventListener(eventListener, &_aidl_return); \
        UT_LOG_DEBUG("Result unregisterEventListener(IN:eventListener[%p], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            eventListener.get(), UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_TRUE(_aidl_return); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_UNREGISTEREVENT_FAILURE(hdmiinputHandle, eventListener, exception) \
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_DEBUG("Calling unregisterEventListener(IN:eventListener[%p], OUT:_aidl_return[])", eventListener.get()); \
        status = hdmiinputHandle->unregisterEventListener(eventListener, &_aidl_return); \
        UT_LOG_DEBUG("Result unregisterEventListener(IN:eventListener[%p], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            eventListener.get(), UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        if (exception == Status::EX_NONE) {\
            UT_ASSERT_TRUE(status.isOk()); \
            UT_ASSERT_FALSE(_aidl_return); \
        } \
        else { \
            UT_ASSERT_FALSE(status.isOk()); \
        } \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
    } while (0)

#define HDMIINPUT_GETCONNECTIONSTATE(hdmiinputController, connectionState) \
    do { \
        ::android::binder::Status status; \
        UT_LOG_DEBUG("Calling getConnectionState(OUT:connectionState[])"); \
        status = hdmiinputController->getConnectionState(&connectionState); \
        UT_LOG_DEBUG("Result getConnectionState(OUT:connectionState[%s]), status:[isOK:%s, exception:%s]", \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, connectionState), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_START_SUCCESS(hdmiinputHandle, hdmiinputController, stateCheck) \
    do { \
        ::android::binder::Status status; \
        UT_LOG_DEBUG("Calling start()"); \
        status = hdmiinputController->start(); \
        UT_LOG_DEBUG("Result start(), status:[isOK:%s, exception:%s]", \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
        if (stateCheck) \
        { \
            hdmiinput::State lastState; \
            HDMIINPUT_WAIT_FOR_STATE(hdmiinputHandle, hdmiinput::State::STARTED, lastState); \
            UT_ASSERT_EQUAL(lastState, hdmiinput::State::STARTED); \
        } \
    } while (0)

#define HDMIINPUT_START_FAILURE(hdmiinputHandle, hdmiinputController, expectedState, expectedException) \
    do { \
        android::binder::Status status; \
        hdmiinput::State state; \
        UT_LOG_DEBUG("Calling start()"); \
        status = hdmiinputController->start(); \
        UT_LOG_DEBUG("Result start(), status:[isOK:%s, exception:%s]", \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        if (expectedException == Status::EX_NONE) {\
            UT_ASSERT_TRUE(status.isOk()); \
        } \
        else {\
            UT_ASSERT_FALSE(status.isOk()); \
        } \
        UT_ASSERT_EQUAL(status.exceptionCode(), expectedException); \
        HDMIINPUT_GET_STATE(hdmiinputHandle, state); \
        UT_ASSERT_EQUAL_FATAL(state, expectedState); \
    } while (0)

#define HDMIINPUT_STOP_SUCCESS(hdmiInput, hdmiInputController, stateCheck) \
    do { \
        android::binder::Status status; \
        UT_LOG_DEBUG("Calling stop()"); \
        status = hdmiInputController->stop(); \
        UT_LOG_DEBUG("Result stop(), status:[isOK:%s, exception:%s]", \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
        if (stateCheck) \
        { \
            hdmiinput::State lastState; \
            HDMIINPUT_WAIT_FOR_STATE(hdmiInput, hdmiinput::State::READY, lastState); \
            UT_ASSERT_EQUAL(lastState, hdmiinput::State::READY); \
        } \
    } while (0)

#define HDMIINPUT_STOP_FAILURE(hdmiinputHandle, hdmiInputController, expectedState, expectedException) \
    do { \
        android::binder::Status status; \
        hdmiinput::State state; \
        UT_LOG_DEBUG("Calling stop()"); \
        status = hdmiInputController->stop(); \
        UT_LOG_DEBUG("Result stop(), status:[isOK:%s, exception:%s]", \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        if (expectedException == Status::EX_NONE) {\
            UT_ASSERT_TRUE(status.isOk()); \
        } \
        else {\
            UT_ASSERT_FALSE(status.isOk()); \
        } \
        UT_ASSERT_EQUAL(status.exceptionCode(), expectedException); \
        HDMIINPUT_GET_STATE(hdmiinputHandle, state); \
        UT_ASSERT_EQUAL_FATAL(state, expectedState); \
    } while (0)

#define HDMIINPUT_SETEDID_SUCCESS(hdmiInputController, edidBytes) \
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_DEBUG("Calling setEDID(IN:edidBytes:size:[%zu], OUT:_aidl_return[])", edidBytes.size()); \
        status = hdmiInputController->setEDID(edidBytes, &_aidl_return); \
        UT_LOG_DEBUG("Result setEDID(IN:edidBytes:size:[%zu], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            edidBytes.size(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_TRUE(_aidl_return); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)

#define HDMIINPUT_SETEDID_FAILURE(hdmiInputController, edidBytes, expectedException) \
    do { \
        ::android::binder::Status status; \
        bool _aidl_return; \
        UT_LOG_DEBUG("Calling setEDID(IN:edidBytes:size:[%zu], OUT:_aidl_return[])", edidBytes.size()); \
        status = hdmiInputController->setEDID(edidBytes, &_aidl_return); \
        UT_LOG_DEBUG("Result setEDID(IN:edidBytes:size:[%zu], OUT:_aidl_return[%s]), status:[isOK:%s, exception:%s]", \
            edidBytes.size(), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, _aidl_return), \
            UT_Control_GetMapString(bool_mapTable_hdmiinput, status.isOk()), \
            UT_Control_GetMapString(statusHdmiInputException_mapTable, status.exceptionCode())); \
        if (expectedException == Status::EX_NONE) {\
            UT_ASSERT_TRUE(status.isOk()); \
            UT_ASSERT_FALSE(_aidl_return); \
        } \
        else {\
            UT_ASSERT_FALSE(status.isOk()); \
        } \
        UT_ASSERT_EQUAL(status.exceptionCode(), expectedException); \
    } while (0)

/* Contoller Listener Class */
class HDMIInputControllerListener : public BnHDMIInputControllerListener
{
public:
    HDMIInputControllerListener() : BnHDMIInputControllerListener(){}
    virtual ~HDMIInputControllerListener() = default;

    android::binder::Status onConnectionStateChanged(bool connectionState) override;
    android::binder::Status onSignalStateChanged(::com::rdk::hal::hdmiinput::SignalState signalState) override;
    android::binder::Status onVideoFormatChanged(::com::rdk::hal::hdmiinput::VIC vic) override;
    android::binder::Status onVRRChanged(bool vrrActive, bool M_CONST, bool fastVActive, double frameRate) override;
    android::binder::Status onAVIInfoFrame(const ::std::vector<uint8_t>& data) override;
    android::binder::Status onAudioInfoFrame(const ::std::vector<uint8_t>& data) override;
    android::binder::Status onSPDInfoFrame(const ::std::vector<uint8_t>& data) override;
    android::binder::Status onDRMInfoFrame(const ::std::vector<uint8_t>& data) override;
    android::binder::Status onVendorSpecificInfoFrame(const ::std::vector<uint8_t>& data) override;
    android::binder::Status onHDCPStatusChanged(::com::rdk::hal::hdmiinput::HDCPStatus hdcpStatus, ::com::rdk::hal::hdmiinput::HDCPProtocolVersion hdcpProtocolVersion) override;

public:
    bool getNextConnectionState(bool& connectionState);
    bool getNextSignalState(hdmiinput::SignalState& signalState);
    bool getNextVideoFormat(hdmiinput::VIC& vic);
    bool getNextVRRInfo(bool& vrrActive, bool& M_CONST, bool& fastVActive, double& frameRate);
    bool getNextAVIInfoFrame(std::vector<uint8_t>& data);
    bool getNextAudioInfoFrame(std::vector<uint8_t>& data);
    bool getNextSPDInfoFrame(std::vector<uint8_t>& data);
    bool getNextDRMInfoFrame(std::vector<uint8_t>& data);
    bool getNextVendorSpecificInfoFrame(std::vector<uint8_t>& data);
    bool getNextHDCPStatus(hdmiinput::HDCPStatus& hdcpStatus, hdmiinput::HDCPProtocolVersion& hdcpProtocolVersion);
private:
    mutex mConnectionStateMutex;
    mutex mSignalStateMutex;
    mutex mVideoFormatMutex;
    mutex mVRRMutex;
    mutex mAVIInfoMutex;
    mutex mAudioInfoMutex;
    mutex mSPDInfoMutex;
    mutex mDRMInfoMutex;
    mutex mVendorSpecificInfoMutex;
    mutex mHDCPStatusMutex;

    std::queue<bool> mConnectionStateQueue;
    std::queue<hdmiinput::SignalState> mSignalStateQueue;
    std::queue<hdmiinput::VIC> mVideoFormatQueue;
    std::queue<std::tuple<bool, bool, bool, double>> mVRRQueue;
    std::queue<std::vector<uint8_t>> mAVIInfoQueue;
    std::queue<std::vector<uint8_t>> mAudioInfoQueue;
    std::queue<std::vector<uint8_t>> mSPDInfoQueue;
    std::queue<std::vector<uint8_t>> mDRMInfoQueue;
    std::queue<std::vector<uint8_t>> mVendorSpecificInfoQueue;
    std::queue<std::tuple<hdmiinput::HDCPStatus, hdmiinput::HDCPProtocolVersion>> mHDCPStatusQueue;
};

/* Event Listener Class */
class HDMIInputEventListener : public BnHDMIInputEventListener
{
public:
    HDMIInputEventListener(){}
    virtual ~HDMIInputEventListener() = default;

    android::binder::Status onStateChanged(::com::rdk::hal::hdmiinput::State oldState, ::com::rdk::hal::hdmiinput::State newState) override;
    android::binder::Status onEDIDChange(const ::std::vector<uint8_t>& edid) override;

public:
    bool getNextStateChanges(pair<hdmiinput::State, hdmiinput::State>& stateChanges);
    bool getNextEDIDChange(std::vector<uint8_t>& edid);

private:
    mutex mStateChangeMutex;
    mutex mEDIDChangeMutex;

    std::queue<std::pair<hdmiinput::State, hdmiinput::State>> mStateChangeQueue;
    std::queue<std::vector<uint8_t>> mEDIDChangeQueue;
};

extern android::sp<::com::rdk::hal::hdmiinput::IHDMIInputManager> gHdmiInputManager;

#endif // __VTS_HDMIINPUT_H__

/** @} */ // End of HDMI_INPUT_HALTEST_HEADER
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT Module
/** @} */ // End of HPK