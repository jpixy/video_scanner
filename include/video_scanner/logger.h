#ifndef VIDEO_SCANNER_LOGGER_H
#define VIDEO_SCANNER_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <memory>
#include <string>
#include <chrono>

namespace video_scanner {

class Logger {
public:
    // 修改 init 函数签名，添加 flush_interval 参数
    static void init(const std::string& log_file = "logs/video_scanner.log", 
                     spdlog::level::level_enum level = spdlog::level::info,
                     std::chrono::seconds flush_interval = std::chrono::seconds(3));
    static std::shared_ptr<spdlog::logger>& get();
    
private:
    static std::shared_ptr<spdlog::logger> logger_;
};

} // namespace video_scanner

// 日志宏定义保持不变
#define LOG_TRACE(...)    video_scanner::Logger::get()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)    video_scanner::Logger::get()->debug(__VA_ARGS__)
#define LOG_INFO(...)     video_scanner::Logger::get()->info(__VA_ARGS__)
#define LOG_WARN(...)     video_scanner::Logger::get()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    video_scanner::Logger::get()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) video_scanner::Logger::get()->critical(__VA_ARGS__)

#endif // VIDEO_SCANNER_LOGGER_H