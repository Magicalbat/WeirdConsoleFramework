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

	OpenGL_Interface glInterface(WIDTH, HEIGHT, CHAR_WIDTH, CHAR_HEIGHT);

	std::vector<Cochar> screen(WIDTH * HEIGHT);
	for (auto& item : screen)
	{
		item = { ' ', {1, 1, 1} };
	}

	std::vector<glm::vec2> chars(WIDTH);
	for (auto& pos : chars)
	{
		pos.x = (float)(rand() % WIDTH);
		pos.y = -(float)(rand() % HEIGHT);
	}
	
    auto prevTime = std::chrono::high_resolution_clock::now();

	while (glInterface.running())
	{
		auto curTime = std::chrono::high_resolution_clock::now();
		auto delta_ms = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - prevTime);
		prevTime = curTime;
		float delta = (float)delta_ms.count() * 0.001f;
		
		for (auto& pos : chars)
		{
			pos.y += 0.1f;

			if (fmodf(pos.y, 1.0f) < 0.1f && pos.y >= 0 && pos.y < HEIGHT)
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
	}

	return 0;
}