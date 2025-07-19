#pragma once
#include <string>
#include <fstream>
#include "AnnexB/Nalu.h"

namespace AnnexB {

/**
 * @brief H264文件读取类
 */
class FileReader {
public:
    explicit FileReader(const std::string& fileName);
    ~FileReader();

public:
    /**
     * @brief  查找起始码
     * @return 是否成功
     * @param  buf 数据缓冲区
     * @param  len 数据长度
     * @param  startCodeLen 起始码长度
     */
    static bool FindStartCode(const uint8_t* buf, std::size_t len, std::size_t& startCodeLen);

public:
    /**
     * @brief  打开文件
     * @return 是否成功
     */
    bool open();

    /**
     * @brief  关闭文件
     */
    void close();

    /**
     * @brief  读取一帧数据
     * @return 是否成功
     * @param  nalu 帧数据
     */
    bool readNalu(Nalu& nalu);

private:
    /**
     * @brief  从文件读取数据
     * @return 是否成功
     */
    bool readFromFile();

private:
    // 文件流
    std::ifstream m_file;

    // 文件名称
    std::string m_fileName;

    // 文件数据缓冲区
    std::vector<uint8_t> m_buffer;
};

}; // namespace TestAnnexBFileReader