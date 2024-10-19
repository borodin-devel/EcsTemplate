#pragma once

#include <string>

#ifdef _WIN32

namespace Game::Platform::Windows {

std::string getStackTrace();
void showErrorWindow(const std::string& errorMessage);
std::string utf16ToUtf8(const std::wstring& utf16Str);
std::string getExecutablePath();

} // namespace Game::Platform::Windows

#endif