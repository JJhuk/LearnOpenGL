// ReSharper disable StringLiteralTypo
// Do not Blame
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

float vertices[] = {
	// 위치              // 색상
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 오른쪽 하단
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 왼쪽 하단
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 상단
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto* const window = glfwCreateWindow(800, 600, "Exercise1", nullptr, nullptr);
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

	unsigned vbo, vao;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const Shader our_shader("Shaders/shader.vert", "Shaders/shader.frag");
	unsigned program_id = our_shader.program_id;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(0)); //pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3)); //color
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		our_shader.use();
		our_shader.set_float("offset", 0.5f);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	return 0;
}