#pragma once

#include <filesystem>

namespace Game::Platform::Base {

extern std::filesystem::path g_cwd;

bool isDebuggerAttached();
void customTerminateHandler();
void initialize();

}
