#include "Common/BitStream.h"

namespace Common {

BitStream::BitStream(const uint8_t* buffer, std::size_t size) : m_buffer(buffer, buffer + size) {

}

int BitStream::readOneBit() {
    if (this->isEnd()) {
        return -1;
    }

    // 获取当前比特
    --m_bitOffset;
    int bit = m_buffer[m_pos] >> m_bitOffset;

    // 当前字节的偏移量移动到末尾，进入下一个字节
    if (m_bitOffset <= 0) {
        m_bitOffset = 8;
        ++m_pos;
    }

    return bit & 0x01;
}

int BitStream::readNBits(std::size_t count) {
    // 获取当前可读比特数 = 缓冲区总比特数 - 字节偏移量 - 比特偏移量
    auto remainBites = this->getRemainBitSize();
    count = count <= remainBites ? count : remainBites;

    // 读取当前比特并移动到正确的位置
    int ret = 0;
    for (auto idx = 1; idx <= count; ++idx) {
        ret |= this->readOneBit() << (count - idx);
    }

    return ret;
}

int BitStream::seekNBits(std::size_t count) {
    // 暂存当前读取的位置和比特偏移量
    auto currPos = m_pos;
    auto currBitOffset = m_bitOffset;

    // 读取指定比特数
    int ret = this->readNBits(count);

    // 恢复读取位置和比特偏移量
    m_pos = currPos;
    m_bitOffset = currBitOffset;

    // 返回读取的比特数
    return ret;
}

} // namespace Common