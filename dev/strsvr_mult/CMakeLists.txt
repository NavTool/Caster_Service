cmake_minimum_required(VERSION 3.5)

project(strsvr_mult LANGUAGES CXX)

#版本14以下可能会导致abseil不支持
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#包含的源文件和头文件
# include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/include)
# include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src)

set(MAIN_SOURCES
   main.cpp
)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES})

target_link_libraries(${PROJECT_NAME} rtklib)


