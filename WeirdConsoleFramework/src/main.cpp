#include <iostream>
#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image/stb_image.h"

#include "cochar.hpp"
#include "interface.hpp"

const uint32_t WIDTH = 160;
const uint32_t HEIGHT = 90;
const uint32_t CHAR_WIDTH = 8;
const uint32_t CHAR_HEIGHT = 8;

const float CHAR_UV_STEP = 1.0f / 225.0f;

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

	Screen screen(WIDTH * HEIGHT);
	for (auto& item : screen)
	{
		item = { ' ', {1, 1, 1} };
	}

	OpenGL_Interface glInterface(WIDTH, HEIGHT, CHAR_WIDTH, CHAR_HEIGHT);

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

		glInterface.drawScreen(screen);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}