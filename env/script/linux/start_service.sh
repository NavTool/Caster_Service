#!/bin/bash

# 获取当前脚本目录
EXECUTABLE_DIR=$(dirname "$(readlink -f "$0")")

# 固定的可执行程序名称
EXECUTABLE_NAME="Caster_Service"

# Supervisor注册的名称
SUPERVISOR_NAME="CASTER_SERVICE"

# Redis 配置文件路径
REDIS_CONFIG="$EXECUTABLE_DIR/env/redis.conf"  # 修改为你的 Redis 配置文件路径


# 配置文件路径
CONFIG_FILE="/etc/supervisor/conf.d/$SUPERVISOR_NAME.conf"

# 守护进程的完整路径
COMMAND="$EXECUTABLE_DIR/$EXECUTABLE_NAME"


echo "执行安装服务及运行环境脚本..."


# 检测系统包管理器并安装 Supervisor
install_supervisor() {
    if which apt >/dev/null 2>&1; then
        echo "正在通过 apt 安装 Supervisor..."
        sudo apt update
        sudo apt install -y supervisor
    elif which yum >/dev/null 2>&1; then
        echo "正在通过 yum 安装 Supervisor..."
        sudo yum install -y supervisor
    else
        echo "无法识别系统的包管理器，请手动安装 Supervisor。"
        exit 1
    fi
}

install_redis() {
    if which apt >/dev/null 2>&1; then
        echo "正在通过 apt 安装 redis-server..."
        sudo apt update
        sudo apt install -y redis-server
    elif which yum >/dev/null 2>&1; then
        echo "正在通过 yum 安装 redis-server..."
        sudo yum install -y redis
    else
        echo "无法识别系统的包管理器，请手动安装 redis-server。"
        exit 1
    fi
}


# 检查 Supervisor 是否已安装
if ! which supervisorctl >/dev/null 2>&1; then
    echo "Supervisor 未安装，正在安装..."
    install_supervisor
else
    echo "Supervisor 已安装。"
fi

# 检查 redis-server 是否已安装
if ! which redis-server >/dev/null 2>&1; then
    echo "Redis-server 未安装，正在安装..."
    install_redis
else
    echo "Redis-server 已安装。"
fi


echo "创建 Supervisor 的配置文件"

# 检查文件是否存在，存在则覆盖
if [ -f "$CONFIG_FILE" ]; then
    echo "配置文件已存在，将覆盖 $CONFIG_FILE"
else
    echo "创建新的配置文件: $CONFIG_FILE"
fi

# 创建或覆盖 Supervisor 配置文件
cat <<EOL > $CONFIG_FILE
[program:$SUPERVISOR_NAME]
command=$COMMAND         ; 守护进程的命令路径
directory=$EXECUTABLE_DIR    ;
startsecs=3
startretries=3
autostart=true           ; 启动时自动运行
autorestart=true         ; 进程退出时自动重启
stderr_logfile=$EXECUTABLE_DIR/KORO_CASTER.err.log  ; 错误日志文件路径
stdout_logfile=$EXECUTABLE_DIR/KORO_CASTER.out.log  ; 标准输出日志文件路径
stderr_logfile_maxbytes=2MB
stdout_logfile_maxbytes=2MB
user=$USER             ; 指定运行用户

EOL

# 提示操作完成
echo "Supervisor配置文件已生成或覆盖: $CONFIG_FILE"

# 重载 Supervisor 配置
echo "重新加载 Supervisor 配置..."
sudo supervisorctl reread
sudo supervisorctl update

# echo "启动服务："

# # 检查 redis-server 是否在运行
# REDIS_PID=$(pgrep -x "redis-server")
# if [ -n "$REDIS_PID" ]; then
#     # 获取 redis-server 的启动命令行参数
#     REDIS_COMMAND=$(ps -p $REDIS_PID -o args=)

#     if [[ "$REDIS_COMMAND" == *"$REDIS_CONFIG"* ]]; then
#         echo "Redis-server 已经在按照配置文件 $REDIS_CONFIG 运行，无需重启。"
#     else
#         echo "Redis-server 正在运行，但未使用配置文件 $REDIS_CONFIG，准备停止..."
#         sudo systemctl stop redis-server
#         echo "以指定配置文件启动 redis-server..."
#         redis-server $REDIS_CONFIG &
#     fi
# else
#     echo "Redis-server 未在运行，使用指定配置文件启动..."
#     redis-server $REDIS_CONFIG &
# fi

# echo "启动 Caster_Service..."
# sudo supervisorctl start CASTER_SERVICE"


# 提示如何启动和关闭服务
echo "环境配置完成!"
echo "服务管理提示："
echo "启动Caster服务需要先启动Redis-Server"
echo "建议采用配置文件的方式重新启动服务(先关闭Redis-Server,再启动Redis-Server),指令如下"
echo "要启动Redis-Server,请运行: redis-server $REDIS_CONFIG >/dev/null 2>&1"
echo "要关闭Redis-Server,请运行: systemctl stop redis-server"
echo "要启动Caster服务,请运行: sudo supervisorctl start CASTER_SERVICE"
echo "要停止Caster服务,请运行: sudo supervisorctl stop CASTER_SERVICE"


