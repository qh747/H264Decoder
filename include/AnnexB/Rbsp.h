#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace AnnexB {

/**
 * @brief Rbsp类
 * @note  Rbsp的数据内容为Ebsp去除防竞争字节序后的数据
 * +---------------+---------------+---------------+---------------+
 * | 1             | 2             | 3             | 4             |
 * +---------------+---------------+---------------+---------------+
 * |F|NRI|  Type   |               Nalu Body                       |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+---------------+---------------+
 *
 */
class Rbsp {
public:
    /**
     * @brief 设置rbsp数据
     * @param buffer rbsp数据
     * @param bufferLen rbsp数据长度
     */
    void setBuffer(const uint8_t* buffer, size_t bufferLen);

    /**
     * @brief 清空rbsp数据
     */
    void clear();

public:
    /**
     * @brief  判断rbsp数据是否有效
     * @return 是否有效
     */
    bool isValid() const;

    /**
     * @brief  获取rbsp数据
     * @return rbsp数据
     */
    const std::vector<uint8_t>& getBuffer() const;

    /**
     * @brief  输出rbsp数据
     * @return rbsp数据
     */
    std::string dumpString() const;

private:
    // 存储RBSP数据
    std::vector<uint8_t> m_buffer;
};

}; // namespace AnnexB