#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> //« ø‰«— OpenGL

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int program_id;
	Shader(const char* vertex_path, const char* fragment_path);

	void use();
	void set_bool(const std::string& name, bool value);
	void set_int(const std::string& name, int value);
	void set_float(const std::string& name, float value);

private:
	const char* vertex_shader_code_;
	const char* fragment_shader_code_;


	void read_shader_code(const char* vertex_path, const char* fragment_path);
	static bool is_success_compile_shader(GLuint shader_id);
	static void print_shader_error(GLuint shader_id);
	static GLuint compile_shader(const char* source, GLuint shader_type);
};

#endif
