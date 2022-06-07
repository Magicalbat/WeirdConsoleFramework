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

        virtual void DrawScreen(Screen& screen);
        virtual void SetCochar(int x, int y, Cochar cochar);
    
    protected:
        uint32_t m_Width, m_Height, m_CharWidth, m_CharHeight;
    };

    class OpenGL_Interface : public Interface
    {
    public:
        OpenGL_Interface(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight);
        ~OpenGL_Interface();

        void DrawScreen(Screen& screen) override;
        void SetCochar(int x, int y, Cochar cochar) override;

    private:

        unsigned int m_ShaderProgram;

        unsigned int m_PositionBuffer, m_TranslationBuffer, m_CocharBuffer;
    }
}