#include <iostream>
#include <AnnexB/Nalu.h>
#include <AnnexB/Ebsp.h>
#include <AnnexB/Rbsp.h>
#include <AnnexB/NaluSPS.h>
#include <AnnexB/NaluPPS.h>
#include <AnnexB/FileReader.h>
using namespace Common;
using namespace AnnexB;

// 测试Nalu PPS解析函数
void FuncTestPPS() {
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
            if (sps.parse(rbsp.getBuffer())) {
                nalu.clear();

                if (!reader.readNalu(nalu)) {
                    break;
                }

                if (!nalu.getEbsp(ebsp)) {
                    break;
                }

                if (!ebsp.getRbsp(rbsp)) {
                    break;
                }

                NaluPPS pps;
                if (!pps.parse(rbsp.getBuffer(), sps)) {
                    break;
                }

                std::cout << nalu.dumpString();
                std::cout << pps.dumpString();
            }

        } while (false);

        nalu.clear();
    }

    reader.close();
}

int main()
{
    // 测试Nalu PPS解析函数
    FuncTestPPS();

    return 0;
}