#include <cstdint>
#include <iostream>
#include <Stream/NaluStream.h>
using namespace Stream;

// 测试NaluStream类
void FuncTestNaluStream() {
    {
        // 测试数据，0x67的二进制为：0110 0111 -> 011 00111
        uint8_t buffer[] = {0x67};
        std::size_t size = sizeof(buffer) / sizeof(uint8_t);

        // 测试读取无符号哥伦布熵编码
        NaluStream ns(buffer, size);
        std::cout << "readUe(v): " << ns.readUev() << std::endl;
        std::cout << "readUe(v): " << ns.readUev() << std::endl;
    }

    {
        // 测试数据，0x67的二进制为：0110 0111 -> 011 00111
        uint8_t buffer[] = {0x67};
        std::size_t size = sizeof(buffer) / sizeof(uint8_t);

        // 测试读取有符号哥伦布熵编码
        NaluStream ns(buffer, size);
        std::cout << "readSe(v): "  << ns.readSev() << std::endl;
        std::cout << "readSe(v): "  << ns.readSev() << std::endl;
    }
}

int main()
{
    // 测试NaluStream类
    FuncTestNaluStream();

    return 0;
}