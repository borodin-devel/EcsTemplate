#include "console.hpp"

#include "spdlog/spdlog.h"

#include <algorithm>

namespace Game {

Console::Console(std::filesystem::path& logFilePath)
    : m_logFilePath(logFilePath)
{
    auto consoleFontPath = std::filesystem::current_path()
        / std::filesystem::canonical("resources/fonts/Fira_Code_v6.2/ttf/FiraCode-Regular.ttf");
    m_font = LoadFont(consoleFontPath.string().c_str());

    openLogFileStream();
}

Console::~Console() { closeLogFileStream(); }

void Console::changeState()
{
    m_isActive = !m_isActive;

    if (m_isActive) {
        m_isUpdated = false;
        m_stickyMode = true;

        m_logFileStream.clear();
        m_logFileStream.seekg(0, std::ios::end);
        m_logFileStreamPosBottom = m_logFileStream.tellg();
    }
}

void Console::update()
{
    // If the console is open, check for new log entries
    if (m_isActive && m_logFileStream.is_open()) {
        auto pressedKey = KEY_NULL;

        if (IsKeyPressed(KEY_PAGE_UP)) {
            pressedKey = KEY_PAGE_UP;

        } else if (IsKeyPressed(KEY_PAGE_DOWN)) {
            pressedKey = KeyboardKey::KEY_PAGE_DOWN;
        }

        m_currentFileSize = getLogFileSize();

        if (m_currentFileSize != m_lastFileSize) {
            m_isUpdated = true;
            m_lastFileSize = m_currentFileSize;
        } else {
            m_isUpdated = false;
        }

        switch (pressedKey) {
        case KEY_PAGE_UP: {
            moveOneLineUp();
            break;
        }
        case KEY_PAGE_DOWN: {
            moveOneLineDown();
            break;
        }
        default:
            break;
        }

        if (m_stickyMode && m_isUpdated) {
            m_logFileStream.clear();
            m_logFileStream.seekg(0, std::ios::end);
            m_logFileStreamPosBottom = m_logFileStream.tellg();
        }

        readNewLogEntries();
    }
}

// Open the log file stream at the current end for reading
void Console::openLogFileStream()
{
    m_logFileStream.open(m_logFilePath);

    if (!m_logFileStream.is_open()) {
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

// Function to get the current file size
std::streamoff Console::getLogFileSize()
{
    m_logFileStream.clear();

    // Save current position
    std::streampos currentPos = m_logFileStream.tellg();

    // Move to the end of the file
    m_logFileStream.seekg(0, std::ios::end);
    std::streamoff fileSize = m_logFileStream.tellg();

    // Restore the file position
    m_logFileStream.seekg(currentPos);

    return fileSize;
}

// Move the file pointer one line up (backward)
void Console::moveOneLineUp() { }

// Move the file pointer one line down (forward)
void Console::moveOneLineDown()
{
    m_logFileStream.seekg(m_logFileStreamPosBottom, std::ios::beg);

    std::string temp;

    // Read and discard the current line
    if (std::getline(m_logFileStream, temp)) {
        return;
    }
}

void Console::getLastNLines(size_t n)
{
    std::string buffer;
    std::string leftover;
    size_t totalLinesRead = 0;

    m_logFileStream.seekg(m_logFileStreamPosBottom, std::ios::beg);
    std::streamoff filePos = m_logFileStreamPosBottom;
    m_consoleLines.clear();

    while (filePos > 0 && totalLinesRead < n) {
        size_t toRead = std::min(m_bufferSize, static_cast<size_t>(filePos));
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
void Console::readNewLogEntries() { getLastNLines(10); }

// Function to draw the console
void Console::render()
{
    if (m_isActive) {
        DrawRectangle(0, 0, GetScreenWidth(), 200, BLACK);
        int y = 1;
        for (const auto& line : m_consoleLines) {
            DrawTextEx(m_font, line.c_str(), Vector2 { 1, static_cast<float>(y) }, 20, 1, RAYWHITE);
            y += 21;
        }
    }
}

} // namespace Game
