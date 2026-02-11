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
 * @defgroup HDMI_INPUT_HALTEST_HELPER HDMI INPUT HAL Tests helper functions
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
 * @file vts_hdmiinput_helper_functions.cpp
 *
 */

#include "vts_hdmiinput_helper_functions.h"

const ut_control_keyStringMapping_t bool_mapTable_hdmiinput[] =
{
    {"false", (int32_t) false},
    {"true",  (int32_t) true},
    {NULL, -1}
};

/* Status::Exception */
const ut_control_keyStringMapping_t statusHdmiInputException_mapTable[] =
{
    {"EX_NONE",                  (int32_t) Status::EX_NONE},
    {"EX_SECURITY",              (int32_t) Status::EX_SECURITY},
    {"EX_BAD_PARCELABLE",        (int32_t) Status::EX_BAD_PARCELABLE},
    {"EX_ILLEGAL_ARGUMENT",      (int32_t) Status::EX_ILLEGAL_ARGUMENT},
    {"EX_NULL_POINTER",          (int32_t) Status::EX_NULL_POINTER},
    {"EX_ILLEGAL_STATE",         (int32_t) Status::EX_ILLEGAL_STATE},
    {"EX_NETWORK_MAIN_THREAD",   (int32_t) Status::EX_NETWORK_MAIN_THREAD},
    {"EX_UNSUPPORTED_OPERATION", (int32_t) Status::EX_UNSUPPORTED_OPERATION},
    {"EX_SERVICE_SPECIFIC",      (int32_t) Status::EX_SERVICE_SPECIFIC},
    {"EX_PARCELABLE",            (int32_t) Status::EX_PARCELABLE},
    {"EX_HAS_REPLY_HEADER",      (int32_t) Status::EX_HAS_REPLY_HEADER},
    {"EX_TRANSACTION_FAILED",    (int32_t) Status::EX_TRANSACTION_FAILED},
    {NULL, -1}
};

/* HDMIVersion */
const ut_control_keyStringMapping_t hdmiVersion_mapTable[] =
{
    {"UNDEFINED",    (int32_t) HDMIVersion::HDMI_1_3},
    {"VERSION_1_X",  (int32_t) HDMIVersion::HDMI_1_4},
    {"VERSION_2_X",  (int32_t) HDMIVersion::HDMI_2_0},
    {"VERSION_2_X",  (int32_t) HDMIVersion::HDMI_2_1},
    {NULL, -1}
};

/* HDCPProtocolVersion */
const ut_control_keyStringMapping_t hdcpProtocolVersion_mapTable[] =
{
    {"UNDEFINED",    (int32_t) HDCPProtocolVersion::UNDEFINED},
    {"VERSION_1_X",  (int32_t) HDCPProtocolVersion::VERSION_1_X},
    {"VERSION_2_X",  (int32_t) HDCPProtocolVersion::VERSION_2_X},
    {NULL, -1}
};

/* FreeSync */
const ut_control_keyStringMapping_t freeSync_mapTable[] =
{
    {"UNSUPPORTED",  (int32_t) FreeSync::UNSUPPORTED},
    {"FREESYNC",    (int32_t) FreeSync::FREESYNC},
    {"FREESYNC_PREMIUM",    (int32_t) FreeSync::FREESYNC_PREMIUM},
    {"FREESYNC_PREMIUM_PRO",    (int32_t) FreeSync::FREESYNC_PREMIUM_PRO},
    {NULL, -1}
};

/* VIC Enum Mapping Table */
const ut_control_keyStringMapping_t vic_mapTable[] =
{
    {"VIC0_UNAVAILABLE", (int32_t)VIC::VIC0_UNAVAILABLE},

    {"VIC1_640_480_P_60_4_3", (int32_t)VIC::VIC1_640_480_P_60_4_3},
    {"VIC2_720_480_P_60_4_3", (int32_t)VIC::VIC2_720_480_P_60_4_3},
    {"VIC3_720_480_P_60_16_9", (int32_t)VIC::VIC3_720_480_P_60_16_9},
    {"VIC4_1280_720_P_60_16_9", (int32_t)VIC::VIC4_1280_720_P_60_16_9},
    {"VIC5_1920_1080_I_60_16_9", (int32_t)VIC::VIC5_1920_1080_I_60_16_9},
    {"VIC6_720_1440_480_I_60_4_3", (int32_t)VIC::VIC6_720_1440_480_I_60_4_3},
    {"VIC7_720_1440_480_I_60_16_9", (int32_t)VIC::VIC7_720_1440_480_I_60_16_9},
    {"VIC8_720_1440_240_P_60_4_3", (int32_t)VIC::VIC8_720_1440_240_P_60_4_3},
    {"VIC9_720_1440_240_P_60_16_9", (int32_t)VIC::VIC9_720_1440_240_P_60_16_9},
    {"VIC10_2880_480_I_60_4_3", (int32_t)VIC::VIC10_2880_480_I_60_4_3},
    {"VIC11_2880_480_I_60_16_9", (int32_t)VIC::VIC11_2880_480_I_60_16_9},
    {"VIC12_2880_240_P_60_4_3", (int32_t)VIC::VIC12_2880_240_P_60_4_3},
    {"VIC13_2880_240_P_60_16_9", (int32_t)VIC::VIC13_2880_240_P_60_16_9},
    {"VIC14_1440_480_P_60_4_3", (int32_t)VIC::VIC14_1440_480_P_60_4_3},
    {"VIC15_1440_480_P_60_16_9", (int32_t)VIC::VIC15_1440_480_P_60_16_9},
    {"VIC16_1920_1080_P_60_16_9", (int32_t)VIC::VIC16_1920_1080_P_60_16_9},
    {"VIC17_720_576_P_50_4_3", (int32_t)VIC::VIC17_720_576_P_50_4_3},
    {"VIC18_720_576_P_50_16_9", (int32_t)VIC::VIC18_720_576_P_50_16_9},
    {"VIC19_1280_720_P_50_16_9", (int32_t)VIC::VIC19_1280_720_P_50_16_9},
    {"VIC20_1920_1080_I_50_16_9", (int32_t)VIC::VIC20_1920_1080_I_50_16_9},
    {"VIC21_720_1440_576_I_50_4_3", (int32_t)VIC::VIC21_720_1440_576_I_50_4_3},
    {"VIC22_720_1440_576_I_50_16_9", (int32_t)VIC::VIC22_720_1440_576_I_50_16_9},
    {"VIC23_720_1440_288_P_50_4_3", (int32_t)VIC::VIC23_720_1440_288_P_50_4_3},
    {"VIC24_720_1440_288_P_50_16_9", (int32_t)VIC::VIC24_720_1440_288_P_50_16_9},
    {"VIC25_2880_576_I_50_4_3", (int32_t)VIC::VIC25_2880_576_I_50_4_3},
    {"VIC26_2880_576_I_50_16_9", (int32_t)VIC::VIC26_2880_576_I_50_16_9},
    {"VIC27_2880_288_P_50_4_3", (int32_t)VIC::VIC27_2880_288_P_50_4_3},
    {"VIC28_2880_288_P_50_16_9", (int32_t)VIC::VIC28_2880_288_P_50_16_9},
    {"VIC29_1440_576_P_50_4_3", (int32_t)VIC::VIC29_1440_576_P_50_4_3},
    {"VIC30_1440_576_P_50_16_9", (int32_t)VIC::VIC30_1440_576_P_50_16_9},
    {"VIC31_1920_1080_P_50_16_9", (int32_t)VIC::VIC31_1920_1080_P_50_16_9},
    {"VIC32_1920_1080_P_24_16_9", (int32_t)VIC::VIC32_1920_1080_P_24_16_9},
    {"VIC33_1920_1080_P_25_16_9", (int32_t)VIC::VIC33_1920_1080_P_25_16_9},
    {"VIC34_1920_1080_P_30_16_9", (int32_t)VIC::VIC34_1920_1080_P_30_16_9},
    {"VIC35_2880_480_P_60_4_3", (int32_t)VIC::VIC35_2880_480_P_60_4_3},
    {"VIC36_2880_480_P_60_16_9", (int32_t)VIC::VIC36_2880_480_P_60_16_9},
    {"VIC37_2880_576_P_50_4_3", (int32_t)VIC::VIC37_2880_576_P_50_4_3},
    {"VIC38_2880_576_P_50_16_9", (int32_t)VIC::VIC38_2880_576_P_50_16_9},
    {"VIC39_1920_1080_I_50_16_9", (int32_t)VIC::VIC39_1920_1080_I_50_16_9},
    {"VIC40_1920_1080_I_100_16_9", (int32_t)VIC::VIC40_1920_1080_I_100_16_9},
    {"VIC41_1280_720_P_100_16_9", (int32_t)VIC::VIC41_1280_720_P_100_16_9},
    {"VIC42_720_576_P_100_4_3", (int32_t)VIC::VIC42_720_576_P_100_4_3},
    {"VIC43_720_576_P_100_16_9", (int32_t)VIC::VIC43_720_576_P_100_16_9},
    {"VIC44_720_1440_576_I_100_4_3", (int32_t)VIC::VIC44_720_1440_576_I_100_4_3},
    {"VIC45_720_1440_576_I_100_16_9", (int32_t)VIC::VIC45_720_1440_576_I_100_16_9},
    {"VIC46_1920_1080_I_120_16_9", (int32_t)VIC::VIC46_1920_1080_I_120_16_9},
    {"VIC47_1280_720_P_120_16_9", (int32_t)VIC::VIC47_1280_720_P_120_16_9},
    {"VIC48_720_480_P_120_4_3", (int32_t)VIC::VIC48_720_480_P_120_4_3},
    {"VIC49_720_480_P_120_16_9", (int32_t)VIC::VIC49_720_480_P_120_16_9},
    {"VIC50_720_1440_480_I_120_4_3", (int32_t)VIC::VIC50_720_1440_480_I_120_4_3},
    {"VIC51_720_1440_480_I_120_16_9", (int32_t)VIC::VIC51_720_1440_480_I_120_16_9},
    {"VIC52_720_576_P_200_4_3", (int32_t)VIC::VIC52_720_576_P_200_4_3},
    {"VIC53_720_576_P_200_16_9", (int32_t)VIC::VIC53_720_576_P_200_16_9},
    {"VIC54_720_1440_576_I_200_4_3", (int32_t)VIC::VIC54_720_1440_576_I_200_4_3},
    {"VIC55_720_1440_576_I_200_16_9", (int32_t)VIC::VIC55_720_1440_576_I_200_16_9},
    {"VIC56_720_480_P_240_4_3", (int32_t)VIC::VIC56_720_480_P_240_4_3},
    {"VIC57_720_480_P_240_16_9", (int32_t)VIC::VIC57_720_480_P_240_16_9},
    {"VIC58_720_1440_480_I_240_4_3", (int32_t)VIC::VIC58_720_1440_480_I_240_4_3},
    {"VIC59_720_1440_480_I_240_16_9", (int32_t)VIC::VIC59_720_1440_480_I_240_16_9},
    {"VIC60_1280_720_P_24_16_9", (int32_t)VIC::VIC60_1280_720_P_24_16_9},
    {"VIC61_1280_720_P_25_16_9", (int32_t)VIC::VIC61_1280_720_P_25_16_9},
    {"VIC62_1280_720_P_30_16_9", (int32_t)VIC::VIC62_1280_720_P_30_16_9},
    {"VIC63_1920_1080_P_120_16_9", (int32_t)VIC::VIC63_1920_1080_P_120_16_9},
    {"VIC64_1920_1080_P_100_16_9", (int32_t)VIC::VIC64_1920_1080_P_100_16_9},

    {"VIC65_1280_720_P_24_64_27", (int32_t)VIC::VIC65_1280_720_P_24_64_27},
    {"VIC66_1280_720_P_25_64_27", (int32_t)VIC::VIC66_1280_720_P_25_64_27},
    {"VIC67_1280_720_P_30_64_27", (int32_t)VIC::VIC67_1280_720_P_30_64_27},
    {"VIC68_1280_720_P_50_64_27", (int32_t)VIC::VIC68_1280_720_P_50_64_27},
    {"VIC69_1280_720_P_60_64_27", (int32_t)VIC::VIC69_1280_720_P_60_64_27},
    {"VIC70_1280_720_P_100_64_27", (int32_t)VIC::VIC70_1280_720_P_100_64_27},
    {"VIC71_1280_720_P_120_64_27", (int32_t)VIC::VIC71_1280_720_P_120_64_27},

    {"VIC72_1920_1080_P_24_64_27", (int32_t)VIC::VIC72_1920_1080_P_24_64_27},
    {"VIC73_1920_1080_P_25_64_27", (int32_t)VIC::VIC73_1920_1080_P_25_64_27},
    {"VIC74_1920_1080_P_30_64_27", (int32_t)VIC::VIC74_1920_1080_P_30_64_27},
    {"VIC75_1920_1080_P_50_64_27", (int32_t)VIC::VIC75_1920_1080_P_50_64_27},
    {"VIC76_1920_1080_P_60_64_27", (int32_t)VIC::VIC76_1920_1080_P_60_64_27},
    {"VIC77_1920_1080_P_100_64_27", (int32_t)VIC::VIC77_1920_1080_P_100_64_27},
    {"VIC78_1920_1080_P_120_64_27", (int32_t)VIC::VIC78_1920_1080_P_120_64_27},

    {"VIC79_1680_720_P_24_64_27", (int32_t)VIC::VIC79_1680_720_P_24_64_27},
    {"VIC80_1680_720_P_25_64_27", (int32_t)VIC::VIC80_1680_720_P_25_64_27},
    {"VIC81_1680_720_P_30_64_27", (int32_t)VIC::VIC81_1680_720_P_30_64_27},
    {"VIC82_1680_720_P_50_64_27", (int32_t)VIC::VIC82_1680_720_P_50_64_27},
    {"VIC83_1680_720_P_60_64_27", (int32_t)VIC::VIC83_1680_720_P_60_64_27},
    {"VIC84_1680_720_P_100_64_27", (int32_t)VIC::VIC84_1680_720_P_100_64_27},
    {"VIC85_1680_720_P_120_64_27", (int32_t)VIC::VIC85_1680_720_P_120_64_27},

    {"VIC86_2560_1080_P_24_64_27", (int32_t)VIC::VIC86_2560_1080_P_24_64_27},
    {"VIC87_2560_1080_P_25_64_27", (int32_t)VIC::VIC87_2560_1080_P_25_64_27},
    {"VIC88_2560_1080_P_30_64_27", (int32_t)VIC::VIC88_2560_1080_P_30_64_27},
    {"VIC89_2560_1080_P_50_64_27", (int32_t)VIC::VIC89_2560_1080_P_50_64_27},
    {"VIC90_2560_1080_P_60_64_27", (int32_t)VIC::VIC90_2560_1080_P_60_64_27},
    {"VIC91_2560_1080_P_100_64_27", (int32_t)VIC::VIC91_2560_1080_P_100_64_27},
    {"VIC92_2560_1080_P_120_64_27", (int32_t)VIC::VIC92_2560_1080_P_120_64_27},

    {"VIC93_3840_2160_P_24_16_9", (int32_t)VIC::VIC93_3840_2160_P_24_16_9},
    {"VIC94_3840_2160_P_25_16_9", (int32_t)VIC::VIC94_3840_2160_P_25_16_9},
    {"VIC95_3840_2160_P_30_16_9", (int32_t)VIC::VIC95_3840_2160_P_30_16_9},
    {"VIC96_3840_2160_P_50_16_9", (int32_t)VIC::VIC96_3840_2160_P_50_16_9},
    {"VIC97_3840_2160_P_60_16_9", (int32_t)VIC::VIC97_3840_2160_P_60_16_9},

    {"VIC98_4096_2160_P_24_256_135", (int32_t)VIC::VIC98_4096_2160_P_24_256_135},
    {"VIC99_4096_2160_P_25_256_135", (int32_t)VIC::VIC99_4096_2160_P_25_256_135},
    {"VIC100_4096_2160_P_30_256_135", (int32_t)VIC::VIC100_4096_2160_P_30_256_135},
    {"VIC101_4096_2160_P_50_256_135", (int32_t)VIC::VIC101_4096_2160_P_50_256_135},
    {"VIC102_4096_2160_P_60_256_135", (int32_t)VIC::VIC102_4096_2160_P_60_256_135},

    {"VIC103_3840_2160_P_24_64_27", (int32_t)VIC::VIC103_3840_2160_P_24_64_27},
    {"VIC104_3840_2160_P_25_64_27", (int32_t)VIC::VIC104_3840_2160_P_25_64_27},
    {"VIC105_3840_2160_P_30_64_27", (int32_t)VIC::VIC105_3840_2160_P_30_64_27},
    {"VIC106_3840_2160_P_50_64_27", (int32_t)VIC::VIC106_3840_2160_P_50_64_27},
    {"VIC107_3840_2160_P_60_64_27", (int32_t)VIC::VIC107_3840_2160_P_60_64_27},

    {"VIC108_1280_720_P_48_16_9", (int32_t)VIC::VIC108_1280_720_P_48_16_9},
    {"VIC109_1280_720_P_48_64_27", (int32_t)VIC::VIC109_1280_720_P_48_64_27},
    {"VIC110_1680_720_P_48_64_27", (int32_t)VIC::VIC110_1680_720_P_48_64_27},
    {"VIC111_1920_1080_P_48_16_9", (int32_t)VIC::VIC111_1920_1080_P_48_16_9},
    {"VIC112_1920_1080_P_48_64_27", (int32_t)VIC::VIC112_1920_1080_P_48_64_27},
    {"VIC113_2560_1080_P_48_64_27", (int32_t)VIC::VIC113_2560_1080_P_48_64_27},
    {"VIC114_3840_2160_P_48_16_9", (int32_t)VIC::VIC114_3840_2160_P_48_16_9},
    {"VIC115_4096_2160_P_48_256_135", (int32_t)VIC::VIC115_4096_2160_P_48_256_135},
    {"VIC116_3840_2160_P_48_64_27", (int32_t)VIC::VIC116_3840_2160_P_48_64_27},

    {"VIC117_3840_2160_P_100_16_9", (int32_t)VIC::VIC117_3840_2160_P_100_16_9},
    {"VIC118_3840_2160_P_120_16_9", (int32_t)VIC::VIC118_3840_2160_P_120_16_9},
    {"VIC119_3840_2160_P_100_64_27", (int32_t)VIC::VIC119_3840_2160_P_100_64_27},
    {"VIC120_3840_2160_P_120_64_27", (int32_t)VIC::VIC120_3840_2160_P_120_64_27},

    {"VIC121_5120_2160_P_24_64_27", (int32_t)VIC::VIC121_5120_2160_P_24_64_27},
    {"VIC122_5120_2160_P_25_64_27", (int32_t)VIC::VIC122_5120_2160_P_25_64_27},
    {"VIC123_5120_2160_P_30_64_27", (int32_t)VIC::VIC123_5120_2160_P_30_64_27},
    {"VIC124_5120_2160_P_48_64_27", (int32_t)VIC::VIC124_5120_2160_P_48_64_27},
    {"VIC125_5120_2160_P_50_64_27", (int32_t)VIC::VIC125_5120_2160_P_50_64_27},
    {"VIC126_5120_2160_P_60_64_27", (int32_t)VIC::VIC126_5120_2160_P_60_64_27},
    {"VIC127_5120_2160_P_100_64_27", (int32_t)VIC::VIC127_5120_2160_P_100_64_27},
    {"VIC193_5120_2160_P_120_64_27", (int32_t)VIC::VIC193_5120_2160_P_120_64_27},

    {"VIC194_7680_4320_P_24_16_9", (int32_t)VIC::VIC194_7680_4320_P_24_16_9},
    {"VIC195_7680_4320_P_25_16_9", (int32_t)VIC::VIC195_7680_4320_P_25_16_9},
    {"VIC196_7680_4320_P_30_16_9", (int32_t)VIC::VIC196_7680_4320_P_30_16_9},
    {"VIC197_7680_4320_P_48_16_9", (int32_t)VIC::VIC197_7680_4320_P_48_16_9},
    {"VIC198_7680_4320_P_50_16_9", (int32_t)VIC::VIC198_7680_4320_P_50_16_9},
    {"VIC199_7680_4320_P_60_16_9", (int32_t)VIC::VIC199_7680_4320_P_60_16_9},
    {"VIC200_7680_4320_P_100_16_9", (int32_t)VIC::VIC200_7680_4320_P_100_16_9},
    {"VIC201_7680_4320_P_120_16_9", (int32_t)VIC::VIC201_7680_4320_P_120_16_9},

    {"VIC202_7680_4320_P_24_64_27", (int32_t)VIC::VIC202_7680_4320_P_24_64_27},
    {"VIC203_7680_4320_P_25_64_27", (int32_t)VIC::VIC203_7680_4320_P_25_64_27},
    {"VIC204_7680_4320_P_30_64_27", (int32_t)VIC::VIC204_7680_4320_P_30_64_27},
    {"VIC205_7680_4320_P_48_64_27", (int32_t)VIC::VIC205_7680_4320_P_48_64_27},
    {"VIC206_7680_4320_P_50_64_27", (int32_t)VIC::VIC206_7680_4320_P_50_64_27},
    {"VIC207_7680_4320_P_60_64_27", (int32_t)VIC::VIC207_7680_4320_P_60_64_27},
    {"VIC208_7680_4320_P_100_64_27", (int32_t)VIC::VIC208_7680_4320_P_100_64_27},
    {"VIC209_7680_4320_P_120_64_27", (int32_t)VIC::VIC209_7680_4320_P_120_64_27},

    {"VIC210_10240_4320_P_24_64_27", (int32_t)VIC::VIC210_10240_4320_P_24_64_27},
    {"VIC211_10240_4320_P_25_64_27", (int32_t)VIC::VIC211_10240_4320_P_25_64_27},
    {"VIC212_10240_4320_P_30_64_27", (int32_t)VIC::VIC212_10240_4320_P_30_64_27},
    {"VIC213_10240_4320_P_48_64_27", (int32_t)VIC::VIC213_10240_4320_P_48_64_27},
    {"VIC214_10240_4320_P_50_64_27", (int32_t)VIC::VIC214_10240_4320_P_50_64_27},
    {"VIC215_10240_4320_P_60_64_27", (int32_t)VIC::VIC215_10240_4320_P_60_64_27},
    {"VIC216_10240_4320_P_100_64_27", (int32_t)VIC::VIC216_10240_4320_P_100_64_27},
    {"VIC217_10240_4320_P_120_64_27", (int32_t)VIC::VIC217_10240_4320_P_120_64_27},

    {"VIC218_4096_2160_P_100_256_135", (int32_t)VIC::VIC218_4096_2160_P_100_256_135},
    {"VIC219_4096_2160_P_120_256_135", (int32_t)VIC::VIC219_4096_2160_P_120_256_135},

    {NULL, -1}
};

std::string hdmiinputVecIntToString(std::vector<int32_t>& addrs)
{
    std::string s = "";
    for (size_t i = 0; i < addrs.size(); ++i)
    {
        s += std::to_string(addrs[i]);
        if (i != addrs.size() - 1) s += ", ";
    }
    return s;
}

std::string hdmiinputVecByteToHexString(const std::vector<uint8_t>& data)
{
    std::ostringstream oss;
    oss << std::uppercase << std::hex;  // uppercase hex letters
    for (size_t i = 0; i < data.size(); ++i)
    {
        oss << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
        if (i + 1 < data.size())
            oss << ",";
    }
    return oss.str();
}

/** @} */ // End of HDMI_INPUT_HALTEST_HELPER
/** @} */ // End of HDMI_INPUT_HALTEST
/** @} */ // End of HDMI_INPUT
/** @} */ // End of HPK