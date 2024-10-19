#pragma once

#include <stdarg.h>

void customLogHandler(int logLevel, const char* text, va_list args);
