#include <iomanip>
#include <sstream>
#include <iostream>
#include "Utils/NaluHelper.h"
#include "Utils/StringHelper.h"
#include "AnnexB/NaluStream.h"
#include "AnnexB/NaluSEI.h"
using Common::Nalu_t;
using Utils::NaluHelper;
using Utils::StringHelper;

namespace AnnexB {

bool NaluSEI::parse(const std::vector<uint8_t>& buffer) {
    if (buffer.empty()) {
        std::cerr << "Parse nalu sei error. input param invalid." << std::endl;
        return false;
    }

    NaluStream ns(buffer.data(), buffer.size());
    int readCount = 0;

    // 读取nalu类型
    int head = ns.readNBits(8);
    auto type = static_cast<Nalu_t>(head & 0x1f);
    if (Nalu_t::SEI != type) {
        std::cerr << "Parse nalu sei error. nalu type invalid." << std::endl;
        return false;
    }

    ++readCount;

    // payload_type
    m_seiParam.payload_type = 0;
    while (0xFF == ns.seekNBits(8)) {
        m_seiParam.payload_type += 0xFF;

        ns.readNBits(8);
        ++readCount;
    }

    m_seiParam.payload_type += ns.readNBits(8);
    ++readCount;

    // payload_size
    m_seiParam.payload_size = 0;
    while (0xFF == ns.seekNBits(8)) {
        m_seiParam.payload_size += 0xFF;

        ns.readNBits(8);
        ++readCount;
    }

    m_seiParam.payload_size += ns.readNBits(8);
    ++readCount;

    // payload data
    if (buffer.size() > readCount) {
        m_seiParam.payload_data.assign(buffer.begin() + readCount, buffer.begin() + readCount + static_cast<int>(m_seiParam.payload_size));
    }

    m_isValid = true;
    return true;
}

std::string NaluSEI::dumpString() const {
    if (!m_isValid) {
        return "";
    }

    auto pt = static_cast<Common::NaluSeiPt_t>(m_seiParam.payload_type);

    std::stringstream ss;
    ss << "SEI ========================" << std::endl;
    ss << "payload_type: " << m_seiParam.payload_type << " (" << StringHelper::NaluSeiPtToStr(pt) << ")" << std::endl;
    ss << "payload_size: " << m_seiParam.payload_size << std::endl;

    ss << "payload_data: ";
    for (const auto& b : m_seiParam.payload_data) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b) << " ";
    }
    ss << std::endl;

    return ss.str();
}

} // namespace AnnexB