#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Common/DataDef.h"
#include "AnnexB/Ebsp.h"

namespace AnnexB {

/**
 * @brief NALU类
 * @note  NALU数据格式一
 * +---------------+---------------+---------------+---------------+
 * | 1             | 2             | 3             | 4             |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                 Start Code (0x 00 00 00 01)                   |
 * +---------------+---------------+---------------+---------------+
 * |F|NRI|  Type   |               Nalu Body                       |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+---------------+---------------+
 *
 * @note  NALU数据格式二
 * +---------------+---------------+---------------+---------------+
 * | 1             | 2             | 3             | 4             |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |            Start Code (0x 00 00 01)           |F|NRI|  Type   |
 * +---------------+---------------+---------------+---------------+
 * |                           Nalu Body                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+---------------+---------------+
 *
 */
class Nalu {
public:
    /**
     * @brief 设置起始码长度
     * @param startCodeLen 起始码长度
     */
    void setStartCodeLen(std::size_t startCodeLen);

    /**
     * @brief 设置nalu数据
     * @param buffer nalu数据
     * @param bufferLen nalu数据长度
     */
    void setBuffer(const uint8_t* buffer, size_t bufferLen);

    /**
     * @brief 清空nalu数据
     */
    void clear();

public:
    /**
     * @brief  判断nalu数据是否有效
     * @return 是否有效
     */
    bool isValid() const;

    /**
     * @brief  获取nalu数据首部
     * @return nalu数据首部
     */
    uint8_t getHead() const;

    /**
     * @brief  获取ebsp数据
     * @return 获取结果
     * @param  ebsp ebsp数据
     */
    bool getEbsp(Ebsp& ebsp) const;

    /**
     * @brief  获取nalu类型
     * @return nalu类型
     */
    Common::Nalu_t getNaluType() const;

    /**
     * @brief  获取起始码长度
     * @return 起始码长度
     */
    std::size_t getStartCodeLen() const;

    /**
     * @brief  输出nalu数据
     * @return nalu数据
     */
    std::string dumpString() const;

private:
    // 起始码长度
    std::size_t m_startCodeLen { 0 };

    // nalu数据
    std::vector<uint8_t> m_buffer;
};

}; // namespace TestAnnexBFileReader