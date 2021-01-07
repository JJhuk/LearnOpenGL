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
	//위치					// 색상
	 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, // 오른쪽 하단
	-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, // 왼쪽 하단
	 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f  // 상단
};

const char* vertex_shader_source = R"(
	#version 330 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;

	out vec3 ourColor;

	void main()
	{
		gl_Position = vec4(aPos, 1.0);
		ourColor = aColor;
	}
)";

const char* fragment_shader_source = R"(
	#version 330 core

	out vec4 FragColor;

	in vec3 ourColor;

	void main()
	{
		FragColor = vec4(ourColor, 1.0);
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

	//위치 Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(0)); //다음 위치 데이터까지 stride가 6 * 4 = 24
	glEnableVertexAttribArray(0);

	//색상 Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(float))); //색상의 offset은 위치 데이터 다음이므로 3칸뒤
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		glUseProgram(program);

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
