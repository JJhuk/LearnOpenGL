#include "common.h"
#include "def.h"
#include <string>

void Common::Window::init() const {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

Common::Window::Window(std::string title) {
    init();
    title = title.empty() ? "Empty Title" : title;
    windowPtr = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);
    assert(windowPtr != nullptr);
    glfwMakeContextCurrent(windowPtr);
    assert(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));
}

Common::Window::~Window() {
    glfwTerminate();
}
