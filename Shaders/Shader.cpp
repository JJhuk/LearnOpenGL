#include "Shader.h"

#include "Common/shader.h"

void Shader::read_shader_code(const char* vertex_path, const char* fragment_path)
{
	std::ifstream vertex_shader_file;
	std::ifstream fragment_shader_file;

	vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertex_shader_file.open(vertex_path);
		fragment_shader_file.open(fragment_path);
		std::stringstream vertex_shader_stream, fragment_shader_stream;

		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();

		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_shader_code_ = vertex_shader_stream.str();
		fragment_shader_code_ = fragment_shader_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	
}

bool Shader::is_success_compile_shader(const GLuint shader_id)
{
	int success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	return success != 0;
}

void Shader::print_shader_error(const GLuint shader_id)
{
	char info_log[512];

	if (is_success_compile_shader(shader_id))
		return;

	glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
}

GLuint Shader::compile_shader(const char* source, const GLuint shader_type)
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


Shader::Shader(const char* vertex_path, const char* fragment_path) : program_id(0)
{
	read_shader_code(vertex_path, fragment_path);

	program_id = glCreateProgram();
	const auto vertex_shader = compile_shader(this->vertex_shader_code_.c_str(), GL_VERTEX_SHADER);
	const auto fragment_shader = compile_shader(this->fragment_shader_code_.c_str(), GL_FRAGMENT_SHADER);

	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);
	glLinkProgram(program_id);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::use() const
{
	glUseProgram(program_id);
}

void Shader::set_bool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), static_cast<int>(value));
}

void Shader::set_int(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set_float(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}