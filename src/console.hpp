#pragma once

#include <deque>
#include <filesystem>
#include <fstream>
#include <string>

namespace Game {

struct Console {
    static inline const int m_maxConsoleLines = 10;

    size_t m_currentFileSize = 0;
    size_t m_logFileLastPos = 0;
    int m_scrollOffset = 0;
    int m_maxScrollOffset = 0;
    bool m_isConsoleActive = false;

    std::deque<std::string> m_consoleLines;
    std::filesystem::path m_logFilePath;
    std::ifstream m_logFileStream;

    Console(std::filesystem::path& logFilePath);
    ~Console();

    void changeState();
    void update();

    void openLogFileStream();
    void closeLogFileStream();

    void getLastNLines(size_t n);
    void readNewLogEntries();

    void render();
};

} // namespace Game
