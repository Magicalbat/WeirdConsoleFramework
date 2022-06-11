#include "wcf.hpp"

#include <iostream>

using namespace wcf;

static Internal::InternalData s_Data;

void wcf::init(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight)
{
    s_Data = {
        width, height, charWidth, charHeight,
        std::vector<Cochar>(width*height),
        std::make_shared<OpenGL_Interface>(width, height, charWidth, charHeight)
    };

    for (auto& item : s_Data.screen)
    {
        item = { ' ', {1, 1, 1} };
    }
}

void wcf::start(float fps, std::function<void(float delta)> update, std::function<void()> draw)
{
    float delta = 1.0f / fps;
    float accumulator = 0.0f;

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (s_Data.interface->running())
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
        s_Data.interface->drawScreen(s_Data.screen);
    }
}

void wcf::setKeyCallback(std::function<void(Key key, bool pressed)> callback)
{
    s_Data.interface->setKeyCallback(callback);
}

Vector2 wcf::getMousePos()
{
    wcf::Vector2 screenPos = s_Data.interface->getMousePos();
    return {
        fmaxf(0.0f, fminf((float)s_Data.width, screenPos.x / s_Data.charWidth)),
        fmaxf(0.0f, fminf((float)s_Data.height, screenPos.y / s_Data.charHeight)),
        
    };
}

Cochar& wcf::getCochar(int x, int y)
{
    return s_Data.screen[x + y * s_Data.width];
}

void wcf::clear(Cochar cochar)
{
    for (auto& cc : s_Data.screen)
        cc = cochar;
}

void wcf::drawCochar(int x, int y, Cochar cochar)
{
    if (x > 0 && x < s_Data.width && y > 0 && y < s_Data.height)
        s_Data.screen[x + y * s_Data.width] = cochar;
}

void wcf::drawCochar(Vector2 pos, Cochar cochar)
{
    if (pos.x > 0 && pos.x < s_Data.width && pos.y > 0 && pos.y < s_Data.height)
        s_Data.screen[(int)pos.x + (int)pos.y * s_Data.width] = cochar;
}

void wcf::fillRect(int x, int y, int w, int h, Cochar cochar)
{
    for (int x1 = x; x1 < x + w; x1++)
        for (int y1 = y; y1 < y + h; y1++)
            drawCochar(x1, y1, cochar);
            //s_Data.screen[x1 + y1 * s_Data.width] = cochar;
}

void wcf::fillRect(Rect rect, Cochar cochar)
{
    for (int x = rect.x; x < rect.x + rect.w; x++)
        for (int y = rect.y; y < rect.y + rect.h; y++)
            drawCochar(x, y, cochar);
            //s_Data.screen[x + y * s_Data.width] = cochar;
}

uint32_t wcf::width()        { return s_Data.width; }
uint32_t wcf::height()       { return s_Data.height; }
uint32_t wcf::charWidth()    { return s_Data.charWidth; }
uint32_t wcf::charHeight()   { return s_Data.charHeight; }