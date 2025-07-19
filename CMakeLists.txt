cmake_minimum_required(VERSION 3.10)
project(H264Decoder)

# 设置编译选项
option(OPTION_BUILD_SHARED_LIB "Build Shared Library" OFF)
option(OPTION_BUILD_STATIC_LIB "Build Static Library" ON)
option(OPTION_BUILD_TESTS "Build Tests" ON)

# 设置编译器
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置debug
set(CMAKE_BUILD_TYPE Debug)

# 设置库名称
set(LIB_NAME ${PROJECT_NAME})
set(LIB_STATIC "${PROJECT_NAME}Static")
set(LIB_SHARED "${PROJECT_NAME}Shared")

# 设置库路径和头文件路径
set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)
set(LIB_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)

# 库输出路径
set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

# 参与编译的文件
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILES)

# 生成动态库/静态库
if (OPTION_BUILD_SHARED_LIB AND OPTION_BUILD_STATIC_LIB)
    message("Enable Shared Library And Static Library")

    add_library(${LIB_SHARED} SHARED ${SRC_FILES})
    set_target_properties(${LIB_SHARED} PROPERTIES OUTPUT_NAME ${LIB_NAME})
    target_include_directories(${LIB_SHARED} PUBLIC ${LIB_INCLUDE_PATH})
    
    add_library(${LIB_STATIC} STATIC ${SRC_FILES})
    set_target_properties(${LIB_STATIC} PROPERTIES OUTPUT_NAME ${LIB_NAME})
    target_include_directories(${LIB_STATIC} PUBLIC ${LIB_INCLUDE_PATH})
elseif (OPTION_BUILD_SHARED_LIB)
    message("Enable Shared Library")
    
    add_library(${LIB_SHARED} SHARED ${SRC_FILES})
    set_target_properties(${LIB_SHARED} PROPERTIES OUTPUT_NAME ${LIB_NAME})
    target_include_directories(${LIB_SHARED} PUBLIC ${LIB_INCLUDE_PATH})
else()
    message("Enable Static Library")

    add_library(${LIB_STATIC} STATIC ${SRC_FILES})
    set_target_properties(${LIB_STATIC} PROPERTIES OUTPUT_NAME ${LIB_NAME})
    target_include_directories(${LIB_STATIC} PUBLIC ${LIB_INCLUDE_PATH})
endif ()

# 添加测试程序
if (OPTION_BUILD_TESTS)
    message("Enable Tests")
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
else()
    message("Disable Tests")
endif ()