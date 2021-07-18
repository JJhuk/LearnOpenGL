#include "common.h"
#include "def.h"
#include <string>
#include <cassert>

void Common::Window::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// initialize and configure
Common::Window::Window(std::string title) {
    init();
    title = title.empty() ? "Empty Title" : title;
    windowPtr = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);
    assert(windowPtr != nullptr);
    glfwMakeContextCurrent(windowPtr);

    // glad: 모든 OpenGL 함수 포인터를 로딩합니다.
    assert(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));
}

Common::Window::~Window() {
    glfwTerminate();
}
