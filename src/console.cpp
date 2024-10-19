#include "console.hpp"

#include "spdlog/spdlog.h"

#include "raylib-cpp.hpp"

#include <algorithm>

namespace Game {

Console::Console(std::filesystem::path& logFilePath)
    : m_logFilePath(logFilePath)
{
    openLogFileStream();
}

Console::~Console() { closeLogFileStream(); }

void Console::changeState()
{
    m_isConsoleActive = !m_isConsoleActive;

    if (m_isConsoleActive && m_logFileStream.is_open()) {
        m_logFileStream.clear(); // Clear any fail/eof state
        m_logFileStream.seekg(0, std::ios::end);
        m_logFileLastPos = m_logFileStream.tellg();
    } else if (m_isConsoleActive) {
        spdlog::error("Log file {} wasn't open by console", m_logFilePath.string());
    }
}

void Console::update()
{
    // If the console is open, check for new log entries
    if (m_isConsoleActive) {
        readNewLogEntries();
    }
}

// Open the log file stream at the current end for reading
void Console::openLogFileStream()
{
    m_logFileStream.open(m_logFilePath);

    if (m_logFileStream.is_open()) {
        m_logFileStream.clear(); // Clear any fail/eof state
        m_logFileStream.seekg(0, std::ios::end); // Start reading from the end of the file
        m_logFileLastPos = m_logFileStream.tellg(); // Record the current file position
    } else {
        spdlog::error("Log file {} can't be open by console", m_logFilePath.string());
    }
}

// Close the log file stream
void Console::closeLogFileStream()
{
    if (m_logFileStream.is_open()) {
        m_logFileStream.close();
    } else {
        spdlog::error("Log file {} wasn't open by console", m_logFilePath.string());
    }
}

void Console::getLastNLines(size_t n)
{
    static const size_t BUFFER_SIZE = 8192; // Adjust the buffer size as needed
    std::string buffer;
    std::string leftover;
    size_t totalLinesRead = 0;

    m_logFileStream.seekg(0, std::ios::end); // Move to the end of the file
    std::streamoff filePos = m_logFileStream.tellg();
    m_consoleLines.clear();

    while (filePos > 0 && totalLinesRead < n) {
        size_t toRead = std::min(BUFFER_SIZE, static_cast<size_t>(filePos));
        filePos -= toRead;
        m_logFileStream.seekg(filePos);

        buffer.resize(toRead);
        m_logFileStream.read(&buffer[0], toRead);

        std::string data = buffer + leftover; // Append any leftover from previous read

        // Process data from the end
        size_t pos = data.size();
        while (pos > 0 && totalLinesRead < n) {
            size_t newPos = data.rfind('\n', pos - 1);
            std::string line;
            if (newPos != std::string::npos) {
                line = data.substr(newPos + 1, pos - newPos - 1);
                pos = newPos;
            } else {
                // No more newline characters; take the remaining data as the first line
                line = data.substr(0, pos);
                pos = 0;
            }

            // Avoid adding empty lines due to consecutive newline characters
            if (!line.empty()) {
                m_consoleLines.push_front(line);
                totalLinesRead++;
            }
        }

        leftover = data.substr(0, pos); // Store any data before the first '\n' for the next iteration
    }

    // If there's leftover data after reading the entire file, process it
    if (!leftover.empty() && totalLinesRead < n) {
        m_consoleLines.push_front(leftover);
    }

    // Ensure we have at most 'n' lines
    while (m_consoleLines.size() > n) {
        m_consoleLines.pop_front();
    }
}

// Function to read new lines from the open file stream
void Console::readNewLogEntries()
{
    if (m_logFileStream.is_open()) {
        // Reset the stream state and position
        m_logFileStream.clear();
        m_logFileStream.seekg(0, std::ios::end);

        getLastNLines(10);

        // Set the last read position at the end of the file
        m_logFileStream.clear();
        m_logFileStream.seekg(0, std::ios::end);
        m_logFileLastPos = m_logFileStream.tellg();
    } else {
        spdlog::error("Can't read new entries from log file {} by console", m_logFilePath.string());
    }
}

// Function to draw the console
void Console::render()
{
    if (m_isConsoleActive) {
        DrawRectangle(0, 0, GetScreenWidth(), 200, BLACK);
        int y = 5;
        for (const auto& line : m_consoleLines) {
            DrawText(line.c_str(), 5, y, 20, RAYWHITE);
            y += 20;
        }
    }
}

} // namespace Game