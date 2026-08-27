$ErrorActionPreference = "Stop"

function Invoke-NativeCommand {
    param(
        [Parameter(Mandatory = $true)]
        [string]$FilePath,
        [Parameter(Mandatory = $true)]
        [string[]]$Arguments
    )

    & $FilePath @Arguments
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

Invoke-NativeCommand "cmake" @(
    "-S", ".",
    "-B", "build",
    "-DCMAKE_CXX_COMPILER=cl",
    "-DCMAKE_C_COMPILER=cl",
    "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin"
)

Invoke-NativeCommand "cmake" @(
    "--build", "build",
    "--config", "Release",
    "--", "/m"
)

# 移动所有文件和子目录从 build/release 到 build
Move-Item -Path "bin\Release\*" -Destination "bin"  -Force

# 删除 build/release 目录
Remove-Item -Path "bin\Release" -Recurse -Force

# 创建目标目录
New-Item -Path "bin\env" -ItemType Directory -Force
# 创建目标目录
New-Item -Path "bin\env\redis" -ItemType Directory -Force

# 移动目录和文件
Copy-Item -Path "env\Redis-7.4.0-Windows-x64-msys2\*" -Destination "bin\env\redis" -Recurse -Force

# 复制文件，并重命名
Copy-Item -Path ".cmake\redis_windows.conf.in" -Destination "bin\env\redis\redis.conf" -Force

Copy-Item -Path "env\scripts" -Destination "bin\env\scripts" -Recurse -Force
