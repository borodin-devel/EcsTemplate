#include "platform_windows.hpp"

#include <iostream>

#ifdef _WIN32
    #if !defined(UNICODE) || !defined(_UNICODE)
        #error "UNICODE or _UNICODE is not defined! Please enable Unicode support."
    #endif

    #include <windows.h>

    #include <dbghelp.h>
#else
    #error "This platform is not supported!"
#endif

#ifdef _WIN32

namespace Game::Platform::Windows {

// Function to capture and return the stack trace
std::string getStackTrace()
{
    // Initialize symbol handler
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    // Capture stack backtrace
    void* stack[100];
    USHORT frames = CaptureStackBackTrace(0, 100, stack, NULL);

    std::string stackTrace;
    for (USHORT i = 0; i < frames; ++i) {
        DWORD64 address = (DWORD64)(stack[i]);

        // Allocate memory for symbol information
        SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        DWORD64 displacement = 0;
        if (SymFromAddr(process, address, &displacement, symbol)) {
            stackTrace += std::to_string(frames - i - 1) + ": " + symbol->Name + " - 0x"
                + std::to_string(symbol->Address) + "\n";
        } else {
            stackTrace += std::to_string(frames - i - 1) + ": [Unknown Symbol] - 0x" + std::to_string(address) + "\n";
        }

        free(symbol);
    }

    SymCleanup(process);
    return stackTrace;
}

// Function to display the error and stack trace in a message box
void showErrorWindow(const std::string& errorMessage)
{
    std::string message = "An unhandled exception occurred:\n\n" + errorMessage + "\n\nCall Stack:\n" + getStackTrace();
    MessageBoxA(NULL, message.c_str(), "Error: Unhandled Exception", MB_ICONERROR | MB_OK);
}

// Convert UTF-16 (std::wstring) to UTF-8 (std::string)
std::string utf16ToUtf8(const std::wstring& utf16Str)
{
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, utf16Str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8Str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, utf16Str.c_str(), -1, &utf8Str[0], sizeNeeded, nullptr, nullptr);
    return utf8Str;
}

// Get the executable path as a UTF-8 string on Windows
std::string getExecutablePath()
{
    wchar_t buffer[MAX_PATH];
    DWORD length = GetModuleFileNameW(NULL, buffer, MAX_PATH);

    if (length == 0) {
        std::cerr << "Error getting executable path. Error code: " << GetLastError() << std::endl;
        return "";
    }

    std::wstring widePath(buffer, length);
    return utf16ToUtf8(widePath); // Convert to UTF-8
}

} // namespace Game::Platform::Windows

#endif