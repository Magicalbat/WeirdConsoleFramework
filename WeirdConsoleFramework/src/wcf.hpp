#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <chrono>

#include "cochar.hpp"
#include "interface.hpp"
#include "rect.hpp"
#include "vector2.hpp"

namespace wcf
{
	namespace Internal
	{
		struct InternalData 
		{
			uint32_t width, height, charWidth, charHeight;
			std::vector<Cochar> screen;
			std::shared_ptr<Interface> interface;

			InternalData()
				:width(0), height(0), charWidth(0), charHeight(0), screen(), interface()
			{}

			InternalData(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight, std::vector<Cochar> screen, std::shared_ptr<Interface> interface)
				:width(width), height(height), charWidth(charWidth), charHeight(charHeight), screen(screen), interface(interface)
			{}
		};
	}

	void init(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight);
	void start(float fps, std::function<void(float delta)> update, std::function<void()> draw);
	void setKeyCallback(std::function<void(Key key, bool pressed)> callback);

	Vector2 getMousePos();

	Cochar& getCochar(int x, int y);

	void clear(Cochar cochar);
	void drawCochar(int x, int y, Cochar cochar);
	void drawCochar(Vector2 pos, Cochar cochar);
	void fillRect(int x, int y, int w, int h, Cochar cochar);
	void fillRect(Rect rect, Cochar cochar);

	uint32_t width();
	uint32_t height();
	uint32_t charWidth();
	uint32_t charHeight();
}