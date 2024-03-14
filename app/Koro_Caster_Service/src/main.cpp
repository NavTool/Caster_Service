#include <iostream>
#include <fstream>
#include <memory>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <coroutine>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "version.h"
#include "ntrip_caster.h"

#define CONF_PATH "Koro_Caster_Service_Conf.json"

// trace：最详细的日志级别，提供追踪程序执行流程的信息。
// debug：调试级别的日志信息，用于调试程序逻辑和查找问题。
// info：通知级别的日志信息，提供程序运行时的一般信息。
// warn：警告级别的日志信息，表明可能发生错误或不符合预期的情况。
// error：错误级别的日志信息，表明发生了某些错误或异常情况。
// critical：严重错误级别的日志信息，表示一个致命的或不可恢复的错误。

int main(int argc, char **argv)
{
    // 启用dump
    prctl(PR_SET_DUMPABLE, 1);

    // 设置core dunp大小
    struct rlimit rlimit_core;
    rlimit_core.rlim_cur = RLIM_INFINITY; // 设置大小为无限
    rlimit_core.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &rlimit_core);

    // 程序启动
    spdlog::info("{}/{} Start", PROJECT_SET_NAME, PROJECT_SET_VERSION);
    spdlog::info("Software Version: {}-{}", PROJECT_GIT_VERSION, PROJECT_TAG_VERSION);
    //----------------------功能测试区

    //---------------------------------
    // 解析输入：
    std::string conf_path;
    conf_path = CONF_PATH;
    // if (argc > 1)
    // {
    //     conf_path=argv[1];
    // }
    // else
    // {
    //     conf_path=CONF_PATH;
    // }

    // 打开配置文件
    spdlog::info("Conf Path:{}", conf_path);
    std::ifstream f(conf_path);
    if (!f.is_open())
    {
        spdlog::error("Conf File Open Fail! Exit.");
        return 0;
    }

    // 读取全局配置
    spdlog::info("Load Conf...");
    json cfg = json::parse(f);
    f.close();

    // coredunp 选项
    bool Core_Dump = cfg["Debug_Mode"]["Core_Dump"];

    bool Debug_Info = cfg["Debug_Mode"]["Debug_Info"];
    bool Info_Record = cfg["Debug_Mode"]["Info_Record"];

    bool logstd = true;
    bool logfile = cfg["Log_File_Setting"]["Info_Log"]["Switch"];
    std::string logpath = cfg["Log_File_Setting"]["Info_Log"]["Save_Path"];
    int file_hour = cfg["Log_File_Setting"]["Info_Log"]["File_Hour"];
    int file_min = cfg["Log_File_Setting"]["Info_Log"]["File_Min"];

    if (!Core_Dump) // 是否需要关闭 coredump
    {
        prctl(PR_SET_DUMPABLE, 0);
    }

    // 初始化日志系统
    std::vector<spdlog::sink_ptr> sinks;
    if (logfile) // 输出到文件
    {
        spdlog::info("Write log to File...");
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(logpath, file_hour, file_min));
    }
    if (logstd) // 输出到控制台
    {
        spdlog::info("Write log to Std...");
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(logpath, file_hour, file_min));
    }
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
    // 把所有sink放入logger
    auto logger = std::make_shared<spdlog::logger>("log", begin(sinks), end(sinks));

    spdlog::set_default_logger(logger);
    spdlog::flush_every(std::chrono::seconds(5)); // 设置日志刷新时间
    spdlog::flush_on(spdlog::level::warn);        // 如果遇到警告以上的信息则立即刷新日志，避免丢失记录

    if (Debug_Info) // 设置输出日志的级别
    {
        spdlog::set_level(spdlog::level::debug);
    }
    if (Info_Record)
    {
    }

    // 初始化完成，开始进入正式流程
    spdlog::info("Start Log...");
    spdlog::info("Software: {}/{}", PROJECT_SET_NAME, PROJECT_SET_VERSION);
    spdlog::info("Version : {}-{}", PROJECT_GIT_VERSION, PROJECT_TAG_VERSION);

    spdlog::info("Init Server...");
    ntrip_caster a(cfg); // 创建一个对象，传入config

    spdlog::info("Start Server...");
    a.start(); // 原神，启动！

    return 0;
}