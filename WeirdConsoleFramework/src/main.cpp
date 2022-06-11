#include <iostream>
#include <vector>
#include <algorithm>

#include "wcf.hpp"
#include "vector2.hpp"
#include "rect.hpp"

int main()
{
	wcf::init(160, 90, 8, 8);

	wcf::Rect r1{20, 20, 20, 20};
	wcf::Rect r2{ 0, 0, 10, 10 };

	auto update = [&](float delta)
	{
		wcf::Vector2 mousePos = wcf::getMousePos();
		r2.x = mousePos.x; r2.y = mousePos.y;
	};

	auto draw = [&]()
	{
		wcf::clear({ ' ', {0,0,0} });

		if (r1.collideRect(r2))
			wcf::fillRect(r1, { 'X', {0, 1, 1} });
		else
			wcf::fillRect(r1, { 'X', {0, 0, 1} });

		wcf::fillRect(r2, { 'X', {1, 0, 1} });
	};

	wcf::setKeyCallback([](wcf::Key key, bool pressed)
		{
			if (key == wcf::Key::SPACE)
				std::cout << "Space!" << std::endl;
		});

	wcf::start(60.0f, update, draw);

	return 0;
}