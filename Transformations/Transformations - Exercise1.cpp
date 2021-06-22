#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Common/Shader.h"
#include "../Common/def.h"
#include "stb_image.h"

GLFWwindow* gl_init_and_create_window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
	const auto window = glfwCreateWindow(WIDTH, HEIGHT, "Transformation - Exercise1", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed Create Window!";
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to Init GLAD";
		return nullptr;
	}

	return window;
}


int main()
{
	const auto window = gl_init_and_create_window();

	if (window == nullptr)
	{
		return -1;
	}
	const Shader our_shader("Transformations/transformations - Exercise1.vert",
	                        "Transformations/transformations - Exercise1.frag");

#pragma region BIND_BUFFER
	float vertices[] = {
		// positions         // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f // top left 
	};

	unsigned indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);
	// texture Coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#pragma endregion

#pragma region Texture
	unsigned texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int texture_width, texture_height, nr_channels;
	stbi_set_flip_vertically_on_load(true);

	const auto* file_path = "Textures/image/container.jpg";
	auto* data = stbi_load(file_path, &texture_width, &texture_height, &nr_channels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		std::cout << stbi_failure_reason();
		return -1;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	const auto* path = "Textures/image/awesomeface.png";
	data = stbi_load(path, &texture_width, &texture_height, &nr_channels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		std::cout << stbi_failure_reason();
		return -1;
	}

	stbi_image_free(data);
	our_shader.use();
	our_shader.set_int("texture1", 0);
	our_shader.set_int("texture2", 1);
#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE0, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE1, texture2);

		auto transformation = glm::mat4(1.0f);
		// Exercise : Change order rotate and translate
		transformation = rotate(transformation, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		transformation = translate(transformation, glm::vec3(0.5f, -0.5f, 0.0f));

		our_shader.use();
		const GLint uniform_location = glGetUniformLocation(our_shader.program_id, "transform");
		glUniformMatrix4fv(uniform_location, 1, GL_FALSE, value_ptr(transformation));

		glBindVertexArray(vbo);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vbo);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glfwTerminate();
	return 0;
}
