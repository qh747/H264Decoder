#pragma once
#include "Common/BitStream.h"

namespace AnnexB {

class NaluStream : public Common::BitStream {
public:
    NaluStream(const uint8_t* buffer, std::size_t size);
    ~NaluStream() override = default;

public:
    /**
     * @brief  读取无符号指数哥伦布编码
     * @return 读取值
     */
    uint32_t readUev();

    /**
     * @brief  读取有符号指数哥伦布编码
     * @return 读取值
     */
    int readSev();
};

}; // namespace AnnexB