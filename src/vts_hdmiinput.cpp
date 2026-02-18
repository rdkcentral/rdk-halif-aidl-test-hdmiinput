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
    {
        std::lock_guard<std::mutex> lock(mConnectionStateMutex);
        mConnectionStateQueue.emplace(connectionState);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onConnectionStateChanged called with connectionState[%s]", UT_Control_GetMapString(bool_mapTable_hdmiinput, connectionState));
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextConnectionState(bool& connectionState)
{
    std::lock_guard<std::mutex> lock(mConnectionStateMutex);
    if (mConnectionStateQueue.empty())
    {
        return false;
    }
    connectionState = mConnectionStateQueue.front();
    mConnectionStateQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onSignalStateChanged(hdmiinput::SignalState signalState)
{
    {
        std::lock_guard<std::mutex> lock(mSignalStateMutex);
        mSignalStateQueue.emplace(signalState);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onSignalStateChanged called with signalState[%s]", hdmiinput::toString(signalState).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextSignalState(hdmiinput::SignalState& signalState)
{
    std::lock_guard<std::mutex> lock(mSignalStateMutex);
    if (mSignalStateQueue.empty())
    {
        return false;
    }
    signalState = mSignalStateQueue.front();
    mSignalStateQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onVideoFormatChanged(hdmiinput::VIC vic)
{
    {
        std::lock_guard<std::mutex> lock(mVideoFormatMutex);
        mVideoFormatQueue.emplace(vic);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onVideoFormatChanged called with VIC[%s]", hdmiinput::toString(vic).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextVideoFormat(hdmiinput::VIC& vic)
{
    std::lock_guard<std::mutex> lock(mVideoFormatMutex);
    if (mVideoFormatQueue.empty())
    {
        return false;
    }
    vic = mVideoFormatQueue.front();
    mVideoFormatQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onVRRChanged(bool vrrActive, bool M_CONST, bool fastVActive, double frameRate)
{
    {
        std::lock_guard<std::mutex> lock(mVRRMutex);
        mVRRQueue.emplace(vrrActive, M_CONST, fastVActive, frameRate);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onVRRChanged called with vrrActive[%s], M_CONST[%s], fastVActive[%s], frameRate[%.2f]",
        UT_Control_GetMapString(bool_mapTable_hdmiinput, vrrActive),
        UT_Control_GetMapString(bool_mapTable_hdmiinput, M_CONST),
        UT_Control_GetMapString(bool_mapTable_hdmiinput, fastVActive),
        frameRate);
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextVRRInfo(bool& vrrActive, bool& M_CONST, bool& fastVActive, double& frameRate)
{
    std::lock_guard<std::mutex> lock(mVRRMutex);
    if (mVRRQueue.empty())
    {
        return false;
    }
    std::tie(vrrActive, M_CONST, fastVActive, frameRate) = mVRRQueue.front();
    mVRRQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onAVIInfoFrame(const ::std::vector<uint8_t>& data)
{
    {
        std::lock_guard<std::mutex> lock(mAVIInfoMutex);
        mAVIInfoQueue.emplace(data);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onAVIInfoFrame called with size[%d] data[%s]", static_cast<int>(data.size()), hdmiinputVecByteToHexString(data).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextAVIInfoFrame(std::vector<uint8_t>& data)
{
    std::lock_guard<std::mutex> lock(mAVIInfoMutex);
    if (mAVIInfoQueue.empty())
    {
        return false;
    }
    data = mAVIInfoQueue.front();
    mAVIInfoQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onAudioInfoFrame(const ::std::vector<uint8_t>& data)
{
    {
        std::lock_guard<std::mutex> lock(mAudioInfoMutex);
        mAudioInfoQueue.emplace(data);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onAudioInfoFrame called with size[%d] data[%s]", static_cast<int>(data.size()), hdmiinputVecByteToHexString(data).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextAudioInfoFrame(std::vector<uint8_t>& data)
{
    std::lock_guard<std::mutex> lock(mAudioInfoMutex);
    if (mAudioInfoQueue.empty())
    {
        return false;
    }
    data = mAudioInfoQueue.front();
    mAudioInfoQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onSPDInfoFrame(const ::std::vector<uint8_t>& data)
{
    {
        std::lock_guard<std::mutex> lock(mSPDInfoMutex);
        mSPDInfoQueue.emplace(data);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onSPDInfoFrame called with size[%d] data[%s]", static_cast<int>(data.size()), hdmiinputVecByteToHexString(data).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextSPDInfoFrame(std::vector<uint8_t>& data)
{
    std::lock_guard<std::mutex> lock(mSPDInfoMutex);
    if (mSPDInfoQueue.empty())
    {
        return false;
    }
    data = mSPDInfoQueue.front();
    mSPDInfoQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onDRMInfoFrame(const ::std::vector<uint8_t>& data)
{
    {
        std::lock_guard<std::mutex> lock(mDRMInfoMutex);
        mDRMInfoQueue.emplace(data);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onDRMInfoFrame called with size[%d] data[%s]", static_cast<int>(data.size()), hdmiinputVecByteToHexString(data).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextDRMInfoFrame(std::vector<uint8_t>& data)
{
    std::lock_guard<std::mutex> lock(mDRMInfoMutex);
    if (mDRMInfoQueue.empty())
    {
        return false;
    }
    data = mDRMInfoQueue.front();
    mDRMInfoQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onVendorSpecificInfoFrame(const ::std::vector<uint8_t>& data)
{
    {
        std::lock_guard<std::mutex> lock(mVendorSpecificInfoMutex);
        mVendorSpecificInfoQueue.emplace(data);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onVendorSpecificInfoFrame called with size[%d] data[%s]", static_cast<int>(data.size()), hdmiinputVecByteToHexString(data).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextVendorSpecificInfoFrame(std::vector<uint8_t>& data)
{
    std::lock_guard<std::mutex> lock(mVendorSpecificInfoMutex);
    if (mVendorSpecificInfoQueue.empty())
    {
        return false;
    }
    data = mVendorSpecificInfoQueue.front();
    mVendorSpecificInfoQueue.pop();
    return true;
}

android::binder::Status HDMIInputControllerListener::onHDCPStatusChanged(hdmiinput::HDCPStatus hdcpStatus, hdmiinput::HDCPProtocolVersion hdcpProtocolVersion)
{
    {
        std::lock_guard<std::mutex> lock(mHDCPStatusMutex);
        mHDCPStatusQueue.emplace(hdcpStatus, hdcpProtocolVersion);
    }
    UT_LOG_INFO("HDMIInputControllerListener::onHDCPStatusChanged called with hdcpStatus[%s], hdcpProtocolVersion[%s]",
        hdmiinput::toString(hdcpStatus).c_str(),
        hdmiinput::toString(hdcpProtocolVersion).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputControllerListener::getNextHDCPStatus(hdmiinput::HDCPStatus& hdcpStatus, hdmiinput::HDCPProtocolVersion& hdcpProtocolVersion)
{
    std::lock_guard<std::mutex> lock(mHDCPStatusMutex);
    if (mHDCPStatusQueue.empty())
    {
        return false;
    }
    std::tie(hdcpStatus, hdcpProtocolVersion) = mHDCPStatusQueue.front();
    mHDCPStatusQueue.pop();
    return true;
}

/* Event Listener Class */
android::binder::Status HDMIInputEventListener::onStateChanged(hdmiinput::State oldState, hdmiinput::State newState)
{
    {
        std::lock_guard<std::mutex> lock(mStateChangeMutex);
        mStateChangeQueue.emplace(oldState, newState);
    }
    UT_LOG_INFO("HDMIInputEventListener::onStateChanged called with oldState[%s], newState[%s]",
        hdmiinput::toString(oldState).c_str(),
        hdmiinput::toString(newState).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputEventListener::getNextStateChanges(pair<hdmiinput::State, hdmiinput::State>& stateChanges)
{
    std::lock_guard<std::mutex> lock(mStateChangeMutex);
    if (mStateChangeQueue.empty())
    {
        return false;
    }
    stateChanges = mStateChangeQueue.front();
    mStateChangeQueue.pop();
    return true;
}

android::binder::Status HDMIInputEventListener::onEDIDChange(const std::vector<uint8_t>& edid)
{
    {
        std::lock_guard<std::mutex> lock(mEDIDChangeMutex);
        mEDIDChangeQueue.emplace(edid);
    }
    UT_LOG_INFO("HDMIInputEventListener::onEDIDChange called with edid size[%d] data[%s]", static_cast<int>(edid.size()), hdmiinputVecByteToHexString(edid).c_str());
    return android::binder::Status::ok();
}

bool HDMIInputEventListener::getNextEDIDChange(std::vector<uint8_t>& edid)
{
    std::lock_guard<std::mutex> lock(mEDIDChangeMutex);
    if (mEDIDChangeQueue.empty())
    {
        return false;
    }
    edid = mEDIDChangeQueue.front();
    mEDIDChangeQueue.pop();
    return true;
}

/** @} */ // End of HDMI_INPUT_HALTEST_HEADER
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT
/** @} */ // End of HPK