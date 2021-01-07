#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Common/def.h"
#include "Common/shader.h"

struct vec4_f
{
	float r;
	float g;
	float b;
	float a;
} clear_color = {
	0.2f,
	0.3f,
	0.3f,
	1.0f
};

float vertices[] = {
		-0.5f, -0.5f, 0.0f, // 왼쪽 삼각형 왼쪽
		0.0f, 0.5f, 0.0f, // 왼쪽 삼각형 위
		0.5f, -0.5f, 0.0f // 왼쪽 삼각형 오른쪽
};

const char* vertex_shader_source = R"(
	#version 330 core

	layout (location = 0) in vec3 aPos;

	out vec4 vertexColor;

	void main()
	{
		gl_Position = vec4(aPos, 1.0);
		vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
	}
)";

const char* fragment_shader_source =R"(
	#version 330 core

	out vec4 FragColor;

	uniform vec4 ourColor;

	void main()
	{
		FragColor = ourColor;
	}
)";

void frame_buffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	// WINDOW CREATE AND INIT
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto* const window = glfwCreateWindow(WIDTH, HEIGHT, "Exercise2", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed To Create Window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// shader
	const auto vertex_shader = shader::compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
	const auto fragment_shader = shader::compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

	if (vertex_shader == NULL || fragment_shader == NULL)
	{
		return -1;
	}

	const auto program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);


	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//vao vbo
	unsigned vao;
	unsigned vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		glUseProgram(program);

		const auto time = glfwGetTime();
		const auto green_value = 0.5 + sin(time) / 2.0;
		const auto vertex_color_location = glGetUniformLocation(program, "ourColor");
		glUniform4f(vertex_color_location, 0.0f, static_cast<GLfloat>(green_value), 0.0f, 1.0f);

		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glfwTerminate();
	return 0;
}
