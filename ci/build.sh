
# 安装 Ninja（如果尚未安装）
if ! command -v ninja &> /dev/null; then
    echo "Ninja is not installed. Installing Ninja..."
    # sudo apt-get update
    sudo apt-get install -y ninja-build
fi

#检出rely仓库#检出rely子模块代码
git clone --recurse-submodules https://github.com/NavTool/base_third_relys.git rely

#构建目录
mkdir build
#
cd build
#执行CMAKE
cmake -G Ninja ..
#构建
ninja

