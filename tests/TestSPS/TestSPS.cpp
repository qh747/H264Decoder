#include <iostream>
#include <AnnexB/Nalu.h>
#include <AnnexB/Ebsp.h>
#include <AnnexB/Rbsp.h>
#include <AnnexB/NaluSPS.h>
#include <AnnexB/FileReader.h>
using namespace Common;
using namespace AnnexB;

// 测试Nalu SPS解析函数
void FuncTestSPS() {
    FileReader reader("../../resource/720p.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return;
    }

    Nalu nalu;
    while (reader.readNalu(nalu)) {
        do {
            if (Nalu_t::SPS != nalu.getNaluType()) {
                break;
            }

            Ebsp ebsp;
            if (!nalu.getEbsp(ebsp)) {
                break;
            }

            Rbsp rbsp;
            if (!ebsp.getRbsp(rbsp)) {
                break;
            }

            NaluSPS sps;
            if (!sps.parse(rbsp.getBuffer())) {
                break;
            }

            std::cout << nalu.dumpString();
            std::cout << sps.dumpResolution();
            std::cout << sps.dumpString();

        } while (false);

        nalu.clear();
    }

    reader.close();
}

int main()
{
    // 测试Nalu SPS解析函数
    FuncTestSPS();

    return 0;
}