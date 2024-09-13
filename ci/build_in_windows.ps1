
cmake -B build -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin 

cmake --build build --config Release

# 移动所有文件和子目录从 build/release 到 build
Move-Item -Path "bin\Release\*" -Destination "bin"  -Force

# 删除 build/release 目录
Remove-Item -Path "bin\Release" -Recurse -Force


New-Item -Path "bin\env" -ItemType Directory

Move-Item -Path "env\Redis-7.4.0-Windows-x64-msys2\*" -Destination "bin\env"  -Force

Copy-Item -Path ".cmake\redis_windows.conf.in" -Destination "bin\env\redis.conf" -Force


# 创建目标目录
New-Item -Path "bin\env" -ItemType Directory -Force

# 移动目录和文件
Move-Item -Path "env\Redis-7.4.0-Windows-x64-msys2\*" -Destination "bin\env" -Force

# 复制文件，并重命名
Copy-Item -Path ".cmake\redis_windows.conf.in" -Destination "bin\env\redis.conf" -Force
