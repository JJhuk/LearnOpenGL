#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Common/Shader.h"
#include "../Common/def.h"



float vertices[] = {
	-1.0f, -0.5f, 0.0f, // 왼쪽 삼각형 왼쪽
	-0.5f, 0.5f, 0.0f, // 왼쪽 삼각형 위
	0.0f, -0.5f, 0.0f, // 왼쪽 삼각형 오른쪽

	0.0f, -0.5f, 0.0f, // 오른쪽 삼각형 왼쪽
	0.5f, 0.5f, 0.0f, // 오른쪽 삼각형 위
	1.0f, -0.5f, 0.0f, // 오른쪽 삼각형 오른쪽
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

void frame_buffer_size_callback(int w, int h)
{
	glViewport(0, 0, w, h);
}

int main()
{
	// WINDOW CREATE AND INIT
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	const auto window = glfwCreateWindow(WIDTH, HEIGHT, "Exercise1",
	                                     nullptr, nullptr);

	if (!window)
	{
		std::cout << "Faild Create Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//compile and linking shader
	const auto vertex_shader = Shader::compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
	const auto fragment_shader = Shader::compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
	const auto program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//Vertex Array, Buffer Object
	unsigned int vao;
	unsigned int vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6); //삼각형 6개 버텍스 그리기

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);
	glfwTerminate();
	return 0;
}
