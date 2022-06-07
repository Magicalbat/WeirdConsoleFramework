#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image/stb_image.h"

const uint32_t WIDTH = 160/2;
const uint32_t HEIGHT = 90/2;
const uint32_t CHAR_WIDTH = 16;
const uint32_t CHAR_HEIGHT = 16;

const float CHAR_UV_STEP = 1.0f / 225.0f;

constexpr glm::vec2 getCharUV(uint8_t c)
{
	uint32_t index = c - 31;
	if (index > 0 && index <= 224)
		return { (float)index * CHAR_UV_STEP, 0.0f};
	return { 0.0f, 0.0f };
}

unsigned int loadShader(const char* vertexPath, const char* fragmentPath)
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
	using namespace wcf;

	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(WIDTH * CHAR_WIDTH, HEIGHT * CHAR_HEIGHT, "Console Framework", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	std::cout << "\n\x1b[36mVersion: " << glGetString(GL_VERSION) << ", Renderer: " << glGetString(GL_RENDERER) << "\x1b[0m" << std::endl;

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nrChannels;
	uint8_t* data = stbi_load("res/font.png", &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	std::vector<Cochar> screen(WIDTH * HEIGHT);
	for (auto& item : screen)
	{
		item = { ' ', {1, 1, 1} };
	}

	std::vector<glm::vec2> translations(WIDTH * HEIGHT);
	std::vector<CocharData> cochars(WIDTH * HEIGHT);
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			translations[x + y * WIDTH] = { x * CHAR_WIDTH, y * CHAR_HEIGHT };
			cochars[x + y * WIDTH] = { getCharUV((x + y * WIDTH) % 255), {0, 1, 0} };
		}
	}

	unsigned int translationBuffer;
	glGenBuffers(1, &translationBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, translationBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * WIDTH * HEIGHT, translations.data(), GL_STATIC_DRAW);

	unsigned int cocharBuffer;
	glGenBuffers(1, &cocharBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cocharBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CocharData) * WIDTH * HEIGHT, cochars.data(), GL_DYNAMIC_DRAW);

	float positions[8] = {
		0.0f, 0.0f,
		CHAR_WIDTH, 0.0f,
		CHAR_WIDTH, CHAR_HEIGHT,
		0.0f, CHAR_HEIGHT
	};

	unsigned int positionBuffer;
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, translationBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(0));
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, cocharBuffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CocharData), (void*)(0));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CocharData), (void*)(sizeof(glm::vec2)));
	glVertexAttribDivisor(3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

    unsigned int shaderProg = loadShader("res/vertex.glsl", "res/fragment.glsl");
    glUseProgram(shaderProg);

	unsigned int uvStepLoc = glGetUniformLocation(shaderProg, "uUvStep");
	unsigned int modelProjLoc = glGetUniformLocation(shaderProg, "uModelProj");

	glUniform2f(uvStepLoc, CHAR_UV_STEP, 1.0f);

	glm::mat4 orthoProj = glm::ortho(0.0f, (float)(WIDTH*CHAR_WIDTH), (float)(HEIGHT*CHAR_HEIGHT), 0.0f);// , -1.0f, 1.0f);
	glUniformMatrix4fv(modelProjLoc, 1, GL_TRUE, glm::value_ptr(orthoProj));

	std::vector<glm::vec2> chars(WIDTH);
	for (auto& pos : chars)
	{
		pos.x = (float)(rand() % WIDTH);
		pos.y = -(float)(rand() % HEIGHT);
	}
	
    auto prevTime = std::chrono::high_resolution_clock::now();

	glClearColor(0, 1, 1, 1);
	while (!glfwWindowShouldClose(window))
	{
		auto curTime = std::chrono::high_resolution_clock::now();
		auto delta_ms = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - prevTime);
		prevTime = curTime;

		float delta = (float)delta_ms.count() * 0.001f;
		std::string title = "ConsoleFramework " + std::to_string((int)(1.0f / delta)) + "fps";
		glfwSetWindowTitle(window, title.c_str());
		
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto& pos : chars)
		{
			pos.y += 0.1f;

			if (fmodf(pos.y, 1.0f) < 0.1 && pos.y >= 0 && pos.y < HEIGHT)
			{
				screen[(int)pos.x + (int)pos.y * WIDTH].chr = rand() % (255 - ' ') + ' ';
				screen[(int)pos.x + (int)pos.y * WIDTH].col = glm::vec3(0, 1, 0);
			}

			if (pos.y > HEIGHT)
			{
				pos.x = (float)(rand() % WIDTH);
				pos.y = -(float)(rand() % 10);
			}
		}

		for (auto& item : screen)
		{
			if (item.col.y > 0)
				item.col.y -= 0.0025f;
		}

		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				int index = x + y * WIDTH;
				cochars[index] = {getCharUV(screen[index].chr), screen[index].col};
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, cocharBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(CocharData) * cochars.size(), cochars.data());

		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glDrawArraysInstanced(GL_QUADS, 0, 4, WIDTH*HEIGHT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

    glDeleteProgram(shaderProg);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &translationBuffer);
	glDeleteBuffers(1, &cocharBuffer);

	glfwTerminate();

	return 0;
}