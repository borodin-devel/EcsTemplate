
#include "spdlog/spdlog.h"

#include "raylib-cpp.hpp"

static const size_t MAX_TRACELOG_MSG_LENGTH = 256;

void customLogHandler(int logLevel, const char* text, va_list args)
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
