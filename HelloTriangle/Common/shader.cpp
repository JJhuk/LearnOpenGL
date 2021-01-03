#include "shader.h"
#include <iostream>

bool shader::is_success_compile_shader(const GLuint shader_id)
{
	int success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	return success != 0;
}

void shader::print_shader_error(const GLuint shader_id)
{
	char info_log[512];

	if (is_success_compile_shader(shader_id)) return;

	glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
}

GLuint shader::compile_shader(const char* source, const GLuint shader_type)
{
	if (!(shader_type == GL_FRAGMENT_SHADER || shader_type == GL_VERTEX_SHADER))
	{
		std::cout << "shader_type supported only GL_FRAGMENT_SHADER and GL_VERTEX_SHADER";
		return NULL;
	}

	const auto shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	if (!is_success_compile_shader(shader))
	{
		print_shader_error(shader);
		return -1;
	}

	return shader;
}
