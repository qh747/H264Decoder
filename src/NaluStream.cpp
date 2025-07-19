#include "Stream/NaluStream.h"

namespace Stream {

NaluStream::NaluStream(const uint8_t* buffer, std::size_t size) : BitStream(buffer, size) {

}

/**
 * 无符号哥伦布熵编码流程：以编码数字4为例
 *   1. 将数字4加一得到：5
 *   2. 将数字5转换为二进制形式：101
 *   3. 根据二进制位数填充减一个零位(二进制位数为3，补充(3 - 1 = 2)个零位)：00 101
 *   4. 数字4的无符号哥伦布熵编码结果为：00101
 *
 * 解码流程：以解码00101为例
 *   1. 读取零位，判断实际要解码数字的位数：零位为2，解码数字位数为2 + 1 = 3
 *   2. 读取数字位，得到：101
 *   3. 将二进制数字转为十进制数字：5
 *   4. 将数字5减一得到：4
 */
uint32_t NaluStream::readUev() {
    // 读取零位，实际判断不为零位时已经读取了一个数据位
    std::size_t zeroPosCount = 0;
    while (0 == this->readOneBit() && !this->isEnd() && zeroPosCount < 32) {
        ++zeroPosCount;
    }

    // 读取数据位
    int ret = this->readNBits(zeroPosCount);
    ret |= 1 << zeroPosCount;

    // 读取数据减一得到原始数据
    return ret - 1;
}

/**
 * 有符号哥伦布熵编码流程：以编码数字4为例
 *   1. 对编码的数字取绝对值得到：4
 *   2. 将数字4转换为二进制形式：100
 *   3. 在二进制数字后增加符号位，正数为0，负数为1，得到：100 0
 *   4. 根据二进制位数填充减一个零位(二进制位数为3，补充(3 - 1 = 2)个零位)：000 100 0
 *
 * 解码流程：以解码000 100 0为例
 *   1. 读取零位，判断实际要解码数字的位数：零位为3，解码数字位数为3 + 1 = 4
 *   2. 读取数字位，得到：100 0
 *   3. 读取数字的数据位100，得到：4
 *   4. 读取数字的符号位0：+
 */
int NaluStream::readSev() {
    // 读取零位，实际判断不为零位时已经读取了一个数据位
    std::size_t zeroPosCount = 0;
    while (0 == this->readOneBit() && !this->isEnd() && zeroPosCount < 32) {
        ++zeroPosCount;
    }

    // 读取数据位
    int ret = this->readNBits(zeroPosCount);
    ret |= 1 << zeroPosCount;

    // 读取符号位
    int sign = ret & 0x01;

    // 读取有效数据位
    ret = ret >> 1;
    return sign == 0 ? ret : -ret;
}

} // namespace Stream