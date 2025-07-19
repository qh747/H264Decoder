#include <iostream>
#include <AnnexB/Nalu.h>
#include <AnnexB/Ebsp.h>
#include <AnnexB/Rbsp.h>
#include <AnnexB/NaluSEI.h>
#include <AnnexB/FileReader.h>
using namespace Common;
using namespace AnnexB;

// 测试Nalu SEI解析函数
void FuncTestSEI() {
    FileReader reader("../../resource/720p.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return;
    }

    Nalu nalu;
    bool hasSEI = false;

    while (reader.readNalu(nalu)) {
        if (Nalu_t::SEI != nalu.getNaluType()) {
            continue;
        }

        Ebsp ebsp;
        if (!nalu.getEbsp(ebsp)) {
            std::cerr << "Ebsp read error." << std::endl;
            break;
        }

        Rbsp rbsp;
        if (!ebsp.getRbsp(rbsp)) {
            std::cerr << "Rbsp read error." << std::endl;
            break;
        }

        NaluSEI sei;
        if (!sei.parse(rbsp.getBuffer())) {
            std::cerr << "Nalu SEI read error." << std::endl;
            break;
        }

        hasSEI = true;
        std::cout << nalu.dumpString();
        std::cout << sei.dumpString();

        nalu.clear();
    }

    reader.close();

    if (!hasSEI) {
        std::cout << "Nalu sei not in stream." << std::endl;
    }
}

int main()
{
    // 测试Nalu SEI解析函数
    FuncTestSEI();

    return 0;
}