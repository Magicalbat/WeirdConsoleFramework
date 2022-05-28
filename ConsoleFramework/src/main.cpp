#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

uint32_t loadShader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vStream(vertexPath);
	std::string vertexSource;
	if (vStream.is_open())
	{
		std::stringstream ss;
		ss << vStream.rdbuf();
		vertexSource = ss.str();
		vStream.close();
	}
	else
	{
		std::cout << "Failed to load shader at \"" << vertexPath << "\"." << std::endl;
		return 0;
	}

	std::ifstream fStream(fragmentPath);
	std::string fragmentSource;
	if (fStream.is_open())
	{
		std::stringstream ss;
		ss << fStream.rdbuf();
		fragmentSource = ss.str();
		fStream.close();
	}
	else
	{
		std::cout << "Failed to load shader at \"" << fragmentPath << "\"." << std::endl;
		return 0;
	}

	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	char const* vSourcePointer = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vSourcePointer, NULL);
	glCompileShader(vertexShader);

	int isCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int infoLogLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> errorMsg(infoLogLength);
		glGetShaderInfoLog(vertexShader, infoLogLength, &infoLogLength, &errorMsg[0]);
		std::cout << "Vertex Shader Failed to Compile: " << errorMsg.data() << std::endl;
	}

	char const* fSourcePointer = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fSourcePointer, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int infoLogLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> errorMsg(infoLogLength);
		glGetShaderInfoLog(fragmentShader, infoLogLength, &infoLogLength, &errorMsg[0]);
		std::cout << "Fragment Shader Failed to Compile: " << errorMsg.data() << std::endl;
	}

	uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> errorMsg(infoLogLength);
		glGetProgramInfoLog(vertexShader, infoLogLength, &infoLogLength, &errorMsg[0]);
		std::cout << "Shader Program Failed to Link: " << errorMsg.data() << std::endl;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Console Framework", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	
	std::cout << "\n\x1b[36mVersion: " << glGetString(GL_VERSION) << ", Renderer: " << glGetString(GL_RENDERER) << "\x1b[0m" << std::endl;

    float vertices[15] = {
        -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
         0.0f,  0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,   0.0f, 0.0f, 1.0f
    };

    uint32_t vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, &vertices, GL_STATIC_DRAW);

    uint32_t stride = sizeof(float) * 5;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(float) * 2));

    uint32_t shaderProg = loadShader("res/vertex.glsl", "res/fragment.glsl");
    glUseProgram(shaderProg);

	glClearColor(0, 1, 1, 1);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

    glDeleteProgram(shaderProg);
    glDeleteBuffers(1, &vertexBuffer);

	glfwTerminate();

	return 0;
}