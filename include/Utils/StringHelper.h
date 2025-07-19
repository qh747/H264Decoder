#pragma once
#include <string>
#include "Common/DataDef.h"

namespace Utils {

/**
 * @brief 字符串帮助类
 */
class StringHelper {
public:
    // Nalu SEI payload type to string
    static std::string NaluSeiPtToStr(Common::NaluSeiPt_t pt);

    // ITU-T H.264 (V15) E.1.2 HRD parameters syntax
    static Common::NaluSeiPt_t NaluSeiStrToPt(const std::string& str);
};

}; // namespace Utils