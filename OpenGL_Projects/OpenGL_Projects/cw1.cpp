//#include <iostream>
//
//#define GLEW_STATIC
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//const GLint WIDTH = 800, HEIGHT = 600;
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <vector>
//
//#include <stdlib.h>
//#include <math.h>
//
//void init() {
//	glfwInit();
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//}
//
//int main() {
//	init();
//
//	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr, nullptr);
//
//	int screenWidth, screenHeight;
//	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
//
//	if (nullptr == window) {
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//
//		return EXIT_FAILURE;
//	}
//
//	glfwMakeContextCurrent(window);
//
//	glewExperimental = GL_TRUE;
//
//	if (GLEW_OK != glewInit()) {
//		std::cout << "Failed to initialise GLEW" << std::endl;
//		return EXIT_FAILURE;
//	}
//
//	glViewport(0, 0, screenWidth, screenHeight);
//
//	while (!glfwWindowShouldClose(window)) {
//		glfwPollEvents();
//		glClearColor(1.0f,0.1f,0.1f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		glfwSwapBuffers(window);
//	}
//
//	glfwTerminate();
//
//	return EXIT_SUCCESS;
//}