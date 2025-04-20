#include "video_scanner/logger.h"
#include <filesystem>
#include <iostream>

namespace video_scanner {

std::shared_ptr<spdlog::logger> Logger::logger_;

void Logger::init(const std::string& log_file, spdlog::level::level_enum level, std::chrono::seconds flush_interval) {
    try {
        // 如果 logger 已经初始化，直接返回
        if (logger_) {
            LOG_WARN("Logger is already initialized. Skipping reinitialization.");
            return;
        }

        // 创建控制台 sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // 创建 sink 列表
        std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks = {console_sink};

        // 如果 log_file 不为空，创建文件 sink
        if (!log_file.empty()) {
            std::filesystem::path log_path(log_file);
            if (log_path.has_parent_path()) {
                std::filesystem::create_directories(log_path.parent_path());
            }
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file, true);
            sinks.push_back(file_sink);
        }

        // 创建 logger 并使用多个 sink
        logger_ = std::make_shared<spdlog::logger>("video_scanner", sinks.begin(), sinks.end());

        // 设置日志格式: [时间] [级别] [线程] 消息
        logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");

        // 设置全局日志级别
        logger_->set_level(level);

        // 注册 logger
        spdlog::register_logger(logger_);

        // 设置日志刷新间隔
        if (flush_interval.count() > 0) {
            spdlog::flush_every(flush_interval);
        }

        LOG_INFO("Logger initialized successfully");
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        throw;
    }
}

std::shared_ptr<spdlog::logger>& Logger::get() {
    if (!logger_) {
        throw std::runtime_error("Logger not initialized. Call Logger::init() first.");
    }
    return logger_;
}

} // namespace video_scanner