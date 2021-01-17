#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

 inline void frame_buffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}
