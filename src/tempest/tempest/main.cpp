// This file is part of Tempest project
// Author: Karol Kontny

#include <GLFW/glfw3.h>

#include "application.h"

#include <application/argument_parser.h>
#include <application/event_processor.h>
#include <application/glfw_exception.h>
#include <application/main_window.h>
#include <thread/main_thread.h>
#include <array>
#include <cstdlib>
#include <device/monitor.h>
#include <fmt/printf.h>

namespace tst {

void init_Glfw() {
    assert(std::this_thread::get_id() == tst::core::main_thread::get_id());
    if (!glfwInit()) {
        fmt::printf("Can't initialize Glfw\n");
        std::exit(EXIT_FAILURE);
    }
}

auto init_monitor() {
    assert(std::this_thread::get_id() == tst::core::main_thread::get_id());
    GLFWmonitor* glfwMonitor = glfwGetPrimaryMonitor();
    if (glfwMonitor) {
        return std::make_unique<device::monitor>(*glfwMonitor);
    } else {
        throw tst::application::glfw_exception("Can't get monitor");
    }
}

void deinit_Glfw() {
    assert(std::this_thread::get_id() == tst::core::main_thread::get_id());
    glfwTerminate();
}
} // namespace tst

int main(int argc, const char** argv) {
    const tst::application::program_params params = tst::application::parse_arguments(argc, argv);
    tst::init_Glfw();
    try {
        auto monitor = tst::init_monitor();
        tst::application::application app(params, std::move(monitor), "Tempest");
        app.run();
    } catch (tst::application::glfw_exception& ex) {
        const char* description;
        glfwGetError(&description);
        fmt::printf("%s, error: %s\n", ex.what(), description);
        std::exit(EXIT_FAILURE);
    } catch (...) { fmt::printf("Unknown error\n"); }
    tst::deinit_Glfw();
    return 0;
}
