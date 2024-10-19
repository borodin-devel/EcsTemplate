#pragma once

#include <filesystem>

#include <stdarg.h>

namespace Game::Logger {

std::filesystem::path setupLogger(const std::filesystem::path& cwd);
void customRaylibLogHandler(int logLevel, const char* text, va_list args);

} // namespace Game::Logger
