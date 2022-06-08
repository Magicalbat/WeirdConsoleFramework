#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <chrono>

#include "cochar.hpp"
#include "interface.hpp"

namespace wcf
{
	namespace Internal
	{
		struct InternalData 
		{
			uint32_t width, height, charWidth, charHeight;
			std::vector<Cochar> screen;
			std::shared_ptr<Interface> interface;
		};
	}

	void init(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight);
	void start(float fps, std::function<void(float delta)> update, std::function<void()> draw);

	uint32_t width();
	uint32_t height();
	uint32_t charWidth();
	uint32_t charHeight();
}