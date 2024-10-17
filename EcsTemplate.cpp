// EcsTemplate.cpp : Defines the entry point for the application.
//

#include "EcsTemplate.h"

#include "raylib-cpp.hpp"
#include "spdlog/spdlog.h"

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

    spdlog::info("Welcome to spdlog!");

    int screenWidth = 800;
    int screenHeight = 450;

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
