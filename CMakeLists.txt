cmake_minimum_required(VERSION 3.16)

project(Caster_Project)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# 设置使用的 C++ 标准版本为 C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  #要求编译器支持所选的标准
set(CMAKE_CXX_EXTENSIONS ON)  #启用编译器的扩展特性

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
add_definitions(-DLINUX)
ENDIF()
IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
add_definitions(-DWIN32)
ENDIF()

# #全局设置选项
# set(BUILD_EXAMPLES OFF)
# set(BUILD_TESTING OFF)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    #是主项目
    set(JSON_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/rely/json/include")
    set(SPDLOG_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/rely/spdlog/include")
else()
    #是子项目
endif()



add_subdirectory(rely/)
add_subdirectory(lib/)
add_subdirectory(app/)
add_subdirectory(dev/)










