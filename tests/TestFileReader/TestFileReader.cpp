#include <iostream>
#include <AnnexB/Nalu.h>
#include <AnnexB/FileReader.h>
using namespace AnnexB;

// 测试H264文件读取函数
void FuncTestFileReader() {
    FileReader reader("../../resource/demo_video_176x144_baseline.h264");
    if (!reader.open()) {
        std::cerr << "Open file error." << std::endl;
        return;
    }

    Nalu nalu;
    while (reader.readNalu(nalu)) {
        std::cout << nalu.dumpString() << std::endl;
        nalu.clear();
    }

    reader.close();
}

int main()
{
    // 测试H264文件读取函数
    FuncTestFileReader();

    return 0;
}