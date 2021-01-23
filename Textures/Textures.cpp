#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Common/Shader.h"
#include "../Common/def.h"

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto* const window = glfwCreateWindow(WIDTH, HEIGHT, "Texture", nullptr, nullptr);

	if(window == nullptr)
	{
		std::cout << "Failed to Create window";
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to Init GLAD";
		return -1;
	}
	//2332 892

	unsigned int texture1,texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	// 첫번째 텍스쳐1
	// ==============
	glBindTexture(GL_TEXTURE_2D, texture1);

	//텍스쳐 래핑 / 필터링 옵션 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 텍스처 필터링
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //가장 가까운 밉맵 레벨, 선형 보간 택스처 확대는 밉맥 사용 X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int texture_with, texture_height, nr_channels;
	auto* data = stbi_load("Textures/image/container.jpg", &texture_with, &texture_height, &nr_channels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_with, texture_with, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
		return -1;
	}

	stbi_image_free(data);

	// 두번째 텍스쳐

	glBindTexture(GL_TEXTURE_2D, texture2);
	//텍스쳐 래핑 / 필터링 옵션 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 텍스처 필터링
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //가장 가까운 밉맵 레벨, 선형 보간 택스처 확대는 밉맥 사용 X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Textures/image/awesomeface.png", &texture_with, &texture_height, &nr_channels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_with, texture_with, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //png이므로 rgba
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	

	float vertices[] = {
		// position			// color			// texture coords
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.55f, 0.55f,	// top right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.55f, 0.45f,	// bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.45f, 0.45f, // bottom left
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f,	0.0f,	0.45f, 0.55f	// top left
	};

	unsigned indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned vbo, vao, ebo;


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	const Shader our_shader("Textures/shader.vert", "Textures/shader.frag");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 0));
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	our_shader.use();
	our_shader.set_int("texture1", 0);
	our_shader.set_int("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		our_shader.use();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0); // 텍스처를 바인딩 하기 전에 활성화시켜주어야 함. 0은 기본으로 설정.
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		our_shader.use();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}
}
