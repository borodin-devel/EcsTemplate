// EcsTemplate.cpp : Defines the entry point for the application.
//

#include "EcsTemplate.h"

#include "buildinfo.h"

#include <filesystem>

inline const std::filesystem::path g_currentWorkingDir = std::filesystem::current_path();

int main()
{
    std::cout << "ECS Template" << '\n'
              << "Version: " << BuildInfo::Version << '\n'
              << "Built: " << BuildInfo::Timestamp << '\n'
              << "Commit SHA: " << BuildInfo::CommitSHA << '\n'
              << "Path: " << g_currentWorkingDir.string() << std::endl;

    return 0;
}
