#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Common {
    inline void frame_buffer_size_callback(GLFWwindow* window, const int width, const int height)
    {
        glViewport(0, 0, width, height);
    }

    class Window {
    private:
        void init() const;

    public:
        Window(std::string);
        ~Window();
        GLFWwindow* windowPtr;
    };
}