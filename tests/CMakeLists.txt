# 设置可执行程序路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/tests)

# 添加测试程序目录
add_subdirectory(TestSPS)
add_subdirectory(TestPPS)
add_subdirectory(TestSEI)
add_subdirectory(TestEbsp)
add_subdirectory(TestRbsp)
add_subdirectory(TestBitStream)
add_subdirectory(TestNaluStream)
add_subdirectory(TestFileReader)