// EcsTemplate.cpp : Defines the entry point for the application.
//

#include "buildinfo.h"
#include "src/console.hpp"
#include "src/logger.hpp"
#include "src/platform/platform_base.hpp"

#include "spdlog/spdlog.h"

#include "raylib-cpp.hpp"

int main()
{
    // Set the custom terminate handler
    std::set_terminate(Game::Platform::Base::customTerminateHandler);

    Game::Platform::Base::initialize();
    auto logPath = Game::Logger::setupLogger(Game::Platform::Base::g_cwd);
    SetTraceLogCallback(Game::Logger::customRaylibLogHandler);

    spdlog::info("{}", BuildInfo::m_projectName);
    spdlog::info("Version: {} {}", BuildInfo::m_version, BuildInfo::m_buildType);
    spdlog::info("Built: {}", BuildInfo::m_timestamp);
    spdlog::info("Commit SHA: {}", BuildInfo::m_commitShaSum);
    spdlog::info("Current working directory: {}", std::filesystem::current_path().string());

    int screenWidth = 800;
    int screenHeight = 600;

    raylib::Window window(screenWidth, screenHeight, "raylib-cpp - basic window");
    Game::Console console(logPath);

    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        if (IsKeyPressed(KEY_GRAVE)) {
            console.changeState();
        }

        if (IsKeyPressed(KEY_SPACE)) {
            spdlog::critical("SPACEEE!!");
        }

        console.update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the console if active
        console.render();

        EndDrawing();
    }

    return 0;
}
