#include "logger.hpp"

#include "spdlog/spdlog.h"
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "raylib-cpp.hpp"

namespace Game::Logger {

static const size_t MAX_TRACELOG_MSG_LENGTH = 256;

class CustomFormatter : public spdlog::formatter {
public:
    std::unique_ptr<spdlog::pattern_formatter> formatter_with_level;
    std::unique_ptr<spdlog::pattern_formatter> formatter;

    CustomFormatter()
    {
        formatter_with_level = std::make_unique<spdlog::pattern_formatter>("[%H:%M:%S] [%l] %v");
        formatter = std::make_unique<spdlog::pattern_formatter>("[%H:%M:%S] %v");
    }

    ~CustomFormatter() { }

    void format(const spdlog::details::log_msg& msg, spdlog::memory_buf_t& dest) override
    {
        // If log level is greater than INFO, include log level in pattern
        if (msg.level > spdlog::level::info) {
            formatter_with_level->format(msg, dest);
        } else {
            formatter->format(msg, dest);
        }
    }

    std::unique_ptr<spdlog::formatter> clone() const override { return std::make_unique<CustomFormatter>(); }
};

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

    for (auto& sink : sinks) {
        sink->set_formatter(std::make_unique<CustomFormatter>());
    }

    auto logger = std::make_shared<spdlog::logger>("multiLoggerSink", sinks.begin(), sinks.end());

    // Register and set the global logger
    spdlog::set_default_logger(logger);
    // spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
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
