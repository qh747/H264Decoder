#include <iostream>
#include <AnnexB/Nalu.h>
#include <AnnexB/Ebsp.h>
#include <AnnexB/FileReader.h>
using namespace AnnexB;

// 测试EBSP读取函数
void FuncTestEbsp() {
    FileReader reader("../../resource/demo_video_176x144_baseline.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return;
    }

    Nalu nalu;
    while (reader.readNalu(nalu)) {
        std::cout << nalu.dumpString();

        Ebsp ebsp;
        if (nalu.getEbsp(ebsp)) {
            std::cout << ebsp.dumpString() << std::endl;
        }

        nalu.clear();
    }

    reader.close();
}

int main()
{
    // 测试EBSP读取函数
    FuncTestEbsp();

    return 0;
}