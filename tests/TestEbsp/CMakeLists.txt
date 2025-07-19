# 设置测试程序名称
set(TEST_NAME TestEbsp)

# 添加测试程序
add_executable(${TEST_NAME} TestEbsp.cpp)

# 添加依赖
if (OPTION_BUILD_SHARED_LIB)
    add_dependencies(${TEST_NAME} ${LIB_SHARED})
else()
    add_dependencies(${TEST_NAME} ${LIB_STATIC})
endif()

# 链接库
target_link_directories(${TEST_NAME} PRIVATE ${LIB_PATH})
target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME})
target_include_directories(${TEST_NAME} PRIVATE ${LIB_INCLUDE_PATH})