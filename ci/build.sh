#检出主仓库代码
#检出子模块代码
#检出rely仓库#检出rely子模块代码
git clone --recurse-submodules https://github.com/NavTool/base_third_relys.git rely

#构建目录
mkdir build
#
cd bulid
#执行CMAKE
cmake ..
#构建
make

