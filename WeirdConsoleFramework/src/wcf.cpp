#include "wcf.hpp"

using namespace wcf;

static Internal::InternalData sData;

void init(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight)
{
    sData = {
        width, height, charWidth, charHeight,
        std::vector<Cochar>(width*height),
        std::make_shared<OpenGL_Interface>(width, height, charWidth, charHeight)
    };
}

void start(float fps, std::function<void(float delta)> update, std::function<void()> draw)
{
    float delta = 1.0f / delta;
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
            update();

            accumulator -= delta;
        }

        draw();
        sData.interface->drawScreen(sData.screen);

    }
}

uint32_t width()       { return sData.width; }
uint32_t height()      { return sData.height; }
uint32_t charWidth()   { return sData.charWidth; }
uint32_t charHeight()  { return sData.charHeight; }