#pragma once
#include <vector>
#include <cstdint>

namespace Stream {

/**
 * @brief 比特流读取类
 */
class BitStream {
public:
    BitStream(const uint8_t* buffer, std::size_t size);
    virtual ~BitStream() = default;

public:
    /**
     * @brief  读取一个比特位
     * @return 比特位值
     */
    int readOneBit();

    /**
     * @brief  读取n个比特位
     * @return 比特位值
     * @param  count 读取比特位个数
     */
    int readNBits(std::size_t count);

    /**
     * @brief  获取n个比特位
     * @return 比特位值
     * @param  count 读取比特位个数
     */
    int seekNBits(std::size_t count);

public:
    // 获取当前读取位置
    inline std::size_t getPos() const {
        return m_pos;
    }

    // 获取比特位偏移量
    inline std::size_t getBitOffset() const {
        return m_bitOffset;
    }

    // 获取比特流缓冲区大小
    inline std::size_t getSize() const {
        return m_buffer.size();
    }

    // 获取剩余可读比特个数
    inline std::size_t getRemainBitSize() const {
        return (m_buffer.size() * 8) - (m_pos * 8) - (8 - m_bitOffset);
    }

    // 判断是否到达比特流缓冲区末尾
    inline bool isEnd() const {
        return m_pos >= m_buffer.size();
    }

protected:
    // 当前位置
    std::size_t m_pos { 0 };

    // 比特位偏移量
    std::size_t m_bitOffset { 8 };

    // 比特流缓冲区
    const std::vector<uint8_t> m_buffer;
};

}; // namespace Stream