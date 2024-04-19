#include <map>

#include <spdlog/sinks/rotating_file_sink.h>
#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>

#include "logger.h"
#include "config/config.h"

void Logger::init()
{
    if(logger.get()==nullptr ) {
        //读取配置文件
        cfg::Config config = cfg::readConfig();
        std::map<std::string,spdlog::level::level_enum> levelMatchs  = {
            {"trace",spdlog::level::trace},
            {"debug",spdlog::level::debug},
            {"info",spdlog::level::info},
            {"warn",spdlog::level::warn},
            {"err",spdlog::level::err},
            {"critical",spdlog::level::critical},
            {"off",spdlog::level::off}
        };
        //多线程带锁 文件名 单个文件1MB 一共十个文件回滚
        // Rotate files:
        // log.txt -> log.1.txt
        // log.1.txt -> log.2.txt
        // log.2.txt -> log.3.txt
        // log.3.txt -> delete
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_st>(
            "logs/my_log.txt",
            1024*1024 *1,  //1m
            10);  //10个文件夹
        //设置等级 高于就会打印
        file_sink->set_level(levelMatchs[config.getLogLevel()]);
        // 设置输出到标准输出的logger 带有颜色的多线程sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
        console_sink->set_level(levelMatchs[config.getPrintLevel()]);

        //高于file_sink的等级就会记录,高于console_sink就会打印(如warn 不仅会打印还会记录)
        //spdlog::logger logger("global",{file_sink,console_sink});
        auto tlogger = std::make_shared<spdlog::logger>("global",spdlog::sinks_init_list{file_sink, console_sink});
        spdlog::register_logger(tlogger);
        logger = tlogger;
    }
}

