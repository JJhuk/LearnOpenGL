#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> //�ʿ��� OpenGL
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int program_id;
	Shader(const char* vertex_path, const char* fragment_path);

	void use() const;
	void set_bool(const std::string&, const bool &) const;
	void set_int(const std::string&, const int &) const;
	void set_float(const std::string&, const float &) const;
	void set_mat4(const std::string&, const glm::mat4&) const;

    void read_shader_code(const char* vertex_path, const char* fragment_path);
    static bool is_success_compile_shader(GLuint shader_id);
    static void print_shader_error(GLuint shader_id);
    static GLuint compile_shader(const char* source, GLuint shader_type);

private:
	std::string vertex_shader_code_;
	std::string fragment_shader_code_;

};

#endif
