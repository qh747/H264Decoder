#pragma once
#include "Common/DataDef.h"

namespace Stream { class NaluStream; }; // namespace AnnexB

namespace Utils {

/**
 * @brief Nalu解析帮助类
 */
class NaluHelper {
public:
    // ITU-T H.264 (V15) 7.3.2.1.1.1
    static void ParseScalingList(Stream::NaluStream& ns, int* scalingList, int sizeOfScalingList, int& useDefaultScalingMatrixFlag);

    // ITU-T H.264 (V15) E.1.2 HRD parameters syntax
    static void ParseHrdParam(Stream::NaluStream& ns, Common::SPSVuiHrdParam_dt& hrdParam);

    // ITU-T H.264 (V15) E.1.1 VUI parameters syntax
    static void ParseVuiParam(Stream::NaluStream& ns, Common::SPSVuiParam_dt& vuiParam);

public:
    // ITU-T H.264 (V15) 7.4.3 Slice header semantics
    static bool GetSliceType(uint32_t value, Common::Nalu_t type, uint32_t maxNumRefFrames, Common::Slice_t& ret);

    // ITU-T H.264 (V15) 7.3.3.1 Reference picture list modification syntax
    static void ParseRefPicListModification(Stream::NaluStream& ns, Common::Slice_t slice, Common::SliceHeadParam_dt& sliceHeader);

    // ITU-T H.264 (V15) 7.3.3.2 Prediction weight table syntax
    static void ParsePredWeightTable(Stream::NaluStream& ns, Common::Slice_t slice, uint32_t chromaArrayType,
        Common::SliceHeadParam_dt& sliceHeader);

    // ITU-T H.264 (V15) 7.3.3.3 Decoded reference picture marking syntax
    static void ParseDecRefPisMark(Stream::NaluStream& ns, Common::Nalu_t type, Common::SliceHeadParam_dt& sliceHeader);
};

}; // namespace Utils