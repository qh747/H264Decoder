#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Common/DataDef.h"
#include "AnnexB/NaluSPS.h"
namespace AnnexB {

/**
 * @brief Nalu PPS类
 */
class NaluPPS {
public:
    /**
     * @brief  解析PPS
     * @return 解析结果
     * @param  buffer PPS数据
     * @param  sps    SPS数据
     */
    bool parse(const std::vector<uint8_t>& buffer, const NaluSPS& sps);

    /**
     * @brief  输出PPS数据
     * @return PPS数据
     */
    std::string dumpString() const;

public:
    // 判断数据单元是否解析成功
    bool m_isValid { false };

    // pps参数
    Common::PPSParam_dt m_ppsParam;
};

}; // namespace AnnexB