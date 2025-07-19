#pragma once
#include "Common/DataDef.h"

namespace AnnexB { class NaluStream; }; // namespace AnnexB

namespace Utils {

/**
 * @brief Nalu解析帮助类
 */
class NaluHelper {
public:
    // ITU-T H.264 (V15) 7.3.2.1.1.1
    static void ParseScalingList(AnnexB::NaluStream& ns, int* scalingList, int sizeOfScalingList, int* useDefaultScalingMatrixFlag);

    // ITU-T H.264 (V15) E.1.2 HRD parameters syntax
    static void ParseHrdParam(AnnexB::NaluStream& ns, Common::SPSVuiHrdParam_dt& hrdParam);

    // ITU-T H.264 (V15) E.1.1 VUI parameters syntax
    static void ParseVuiParam(AnnexB::NaluStream& ns, Common::SPSVuiParam_dt& vuiParam);
};

}; // namespace Utils