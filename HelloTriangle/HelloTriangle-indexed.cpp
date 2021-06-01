#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragment_shader_source = R"(#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

// float vertices[] = {
// 	// ù �ﰢ��
// 	0.5f,  0.5f, 0.0f,  // ������ ���
//     0.5f, -0.5f, 0.0f,  // ������ �ϴ�
//    -0.5f,  0.5f, 0.0f,  // ���� ���
//    // �ι�° �ﰢ��
//     0.5f, -0.5f, 0.0f,  // ������ �ϴ�
//    -0.5f, -0.5f, 0.0f,  // ���� �ϴ�
//    -0.5f,  0.5f, 0.0f   // ���� ���
// }; 

float vertices[] = {
	0.5f, 0.5f, 0.0f, // ������ ���
	0.5f, -0.5f, 0.0f, // ������ �ϴ�
	-0.5f, -0.5f, 0.0f, // ���� �ϴ�
	-0.5f, 0.5f, 0.0f // ���� ���
};

unsigned int indices[] = {
	0, 1, 3, //ù ��° �ﰢ��
	1, 2, 3 // �� ��° �ﰢ��
};

void frame_buffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

bool is_success_compile(const GLuint shader_id)
{
    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    return success != 0;
}

void print_shader_error(const GLuint shader_id)
{
    char info_log[512];

    if (is_success_compile(shader_id)) return;

    glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
}


GLuint compile_shader(const char* source, const GLuint shader_name)
{
    if (!(shader_name == GL_FRAGMENT_SHADER || shader_name == GL_VERTEX_SHADER))
        return -1;

    const auto shader = glCreateShader(shader_name);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

	if(!is_success_compile(shader))
	{
        print_shader_error(shader);
        return -1;
	}

    return shader;
}

GLFWwindow* create_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto* const window = glfwCreateWindow(800, 600, "Hello World!", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Fail" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
	
    return window;
}

int main()
{
	auto* const window = create_window();

	// �츮�� ���̴� ���α׷��� �����ϰ� �������մϴ�
	// ----------------------------------------
	const auto vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
	const auto fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
	const auto shader_program = glCreateProgram(); //���̴� ���α׷� ����

	//�� ���̴��� ����� ���� ���̴��� �Է��� �ǵ��� �����ؾ� ��.
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	//���̴� ���α׷��� �ϼ������Ƿ� ���̴��� ����.
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
    unsigned int vbo, vao, ebo;

	//CREATE
	glGenVertexArrays(1, &vao); // ���ؽ� �迭 ��ü ����
    glGenBuffers(1, &vbo); // ���ؽ� ���� ��ü ����
	glGenBuffers(1, &ebo); // ��� ��ü ���� ����

	//BINDING
	glBindVertexArray(vao); //���ý� �迭 ��ü ���ε�
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //���ؽ� �۹� ��ü ���ε�
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //���ؽ� ������ ����
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // ��� ��ü ���� ���ε�
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //�ε��� ������ ����

	//���ؽ� �Ӽ��� ���ؽ� �����͸� �ؼ��ϴ� ���
	//���ؽ� �Ӽ��� ��ġ�� 0���� �����ϰ� vec3�� float �Է�
	//�� �Լ��� ȣ���� �� GL_ARRAY_BUFFER�� ���ε� �� VBO�� ���� ����. �̹� ���ε� �Ǿ� �����Ƿ� ���ؽ� �Ӽ� 0�� ���ؽ� �����Ϳ� ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// remember: ���ε� �� ��� ���� ��ü�� VAO�� ����Ǿ� �����Ƿ� VAO�� Ȱ��ȭ �Ǿ� �ִ� ���� EBO�� ���ε� �������� ���ʽÿ�.
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL); //vbo ���ε� ����
	// ���߿� VAO�� ���ε� �����Ͽ� �ٸ� VAO�� ȣ���Ͽ� �� VAO�� �Ǽ��� �������� ���� �� ������, �̴� ���� �߻����� �ʽ��ϴ�.
	// �ٸ� VAO���� �����ϴ� ���� glBindVertexArray�� ���� ȣ���� �ʿ��ϹǷ� VAO(�Ǵ� VBO)�� ���� �ʿ����� ���� �� �Ϲ������� ���ε��� �������� �ʽ��ϴ�.
	glBindVertexArray(NULL);

	// �Ʒ� �Լ��� �ּ�ó�� ���� ������ ���̾������� ���� �׸��ϴ�.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	while(!glfwWindowShouldClose(window))
	{
		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //������ �������� ȭ�� ����� �������� ������ ���� ���۰� ��� �׷��� (���� ���� �Լ�)
		glClear(GL_COLOR_BUFFER_BIT); //���� ��Ʈ ���� (���� ��� �Լ�)

		// draw
        glUseProgram(shader_program);
        glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
        glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteProgram(shader_program);
	
    glfwTerminate();
	
    return 0;
}