#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Common/DataDef.h"

namespace AnnexB {

/**
 * @brief Nalu SPS类
 */
class NaluSPS {
public:
    /**
     * @brief  解析SPS
     * @return 解析结果
     * @param  buffer SPS数据
     */
    bool parse(const std::vector<uint8_t>& buffer);

    /**
     * @brief  输出SPS数据
     * @return SPS数据
     */
    std::string dumpString() const;

    /**
     * @brief  输出SPS分辨率
     * @return SPS分辨率
     */
    std::string dumpResolution() const;

public:
    // sps参数
    Common::SPSParam_dt m_spsParam;
};

}; // namespace AnnexB