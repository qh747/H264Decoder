#include <iostream>
#include <AnnexB/Nalu.h>
#include <AnnexB/Ebsp.h>
#include <AnnexB/Rbsp.h>
#include <AnnexB/FileReader.h>
using namespace AnnexB;

// 测试RBSP读取函数
void FuncTestRbsp() {
    FileReader reader("../../resource/demo_video_176x144_baseline.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return;
    }

    Nalu nalu;
    while (reader.readNalu(nalu)) {
        std::cout << nalu.dumpString();

        do {
            Ebsp ebsp;
            if (!nalu.getEbsp(ebsp)) {
                break;
            }
            std::cout << ebsp.dumpString();

            Rbsp rbsp;
            if (!ebsp.getRbsp(rbsp)) {
                break;
            }
            std::cout << rbsp.dumpString() << std::endl;

        } while (false);

        nalu.clear();
    }

    reader.close();
}

int main()
{
    // 测试RBSP读取函数
    FuncTestRbsp();

    return 0;
}