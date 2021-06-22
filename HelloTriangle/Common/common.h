#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

 inline void frame_buffer_size_callback(int w, int h)
{
	glViewport(0, 0, w, h);
}
