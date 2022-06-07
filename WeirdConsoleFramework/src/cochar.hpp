#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace wcf
{
    struct CocharData
    {
        glm::vec2 uv;
        glm::vec3 fgColor;
    };
    
    struct Cochar
    {
        uint8_t chr;
        glm::vec3 col;
    };
}