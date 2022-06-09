#include "wcf.hpp"

#include <iostream>

static wcf::Internal::InternalData sData;

void wcf::init(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight)
{
    sData = {
        width, height, charWidth, charHeight,
        std::vector<Cochar>(width*height),
        std::make_shared<OpenGL_Interface>(width, height, charWidth, charHeight)
    };

    for (auto& item : sData.screen)
    {
        item = { ' ', {1, 1, 1} };
    }
}

void wcf::start(float fps, std::function<void(float delta)> update, std::function<void()> draw)
{
    float delta = 1.0f / fps;
    float accumulator = 0.0f;

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (sData.interface->running())
    {
		auto newTime = std::chrono::high_resolution_clock::now();
		auto frameTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime);
        float frameTime = (float)frameTimeMS.count() * 0.001f;

        currentTime = newTime;

        accumulator += frameTime;
        while (accumulator >= delta)
        {
            update(delta);

            accumulator -= delta;
        }

        draw();
        sData.interface->drawScreen(sData.screen);
    }
}

wcf::Cochar& wcf::getCochar(int x, int y)
{
    return sData.screen[x + y * sData.width];
}

void wcf::setCochar(int x, int y, wcf::Cochar cochar)
{
    sData.screen[x + y * sData.width] = cochar;
}

uint32_t wcf::width()        { return sData.width; }
uint32_t wcf::height()       { return sData.height; }
uint32_t wcf::charWidth()    { return sData.charWidth; }
uint32_t wcf::charHeight()   { return sData.charHeight; }