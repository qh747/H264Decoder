#include <iomanip>
#include <sstream>
#include "AnnexB/Ebsp.h"

namespace AnnexB {

void Ebsp::setBuffer(const uint8_t* buffer, size_t bufferLen) {
    m_buffer.assign(buffer, buffer + bufferLen);
}

void Ebsp::clear() {
    m_buffer.clear();
}

bool Ebsp::isValid() const {
    return !m_buffer.empty();
}

bool Ebsp::getRbsp(Rbsp& rbsp) const {
    if (!this->isValid()) {
        return false;
    }

    std::vector<uint8_t> buffer(m_buffer.begin(), m_buffer.begin() + 3);

    /**
     * 去除防竞争字节序：
     * 0x00 0x00 0x03 0x00 -> 0x00 0x00 0x00
     * 0x00 0x00 0x03 0x01 -> 0x00 0x00 0x01
     * 0x00 0x00 0x03 0x02 -> 0x00 0x00 0x02
     * 0x00 0x00 0x03 0x03 -> 0x00 0x00 0x03
     */

    auto size = m_buffer.size();
    for (auto idx = 3; idx < size; ++idx) {
        if (0x03 != m_buffer[idx]) {
            // 当前不为不竞争字节序标识0x03
            buffer.push_back(m_buffer[idx]);
        }
        else if (0x00 != m_buffer[idx - 2] || 0x00 != m_buffer[idx - 1]) {
            // 当前为竞争字节序标识0x03，但前两个字节不为0x00，无法构成防竞争字节序
            buffer.push_back(m_buffer[idx]);
        }
        else if (idx + 1 >= size) {
            // 当前为0x00 0x00 0x03，但0x03为末尾字节，无法构成防竞争字节序
            buffer.push_back(m_buffer[idx]);
        }
        else if (0x00 != m_buffer[idx + 1] && 0x01 != m_buffer[idx + 1] && 0x02 != m_buffer[idx + 1] && 0x03 != m_buffer[idx + 1]) {
            // 当前不满足去除防竞争字节序条件
            buffer.push_back(m_buffer[idx]);
        }
    }

    rbsp.setBuffer(buffer.data(), buffer.size());
    return true;
}

std::string Ebsp::dumpString() const {
    if (!this->isValid()) {
        return "";
    }

    std::stringstream ss;
    ss << "EBSP ========================" << std::endl;
    ss << "len: " << m_buffer.size() << std::endl;

    ss << "EBSP BODY: ------------------" << std::endl;
    for (const auto& val : m_buffer) {
        ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(val) << " ";
    }
    ss << std::endl;

    return ss.str();
}

} // namespace AnnexB