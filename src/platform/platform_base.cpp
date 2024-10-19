#include "platform_base.hpp"

#include "buildinfo.h"

#ifdef _WIN32
    #include "platform_windows.hpp"

    #include <windows.h>
#else
    #error "This platform is not supported!"
#endif

#include <exception>

namespace Game::Platform::Base {

std::filesystem::path g_cwd;

// Cross-platform function to detect if the debugger is attached
bool isDebuggerAttached()
{
#ifdef BUILD_TYPE_DEBUG
    #ifdef _WIN32
    return IsDebuggerPresent();
    #elif __linux__
    // On Linux, check if the process is being traced
    return (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1);
    #elif __APPLE__
    // On macOS, use sysctl to detect if the process is being debugged
    int mib[4];
    struct kinfo_proc info;
    size_t size = sizeof(info);

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    sysctl(mib, 4, &info, &size, NULL, 0);

    return (info.kp_proc.p_flag & P_TRACED) != 0;
    #else
    // For unsupported platforms, return false
    return false;
    #endif
    return false;
#else
    return false;
#endif
}

// Custom terminate handler that shows a window with the stack trace and terminates the program
void customTerminateHandler()
{
    try {
        // Try to re-throw the current exception to get its type and message
        std::exception_ptr exptr = std::current_exception();
        if (exptr) {
            std::rethrow_exception(exptr);
        }
    } catch (const std::exception& e) {
#ifdef _WIN32
        Windows::showErrorWindow(e.what()); // Show the error window with the exception message
        _exit(1); // Immediately terminate without cleanup
#endif
    } catch (...) {
#ifdef _WIN32
        Windows::showErrorWindow("Unknown exception occurred"); // Generic message for non-std::exception exceptions
        _exit(1); // Immediately terminate without cleanup
#endif
    }

    // If we reach this point, there's no current exception, so terminate anyway
    _exit(1);
}

void initialize()
{
#ifdef BUILD_TYPE_DEBUG
    g_cwd = BuildInfo::m_workspaceDir;
#else
    #ifdef _WIN32
    g_cwd = Windows::getExecutablePath();
    #endif
#endif

    std::filesystem::current_path(g_cwd);
}

} // namespace Game::Platform::Base
