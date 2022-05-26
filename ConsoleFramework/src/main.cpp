#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Console Framework", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	
	std::cout << "\n\x1b[36mVersion: " << glGetString(GL_VERSION) << ", Renderer: " << glGetString(GL_RENDERER) << "\x1b[0m" << std::endl;

	glClearColor(0, 1, 1, 1);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}