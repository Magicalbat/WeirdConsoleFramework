#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image/stb_image.h"

struct Vertex
{
	glm::vec2 pos;
	glm::vec2 uv;
	glm::vec3 color;

	Vertex()
		:pos(0,0), uv(0,0), color(0,0,0)
	{}

	Vertex(glm::vec2 pos, glm::vec2 uv, glm::vec3 color)
		:pos(pos), uv(uv), color(color)
	{}
};

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

	std::vector<Vertex> vertices(WIDTH * HEIGHT * 4);
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			int index = (x*4) + (y*4) * WIDTH;

			vertices[index] = {
				{ x * CHAR_WIDTH, y * CHAR_HEIGHT },
				getCharUV('a'),
				{ 1.0f,1.0f,1.0f }
			};
			vertices[index + 1] = {
				{ x * CHAR_WIDTH + CHAR_WIDTH, y * CHAR_HEIGHT },
				getCharUV('a') + glm::vec2(CHAR_UV_STEP, 0),
				{ 1.0f,1.0f,1.0f }
			};
			vertices[index + 2] = {
				{ x * CHAR_WIDTH + CHAR_WIDTH, y * CHAR_HEIGHT + CHAR_HEIGHT },
				getCharUV('a') + glm::vec2(CHAR_UV_STEP, 1),
				{ 1.0f,1.0f,1.0f }
			};
			vertices[index + 3] = {
				{ x * CHAR_WIDTH, y * CHAR_HEIGHT + CHAR_HEIGHT },
				getCharUV('a') + glm::vec2(0, 1),
				{ 1.0f,1.0f,1.0f }
			};
		}
	}

	std::vector<std::pair<uint8_t, glm::vec3>> screen(WIDTH * HEIGHT);
	for (auto& item : screen)
	{
		item.first = ' ';
		item.second = glm::vec3(0);
	}

    uint32_t vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    uint32_t stride = sizeof(Vertex);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(glm::vec2)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(glm::vec2) + sizeof(glm::vec2)));

    uint32_t shaderProg = loadShader("res/vertex.glsl", "res/fragment.glsl");
    glUseProgram(shaderProg);

	uint32_t modelProjLoc = glGetUniformLocation(shaderProg, "uModelProj");

	glm::mat4 orthoProj = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f);// , -1.0f, 1.0f);
	glUniformMatrix4fv(modelProjLoc, 1, GL_TRUE, glm::value_ptr(orthoProj));

	std::vector<glm::vec2> chars(WIDTH);
	for (auto& pos : chars)
	{
		pos.x = (float)(rand() % WIDTH);
		pos.y = -(float)(rand() % HEIGHT);
	}

	glClearColor(0, 1, 1, 1);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		for (auto& pos : chars)
		{
			pos.y += 0.1;

			if (fmodf(pos.y, 1.0f) < 0.1 && pos.y >= 0 && pos.y < HEIGHT)
			{
				screen[(int)pos.x + (int)pos.y * WIDTH].first = rand() % (255 - ' ') + ' ';
				screen[(int)pos.x + (int)pos.y * WIDTH].second = glm::vec3(0, 1, 0);
			}

			if (pos.y > HEIGHT)
			{
				pos.x = (float)(rand() % WIDTH);
				pos.y = -(float)(rand() % 10);
			}
		}

		for (auto& item : screen)
		{
			if (item.second.y > 0)
				item.second.y -= 0.0025f;
		}

		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				int index = (x*4) + (y*4) * WIDTH;
				int screenIndex = x + y * WIDTH;
	
				vertices[index + 0].uv = getCharUV(screen[screenIndex].first);
				vertices[index + 1].uv = getCharUV(screen[screenIndex].first) + glm::vec2(CHAR_UV_STEP, 0);
				vertices[index + 2].uv = getCharUV(screen[screenIndex].first) + glm::vec2(CHAR_UV_STEP, 1);
				vertices[index + 3].uv = getCharUV(screen[screenIndex].first) + glm::vec2(0, 1);

				for (int i = 0; i < 4; i++)
					vertices[index + i].color = screen[screenIndex].second;
			}
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());

        glDrawArrays(GL_QUADS, 0, vertices.size() * 2);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

    glDeleteProgram(shaderProg);
    glDeleteBuffers(1, &vertexBuffer);

	glfwTerminate();

	return 0;
}