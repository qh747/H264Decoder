#include <iomanip>
#include <sstream>
#include "AnnexB/Rbsp.h"

namespace AnnexB {

void Rbsp::setBuffer(const uint8_t* buffer, size_t bufferLen) {
    m_buffer.assign(buffer, buffer + bufferLen);
}

void Rbsp::clear() {
    m_buffer.clear();
}

bool Rbsp::isValid() const {
    return !m_buffer.empty();
}

const std::vector<uint8_t>& Rbsp::getBuffer() const {
    return m_buffer;
}

std::string Rbsp::dumpString() const {
    if (!this->isValid()) {
        return "";
    }

    std::stringstream ss;
    ss << "RBSP ========================" << std::endl;
    ss << "len: " << m_buffer.size() << std::endl;

    ss << "RBSP BODY: ------------------" << std::endl;
    for (const auto& val : m_buffer) {
        ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(val) << " ";
    }
    ss << std::endl;

    return ss.str();
}

} // namespace AnnexB