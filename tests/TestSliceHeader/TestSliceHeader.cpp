#include <iostream>
#include <AnnexB/Nalu.h>
#include <AnnexB/Ebsp.h>
#include <AnnexB/Rbsp.h>
#include <AnnexB/NaluSPS.h>
#include <AnnexB/NaluPPS.h>
#include <AnnexB/NaluSliceHeader.h>
#include <AnnexB/FileReader.h>
using namespace Common;
using namespace AnnexB;

// 测试Nalu Slice Header解析函数
void FuncTestSliceHeader() {
    FileReader reader("../../resource/720p.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return;
    }

    Nalu nalu;
    NaluSPS sps;
    NaluPPS pps;

    while (reader.readNalu(nalu)) {
        auto type = nalu.getNaluType();

        Ebsp ebsp;
        if (!nalu.getEbsp(ebsp)) {
            continue;
        }

        Rbsp rbsp;
        if (!ebsp.getRbsp(rbsp)) {
            continue;
        }

        if (Nalu_t::SPS == type) {
            sps.parse(rbsp.getBuffer());
        }
        else if (Nalu_t::PPS == type) {
            pps.parse(rbsp.getBuffer(), sps);
        }
        else if (Nalu_t::IDRSlice == type || Nalu_t::NonIDRSlice == type) {
            NaluSliceHeader sliceHeader;
            sliceHeader.parse(rbsp.getBuffer(), sps, pps);

            std::cout << nalu.dumpString();
            std::cout << sliceHeader.dumpString();
        }

        nalu.clear();
    }

    reader.close();
}

int main()
{
    // 测试Nalu Slice Header解析函数
    FuncTestSliceHeader();

    return 0;
}