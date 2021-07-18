// ReSharper disable StringLiteralTypo
// Do not Blame
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

float vertices[] = {
	// ��ġ             
	 0.5f, -0.5f, 0.0f, //������ �Ʒ� red
	-0.5f, -0.5f, 0.0f, //���� �Ʒ� zero
	 0.0f,  0.5f, 0.0f // �߰� ��� green
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto* const window = glfwCreateWindow(800, 600, "Exercise3", nullptr, nullptr);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void*>(0)); //pos
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		our_shader.use();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	return 0;
}