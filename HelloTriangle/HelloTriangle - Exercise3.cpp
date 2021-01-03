#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Common/def.h"
#include "Common/shader.h"

#define LEFT_TRIANGLE 0
#define RIGHT_TRIANGLE 1
#define TRIANGLE_CNT 2

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

float vertices[TRIANGLE_CNT][9] = {
	{
		-1.0f, -0.5f, 0.0f, // 왼쪽 삼각형 왼쪽
		-0.5f, 0.5f, 0.0f, // 왼쪽 삼각형 위
		0.0f, -0.5f, 0.0f // 왼쪽 삼각형 오른쪽
	},

	{
		0.0f, -0.5f, 0.0f, // 오른쪽 삼각형 왼쪽
		0.5f, 0.5f, 0.0f, // 오른쪽 삼각형 위
		1.0f, -0.5f, 0.0f // 오른쪽 삼각형 오른쪽	
	},
};

const char* vertex_shader_source =
	R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	})";

const char* fragment_shader_source =
	R"(
	#version 330 core
	out vec4 FragColor;

	void main()
	{
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	})";

const char* yellow_fragment_shader_source =
	R"(
	#version 330 core
	out vec4 FragColor;

	void main()
	{
		FragColor = vec4(1.0f, 1.0f, 0f, 1.0f);
	})";

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

	const auto window = glfwCreateWindow(WIDTH, HEIGHT, "Exercise2", nullptr, nullptr);
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
	const auto fragment_shader = shader::compile_shader(fragment_shader_source,GL_FRAGMENT_SHADER);
	const auto yellow_fragment_shader = shader::compile_shader(yellow_fragment_shader_source, GL_FRAGMENT_SHADER);

	if (vertex_shader == NULL || fragment_shader == NULL || yellow_fragment_shader == NULL)
	{
		return -1;
	}

	const auto program = glCreateProgram();
	const auto program2 = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glAttachShader(program2, vertex_shader);
	glAttachShader(program2, yellow_fragment_shader);
	glLinkProgram(program2);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(yellow_fragment_shader);

	//vao vbo
	unsigned vaos[TRIANGLE_CNT];
	unsigned vbos[TRIANGLE_CNT];

	glGenVertexArrays(TRIANGLE_CNT, vaos);
	glGenBuffers(TRIANGLE_CNT, vbos);

	for (auto index = 0; index < TRIANGLE_CNT; index++)
	{
		glBindVertexArray(vaos[index]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[index]);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices[index], vertices[index], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
	}

	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vaos[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(program2);
		glBindVertexArray(vaos[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(TRIANGLE_CNT, vbos);
	glDeleteVertexArrays(TRIANGLE_CNT, vaos);
	glDeleteProgram(program);
	glDeleteProgram(program2);
	glfwTerminate();
	return 0;
}
