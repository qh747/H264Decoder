#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Common/DataDef.h"
namespace AnnexB {

/**
 * @brief Nalu SEI类
 */
class NaluSEI {
public:
    /**
     * @brief  解析SEI
     * @return 解析结果
     * @param  buffer SEI数据
     */
    bool parse(const std::vector<uint8_t>& buffer);

    /**
     * @brief  输出SEI数据
     * @return SEI数据
     */
    std::string dumpString() const;

public:
    // 判断数据单元是否解析成功
    bool m_isValid { false };

    // sei参数
    Common::SEIParam_dt m_seiParam;
};

}; // namespace AnnexB