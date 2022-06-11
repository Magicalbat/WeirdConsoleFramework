#pragma once

#include "vector2.hpp"

namespace wcf
{
	struct Rect
	{
		float x, y, w, h;

		Rect()
			:x(0), y(0), w(0), h(0)
		{}

		Rect(float x, float y, float w, float h)
			:x(x), y(y), w(w), h(h)
		{}

		bool collidePoint(const Vector2& other)
		{
			return other.x > x && other.x < x + w && other.y > y && other.y < y + h;
		}

		bool collideRect(const Rect& other)
		{
			return x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y;
		}
	};
}