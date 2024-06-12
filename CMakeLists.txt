cmake_minimum_required(VERSION 3.16)

project(Caster_Project)

# 设置使用的 C++ 标准版本为 C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  #要求编译器支持所选的标准
set(CMAKE_CXX_EXTENSIONS ON)  #启用编译器的扩展特性


IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
add_definitions(-DLINUX)
ENDIF()
IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
add_definitions(-DWIN32)
ENDIF()

set(JSON_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/rely/json/include")
set(SPDLOG_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/rely/spdlog/include")

#全局设置选项
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)

add_subdirectory(lib/)
add_subdirectory(app/)


# add_subdirectory(rtklib-2.4.3-b34)
#libevent option
if (NOT TARGET libevent)
    set(EVENT__LIBRARY_TYPE STATIC)
    set(EVENT__DISABLE_OPENSSL ON)
    set(EVENT__DISABLE_BENCHMARK ON)
    set(EVENT__DISABLE_TESTS ON)
    set(EVENT__DISABLE_REGRESS ON)
    set(EVENT__DISABLE_SAMPLES ON)
    add_subdirectory(rely/libevent)
endif() 

#yaml-cpp option
set(YAML_BUILD_SHARED_LIBS OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(rely/yaml-cpp)
# add_subdirectory(libssh2-libssh2-1.11.0/)











