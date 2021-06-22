
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../Common/Shader.h"
#include "../Common/common.h"
#include "../Common/stb_image.h"

enum class eTextureOption{
    container,
    smile
};

float vertices[] = {
        // position			// color			// texture coords
        0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// top right
        0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right
        -0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f,	1.0f, 1.0f,	0.0f,	0.0f, 1.0f	// top left
};

unsigned indices[] = {
        0, 1, 3,
        1, 2, 3
};

void setGlTexParameter(eTextureOption option)
{
    switch (option) {
        case eTextureOption::container:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case eTextureOption::smile:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            stbi_set_flip_vertically_on_load(true);
            break;
        default:
            assert(false);
    }
}
void loadTexture(eTextureOption option)
{
    std::string path = "Textures/image/";
    unsigned char* data;
    int texture_width, texture_height, nr_channels;
    switch (option) {
        case eTextureOption::container:
            path += "container.jpg";
            data = stbi_load(path.c_str(), &texture_width, &texture_height, &nr_channels, 0);
            assert(data != nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
        case eTextureOption::smile:
            path += "awesomeface.png";
            data = stbi_load(path.c_str(), &texture_width, &texture_height, &nr_channels, 0);
            assert(data != nullptr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
        default:
            assert(false);
    }
    stbi_image_free(data);

}

void genTexture(GLuint* texture, eTextureOption option)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    setGlTexParameter(option);
    loadTexture(option);
}

int main()
{
    auto window = new Common::Window("CoordinateSystems");

    unsigned vbo, vao, ebo;
    unsigned texture1, texture2;

    genTexture(&texture1, eTextureOption::smile);
    genTexture(&texture2, eTextureOption::container);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    Shader ourShader("CoordinateSystems/shader.vert.glsl", "CoordinateSystems/shader.frag");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    ourShader.use();
    ourShader.set_int("texture1", 0);
    ourShader.set_int("texture2",1);

    while (!glfwWindowShouldClose((window->windowPtr)))
    {
        ourShader.use();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window->windowPtr);
    }
    return 0;
}
