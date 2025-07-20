#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Common/DataDef.h"
#include "AnnexB/NaluSPS.h"
#include "AnnexB/NaluPPS.h"

namespace AnnexB {

/**
 * @brief Nalu Slice Header类
 */
class NaluSliceHeader {
public:
    /**
     * @brief  解析slice header
     * @return 解析结果
     * @param  buffer slice header数据
     * @param  sps    SPS数据
     * @param  pps    PPS数据
     */
    bool parse(const std::vector<uint8_t>& buffer, const NaluSPS& sps, const NaluPPS& pps);

    /**
     * @brief  输出slice header数据
     * @return slice header数据
     */
    std::string dumpString() const;

public:
    // 判断数据单元是否解析成功
    bool m_isValid { false };

    // 判断是否为IDR slice
    bool m_isIdrSlice { false };

    // slice header参数
    Common::SliceHeadParam_dt m_headParam;
};

}; // namespace AnnexB