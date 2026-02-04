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
 * @defgroup HDMI_INPUT_HALTEST_CLASS HDMI INPUT HAL Tests helper functions Header File
 * @{
 * @parblock
 *
 * ### Common functions/macros required for the HDMI INPUT module :
 *
 * Common functions/macros required for the HDMI INPUT module .
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_hdmiinput.cpp
 *
 */

#include "vts_hdmiinput.h"


/* Controller Listener Class */
android::binder::Status HDMIInputControllerListener::onConnectionStateChanged(bool connectionState)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onSignalStateChanged(::com::rdk::hal::hdmiinput::SignalState signalState)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onVideoFormatChanged(::com::rdk::hal::hdmiinput::VIC vic)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onVRRChanged(bool vrrActive, bool M_CONST, bool fastVActive, double frameRate)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onAVIInfoFrame(const ::std::vector<uint8_t>& data)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onAudioInfoFrame(const ::std::vector<uint8_t>& data)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onSPDInfoFrame(const ::std::vector<uint8_t>& data)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onDRMInfoFrame(const ::std::vector<uint8_t>& data)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onVendorSpecificInfoFrame(const ::std::vector<uint8_t>& data)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputControllerListener::onHDCPStatusChanged(::com::rdk::hal::hdmiinput::HDCPStatus hdcpStatus, ::com::rdk::hal::hdmiinput::HDCPProtocolVersion hdcpProtocolVersion)
{
	return android::binder::Status::ok();
}

/* Event Listener Class */
android::binder::Status HDMIInputEventListener::onStateChanged(::com::rdk::hal::hdmiinput::State oldState, ::com::rdk::hal::hdmiinput::State /*newState*/)
{
	return android::binder::Status::ok();
}

android::binder::Status HDMIInputEventListener::onEDIDChange(const ::std::vector<uint8_t>& edid)
{
	return android::binder::Status::ok();
}

/** @} */ // End of HDMI_INPUT_HALTEST_HEADER
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT
/** @} */ // End of HPK