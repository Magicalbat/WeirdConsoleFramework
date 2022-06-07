#include "interface.hpp"

using namespace wcf;

OpenGL_Interface::OpenGL_Interface(uint32_t width, uint32_t height, uint32_t charWidth, uint32_t charHeight)
    :Interface(width, height, charWidth, charHeight), m_Translations(width * height), m_Cochars(width * height)
{
    m_ShaderProgram = loadShaderProgram("res/fragment.glsl", "res/vertex.glsl");
    glUseProgram(m_ShaderProgram);

	unsigned int uvStepLoc = glGetUniformLocation(shaderProg, "uUvStep");
	unsigned int modelProjLoc = glGetUniformLocation(shaderProg, "uProjection");

	glUniform2f(uvStepLoc, 1.0f / 225.0f, 1.0f);

	glm::mat4 orthoProj = glm::ortho(0.0f, (float)(WIDTH*CHAR_WIDTH), (float)(HEIGHT*CHAR_HEIGHT), 0.0f);// , -1.0f, 1.0f);
	glUniformMatrix4fv(modelProjLoc, 1, GL_TRUE, glm::value_ptr(orthoProj));

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			m_Translations[x + y * WIDTH] = { x * CHAR_WIDTH, y * CHAR_HEIGHT };
			m_Cochars[x + y * WIDTH] = { getCharUV((x + y * WIDTH) % 255), {0, 1, 0} };
		}
	}

	float positions[8] = {
		0.0f, 0.0f,
		charWidth, 0.0f,
		charWidth, charHeight,
		0.0f, charHeight
    }

    glGenBuffers(1, &m_PositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, &positionsn[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(0));

    glGenBuffers(1, &m_TranslationBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_TranslationBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * width * heigth, m_Translations.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(0));
	glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &m_Cocharbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_CocharBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CocharData) * width * heigth, m_Cochars.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CocharData), (void*)(0));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CocharData), (void*)(sizeof(glm::vec2)));
	glVertexAttribDivisor(3, 1);
}

OpenGL_Interface::~OpenGL_Interface()
{
    glDeleteProgram(m_ShaderProgram);
	glDeleteBuffers(1, &m_PositionBuffer);
	glDeleteBuffers(1, &m_TranslationBuffer);
	glDeleteBuffers(1, &m_CocharBuffer);
}

void OpenGL_Interface::DrawScreen(Screen& screen) override
{
    for (int y = 0; y < m_Height; y++)
    {
        for (int x = 0; x < m_Width; x++)
        {
            int index = x + y * m_Width;
            m_Cochars[index] = {getCharUV(screen[index].chr), screen[index].col};
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_CocharBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(CocharData) * m_Cochars.size(), m_Cochars.data());

    glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
    glDrawArraysInstanced(GL_QUADS, 0, 4, WIDTH*HEIGHT);
}

unsigned int loadShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    std::ifstream vStream(vertexPath);
	std::string vertexSource;
	if (vStream.is_open())
	{
		std::stringstream ss;
		ss << vStream.rdbuf();
		vertexSource = ss.str();
		vStream.close();
	}
	else
	{
		std::cout << "Failed to load shader at \"" << vertexPath << "\"." << std::endl;
		return 0;
	}

	std::ifstream fStream(fragmentPath);
	std::string fragmentSource;
	if (fStream.is_open())
	{
		std::stringstream ss;
		ss << fStream.rdbuf();
		fragmentSource = ss.str();
		fStream.close();
	}
	else
	{
		std::cout << "Failed to load shader at \"" << fragmentPath << "\"." << std::endl;
		return 0;
	}

	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	char const* vSourcePointer = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vSourcePointer, NULL);
	glCompileShader(vertexShader);

	int isCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int infoLogLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> errorMsg(infoLogLength);
		glGetShaderInfoLog(vertexShader, infoLogLength, &infoLogLength, &errorMsg[0]);
		std::cout << "Vertex Shader Failed to Compile: " << errorMsg.data() << std::endl;
	}

	char const* fSourcePointer = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fSourcePointer, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int infoLogLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> errorMsg(infoLogLength);
		glGetShaderInfoLog(fragmentShader, infoLogLength, &infoLogLength, &errorMsg[0]);
		std::cout << "Fragment Shader Failed to Compile: " << errorMsg.data() << std::endl;
	}

	uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> errorMsg(infoLogLength);
		glGetProgramInfoLog(vertexShader, infoLogLength, &infoLogLength, &errorMsg[0]);
		std::cout << "Shader Program Failed to Link: " << errorMsg.data() << std::endl;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}