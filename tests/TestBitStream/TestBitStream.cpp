#include <cstdint>
#include <iostream>
#include <Common/BitStream.h>
using namespace Common;

// 测试BitStream类
void FuncTestBitStream() {
    // 测试数据
    uint8_t buffer[] = {0x01, 0x02, 0x03, 0x06};
    std::size_t size = sizeof(buffer) / sizeof(uint8_t);

    // 测试数据正常读取
    BitStream bs(buffer, size);
    std::cout << "readOneByte: " << std::hex << std::showbase << bs.readNBits(8) << std::endl;
    std::cout << "readTwoBytes: " << std::hex << std::showbase << bs.readNBits(16) << std::endl;

    // 测试数据越界读取
    while (!bs.isEnd()) {
        std::cout << "readFiveBits: " << std::hex << std::showbase << bs.readNBits(5) << std::endl;
    }
}

int main()
{
    // 测试BitStream类
    FuncTestBitStream();

    return 0;
}