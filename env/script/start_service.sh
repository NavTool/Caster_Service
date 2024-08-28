#!/bin/bash

# 设置变量指向 Redis 服务器和可执行程序
REDIS_PATH="env/redis-server"
APP_PATH="Caster_Service"

# 提取 Redis 和可执行程序的文件名（不含路径）
REDIS_NAME=$(basename "$REDIS_PATH")
APP_NAME=$(basename "$APP_PATH")

echo "运行状态检测脚本启动"

# 主循环，定期检查 Redis 和可执行程序是否在运行
while true; do
    echo "检查 Redis 服务器是否在运行"
    if ! pgrep -f "$REDIS_NAME" > /dev/null; then
        echo "Redis 服务不存在/已停止！停止正在运行的服务..."
        pkill -f "$APP_NAME"
        
        echo "尝试重新启动 Redis 服务..."
        "$REDIS_PATH" &
        sleep 5  # 等待 Redis 启动
        
        echo "重新启动服务..."
        "$APP_PATH" &
    fi

    echo "检查 $APP_NAME 是否在运行"
    if ! pgrep -f "$APP_NAME" > /dev/null; then
        echo "服务已停止！尝试重新启动..."
        "$APP_PATH" &
    fi

    echo "运行状态检测脚本检测完成，等待下一次检测..."
    sleep 10  # 等待 10 秒
done
