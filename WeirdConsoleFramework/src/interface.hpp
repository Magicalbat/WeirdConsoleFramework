#pragma once

#include <vector>

#include "cochar.hpp"

namespace wcf
{
    typedef std::vector<Cochar> Screen;

    // Wrappers for specific graphical APIs
    // For exapmle, there will be an OpenGL and maybe a DirectX interface
    // They will handle all graphics and input
    class Interface
    {
    public:
        Interface(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight)
            :m_Width(width), m_Height(height), m_CharWidth(charWidth), m_CharHeight(charHeight)
        {}

        virtual void drawScreen(Screen& screen) {}
    
    protected:
        uint32_t m_Width, m_Height, m_CharWidth, m_CharHeight;
    };

    class OpenGL_Interface : public Interface
    {
    public:

        OpenGL_Interface(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight);
        ~OpenGL_Interface();

        virtual void drawScreen(Screen& screen) override;

    private:

        unsigned int loadShaderProgram(const char* vertexPath, const char* fragmentPath);
        inline glm::vec2 getCharUV(uint8_t c)
        {
			uint32_t index = c - 31;
			if (index > 0 && index <= 224)
				return { (float)index * (1.0f / 225.0f), 0.0f};
			return { 0.0f, 0.0f };
		}

    private:

        unsigned int m_ShaderProgram;

        unsigned int m_PositionBuffer, m_TranslationBuffer, m_CocharBuffer;

        std::vector<glm::vec2> m_Translations;
        std::vector<CocharData> m_Cochars;
    };
}