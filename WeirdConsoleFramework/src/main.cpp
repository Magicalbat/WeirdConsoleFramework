#include <iostream>
#include <vector>
#include <chrono>

#include "wcf.hpp"
#include "vector2.hpp"

const uint32_t WIDTH = 160;
const uint32_t HEIGHT = 90;
const uint32_t CHAR_WIDTH = 8;
const uint32_t CHAR_HEIGHT = 8;

const float CHAR_UV_STEP = 1.0f / 225.0f;

int main()
{
	wcf::init(WIDTH, HEIGHT, CHAR_WIDTH, CHAR_HEIGHT);

	std::vector<wcf::Vector2> chars(WIDTH);
	for (auto& pos : chars)
	{
		pos.x = (float)(rand() % WIDTH);
		pos.y = -(float)(rand() % HEIGHT);
	}

	auto update = [&](float delta)
	{
		for (auto& pos : chars)
		{
			pos.y += 5.0f * delta;

			if (pos.y > HEIGHT)
			{
				pos.x = (float)(rand() % WIDTH);
				pos.y = -(float)(rand() % 10);
			}
		}
	};

	auto draw = [&]() 
	{
		for (auto& pos : chars)
		{
			if (fmodf(pos.y, 1.0f) < 0.1f && pos.y >= 0 && pos.y < HEIGHT)
			{
				uint8_t chr = rand() % (255 - ' ') + ' ';
				wcf::setCochar((int)pos.x, (int)pos.y, { chr, glm::vec3(0, 1, 0) });
			}
		}

		for (int x = 0; x < wcf::width(); x++)
		{
			for (int y = 0; y < wcf::height(); y++)
			{
				wcf::Cochar& cochar = wcf::getCochar(x, y);
				if (cochar.col.y > 0)
					cochar.col.y -= 0.0025f;
			}
		}
			
	};
	
	wcf::start(60.0f, update, draw);

	return 0;
}