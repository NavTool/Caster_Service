#注册到supervisor服务中



# 查看是否安装supervisor


# 没有安装则安装


# 查看路径中是否已经存在配置文件，如果有，则替换


# 如果没有，创建一个新的，并填入路径和参数


# 更新配置
sudo supervisorctl reread
sudo supervisorctl update


# 启动监控服务