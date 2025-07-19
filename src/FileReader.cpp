#include <cstring>
#include <iostream>
#include "AnnexB/FileReader.h"

namespace AnnexB {

FileReader::FileReader(const std::string& fileName) : m_fileName(fileName) {
    std::cout << "File reader construct. file: " << fileName << std::endl;
}

FileReader::~FileReader() {
    if (m_file.is_open()) {
        this->close();
    }

    std::cout << "File reader destruct. file: " << m_fileName << std::endl;
}

bool FileReader::open() {
    if (m_file.is_open()) {
        std::cout << "Open file warning. open already. file: " << m_fileName << std::endl;
        return true;
    }

    if (m_fileName.empty()) {
        std::cerr << "Open file error. file name empty." << std::endl;
        return false;
    }

    m_file.open(m_fileName.c_str(), std::ios::in | std::ios::binary);
    if (!m_file.is_open()) {
        std::cerr << "Open file error. call ifstream::open() function failed. file: " << m_fileName << std::endl;
        return false;
    }

    std::cout << "Open file success. file: " << m_fileName << std::endl;
    return true;
}

void FileReader::close() {
    if (!m_file.is_open()) {
        std::cout << "Close file warning. close already. file: " << m_fileName << std::endl;
        return;
    }

    m_file.close();
    std::cout << "Close file success. file: " << m_fileName << std::endl;
}

bool FileReader::readNalu(Nalu& nalu) {
    bool isReachEnd = false;
    while (true) {
        // 从文件中读取数据
        isReachEnd = !this->readFromFile();

        // 查找起始码
        std::size_t startCodeLen = 0;
        if (!FileReader::FindStartCode(m_buffer.data(), m_buffer.size(), startCodeLen)) {
            break;
        }

        nalu.setStartCodeLen(startCodeLen);

        // 查找下一个起始码
        for (auto idx = startCodeLen; idx < m_buffer.size(); ++idx) {
            std::size_t nextStartCodeLen;
            if (FileReader::FindStartCode(m_buffer.data() + idx, m_buffer.size() - idx, nextStartCodeLen)) {
                // 拷贝nalu数据
                nalu.setBuffer(m_buffer.data(), idx);

                // 移除本次拷贝的nalu数据
                m_buffer.erase(m_buffer.begin(), m_buffer.begin() + static_cast<int>(idx));
                return true;
            }
        }

        // 到达文件末尾，拷贝剩余数据
        if (isReachEnd) {
            nalu.setBuffer(m_buffer.data(), m_buffer.size());
            m_buffer.clear();
            return true;
        }
    }

    // 查找失败
    return false;
}

bool FileReader::readFromFile() {
    if (!m_file.is_open()) {
        std::cerr << "Read from file error. file not open. file: " << m_fileName << std::endl;
        return false;
    }

    std::vector<char> tempBuffer(1024);

    m_file.read(tempBuffer.data(), static_cast<long>(tempBuffer.size()));
    std::size_t bytesRead = m_file.gcount();

    if (bytesRead > 0) {
        m_buffer.insert(m_buffer.end(), tempBuffer.begin(), tempBuffer.begin() + static_cast<int>(bytesRead));
        return true;
    }
    else {
        if (m_file.fail() && !m_file.eof()) {
            std::cerr << "Read from file error. err info: " << std::strerror(errno) << std::endl;
        }
        return false;
    }
}

bool FileReader::FindStartCode(const uint8_t* buf, std::size_t len, std::size_t& startCodeLen) {
    if (len < 3) {
        return false;
    }

    if (0x00 == buf[0] && 0x00 == buf[1] && 0x01 == buf[2]) {
        startCodeLen = 3;
        return true;
    }

    if (0x00 == buf[0] && 0x00 == buf[1] && 0x00 == buf[2] && 0x01 == buf[3]) {
        startCodeLen = 4;
        return true;
    }

    return false;
}

} // namespace TestAnnexBFileReader