#!/bin/bash


#构建主程序
mkdir build
#
cd build
#执行CMAKE
cmake ..
#构建
make -j$(nproc)
#切换回主目录
cd ..


#构建依赖环境
cd env/redis-7.2.1 && make MALLOC=libc -j$(nproc)
cd ../..
mkdir bin/env -p
cp env/redis-7.2.1/src/redis-server bin/env/redis-server
cp .cmake/redis_inux.conf.in    bin/env/redis.config

#复制部署脚本
mkdir bin/script -p
cp -r env/script/linux/ bin/scripts/
