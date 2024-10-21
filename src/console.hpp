#pragma once

#include "raylib-cpp.hpp"

#include <deque>
#include <filesystem>
#include <fstream>
#include <string>

namespace Game {

struct Console {
    static inline const int m_maxConsoleLines = 10;
    static inline const size_t m_bufferSize = 512; // Adjust the buffer size as needed

    std::filesystem::path m_logFilePath;
    std::ifstream m_logFileStream;

    Font m_font;
    float m_fontSize = 18.0f;
    float m_spacing = 1.0f;
    float m_lineSpacing = 5.0f;
    float m_charWidth = 0.0f;
    float m_charHeight = 0.0f;

    Rectangle m_consoleArea;
    std::deque<std::string> m_consoleLines;
    int m_columns = 0;
    int m_lines = 0;
    int m_totalCharacters = 0;

    std::streamoff m_lastFileSize = 0;
    std::streamoff m_currentFileSize = 0;
    std::streamoff m_logFileStreamPosTop = 0;
    std::streamoff m_logFileStreamPosBottom = 0;

    bool m_isActive = false;
    bool m_isUpdated = false;
    bool m_stickyMode = false;

    Console(std::filesystem::path& logFilePath);
    ~Console();

    void changeState();
    void update();

    void openLogFileStream();
    void closeLogFileStream();
    std::streamoff getLogFileSize();

    void moveOneLineUp();
    void moveOneLineDown();

    void getLastNLines(size_t n);
    void readNewLogEntries();

    void render();
};

} // namespace Game
