#pragma once
#include <glad/glad.h>

class shader
{
public:
	static void print_shader_error(GLuint shader_id);
	static bool is_success_compile_shader(GLuint shader_id);
	static GLuint compile_shader(const char* source, GLuint shader_type);
};
