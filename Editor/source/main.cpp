#include "Editor/Include/Editor.h"
#include "Runtime/Engine.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define MARS_XSTR(s) MARS_STR(s)
#define MARS_STR(s) #s

int main(int argc, char** argv)
{
    std::filesystem::path executable_path(argv[0]);
    std::filesystem::path config_file_path = executable_path.parent_path() / "MarsEditor.ini";

    MarsEngine::MarsEngine* engine = new MarsEngine::MarsEngine();

    engine->startEngine(config_file_path.generic_string());
    engine->initialize();

    MarsEngine::MarsEditor* editor = new MarsEngine::MarsEditor();
    editor->initialize(engine);

    editor->run();

    editor->clear();

    engine->clear();
    engine->shutdownEngine();

    return 0;
}
