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
// 	// 첫 삼각형
// 	0.5f,  0.5f, 0.0f,  // 오른쪽 상단
//     0.5f, -0.5f, 0.0f,  // 오른쪽 하단
//    -0.5f,  0.5f, 0.0f,  // 왼쪽 상단
//    // 두번째 삼각형
//     0.5f, -0.5f, 0.0f,  // 오른쪽 하단
//    -0.5f, -0.5f, 0.0f,  // 왼쪽 하단
//    -0.5f,  0.5f, 0.0f   // 왼쪽 상단
// }; 

float vertices[] = {
	0.5f, 0.5f, 0.0f, // 오른쪽 상단
	0.5f, -0.5f, 0.0f, // 오른쪽 하단
	-0.5f, -0.5f, 0.0f, // 왼쪽 하단
	-0.5f, 0.5f, 0.0f // 왼쪽 상단
};

unsigned int indices[] = {
	0, 1, 3, //첫 번째 삼각형
	1, 2, 3 // 두 번째 삼각형
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

	// 우리의 셰이더 프로그램을 빌드하고 컴파일합니다
	// ----------------------------------------
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
	
    unsigned int vbo, vao, ebo;

	//CREATE
	glGenVertexArrays(1, &vao); // 버텍스 배열 개체 생성
    glGenBuffers(1, &vbo); // 버텍스 버퍼 개체 생성
	glGenBuffers(1, &ebo); // 요소 개체 버퍼 생성

	//BINDING
	glBindVertexArray(vao); //버택스 배열 개체 바인딩
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //버텍스 퍼버 개체 바인딩
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //버텍스 데이터 복사
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // 요소 개체 버퍼 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //인덱스 데이터 복사

	//버텍스 속성의 버텍스 데이터를 해석하는 방법
	//버텍스 속성의 위치를 0으로 지정하고 vec3인 float 입력
	//이 함수를 호출할 때 GL_ARRAY_BUFFER에 바인딩 된 VBO에 의해 결정. 이미 바인딩 되어 있으므로 버텍스 속성 0은 버텍스 데이터와 연결
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// remember: 바인딩 된 요소 버퍼 개체가 VAO에 저장되어 있으므로 VAO가 활성화 되어 있는 동안 EBO를 바인딩 해제하지 마십시오.
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL); //vbo 바인딩 해제
	// 나중에 VAO를 바인딩 해제하여 다른 VAO를 호출하여 이 VAO를 실수로 수정하지 않을 수 있지만, 이는 거의 발생하지 않습니다.
	// 다른 VAO들을 수정하는 것은 glBindVertexArray에 대한 호출이 필요하므로 VAO(또는 VBO)가 직접 필요하지 않을 때 일반적으로 바인딩을 해제하지 않습니다.
	glBindVertexArray(NULL);

	// 아래 함수를 주석처리 하지 않으면 와이어프레임 모드로 그립니다.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	while(!glfwWindowShouldClose(window))
	{
		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //선택한 색상으로 화면 지우기 설정하지 않으면 이전 버퍼가 계속 그려짐 (상태 지정 함수)
		glClear(GL_COLOR_BUFFER_BIT); //지울 비트 선정 (상태 사용 함수)

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