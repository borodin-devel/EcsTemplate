#include "logger.hpp"

#include "spdlog/spdlog.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "raylib-cpp.hpp"

namespace Game::Logger {

static const size_t MAX_TRACELOG_MSG_LENGTH = 256;

std::filesystem::path setupLogger(const std::filesystem::path& cwd)
{
    auto logsDirectory = cwd / "logs";
    if (!std::filesystem::exists(logsDirectory)) {
        std::filesystem::create_directories(logsDirectory);
    }

    auto logPath = logsDirectory / "logfile.log";
    auto logFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath.string(), true);

    std::vector<spdlog::sink_ptr> sinks { logFileSink };
#ifdef BUILD_TYPE_DEBUG
    auto terminalSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sinks.emplace_back(terminalSink);
#endif
    auto logger = std::make_shared<spdlog::logger>("multiLoggerSink", sinks.begin(), sinks.end());

    // Register and set the global logger
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
    spdlog::set_level(spdlog::level::info); // Set log level
    spdlog::flush_on(spdlog::level::info); // Flush immediately on info level

    return logPath;
}

void customRaylibLogHandler(int logLevel, const char* text, va_list args)
{
    char message[MAX_TRACELOG_MSG_LENGTH];
    vsnprintf(message, sizeof(message), text, args);

    // Now, log the message using spdlog, mapping the logLevel appropriately
    switch (logLevel) {
    case TraceLogLevel::LOG_TRACE:
        spdlog::trace("{}", message);
        break;
    case TraceLogLevel::LOG_DEBUG:
        spdlog::debug("{}", message);
        break;
    case TraceLogLevel::LOG_INFO:
        spdlog::info("{}", message);
        break;
    case TraceLogLevel::LOG_WARNING:
        spdlog::warn("{}", message);
        break;
    case TraceLogLevel::LOG_ERROR:
        spdlog::error("{}", message);
        break;
    case TraceLogLevel::LOG_FATAL:
        spdlog::critical("{}", message);
        break;
    default:
        spdlog::info("{}", message);
        break;
    }
}

} // namespace Game::Logger
