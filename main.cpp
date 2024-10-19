// EcsTemplate.cpp : Defines the entry point for the application.
//

#include "buildinfo.h"
#include "src/logger.hpp"
#include "src/platform/platform_base.hpp"

#include "spdlog/spdlog.h"

#include "raylib-cpp.hpp"

#include <iostream>

int main()
{
    // Set the custom terminate handler
    std::set_terminate(Game::Platform::Base::customTerminateHandler);

    Game::Platform::Base::initialize();

    spdlog::info("{}", BuildInfo::m_projectName);
    spdlog::info("Version: {} {}", BuildInfo::m_version, BuildInfo::m_buildType);
    spdlog::info("Built: {}", BuildInfo::m_timestamp);
    spdlog::info("Commit SHA: {}", BuildInfo::m_commitShaSum);
    spdlog::info("Current working directory: {}", std::filesystem::current_path().string());

    int screenWidth = 800;
    int screenHeight = 450;

    SetTraceLogCallback(customLogHandler);
    raylib::Window window(screenWidth, screenHeight, "raylib-cpp - basic window");

    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    return 0;
}
