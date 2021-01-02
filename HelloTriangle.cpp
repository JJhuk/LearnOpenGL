#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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


float vertices[] = {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
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


    unsigned int vbo, vao;
    glGenBuffers(1, &vbo); //버텍스 버퍼 개체 생성
    glGenVertexArrays(1, &vao); //버텍스 배열 개체 생성

    glBindVertexArray(vao); //버택스 배열 개체 바인딩
	
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //버텍스 퍼버 개체 바인딩
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //버텍스 데이터 복사

    const auto vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    const auto fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

	const auto shader_program = glCreateProgram(); //셰이더 프로그램 생성

	//각 셰이더의 출력이 다음 셰이더의 입력이 되도록 연결해야 함.
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

	//셰이더 프로그램을 완성했으므로 셰이더는 삭제.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

	//버텍스 속성의 버텍스 데이터를 해석하는 방법
	//버텍스 속성의 위치를 0으로 지정하고 vec3인 float 입력
	//이 함수를 호출할 때 GL_ARRAY_BUFFER에 바인딩 된 VBO에 의해 결정. 이미 바인딩 되어 있으므로 버텍스 속성 0은 버텍스 데이터와 연결
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	while(!glfwWindowShouldClose(window))
	{
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3); //버택스 배열 시작 인덱스, 우리가 그리는 버택스의 수

		glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //선택한 색상으로 화면 지우기 설정하지 않으면 이전 버퍼가 계속 그려짐 (상태 지정 함수)
        glClear(GL_COLOR_BUFFER_BIT); //지울 비트 선정 (상태 사용 함수)
	}

    glfwTerminate();
	
    return 0;
}