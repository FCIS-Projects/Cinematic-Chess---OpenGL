/*
 * Window.cpp
 *
 *  Created on: May 5, 2016
 *      Author: sara
 */

#include "Window.h"

Window::Window()
{
	// check if initialize GLFW library is successful
	if( !glfwInit() )
	    	std::cerr << "OH NO O_O\n";
}

void Window::setWindowProp(int target, int value)
{
	glfwWindowHint(target, value);

//	// set the version of openGL to 3.3
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//
//	// set GLFW to use the core profile
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	// set the window to not to be resizable by the user
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}


//
//bool Window::checkWindow(GLFWwindow* window)
//{
//	if (window == nullptr)
//	{
//	    glfwTerminate();
//	    return false;g
//	}
//	return true;
//}

GLFWwindow* Window::creatWindow(int width, int hight, const char* title)
{

	// create window and return a pointer to the created window
	GLFWwindow* window = glfwCreateWindow(width, hight, title, NULL, NULL);

	// make the context of this window the current to be called
	glfwMakeContextCurrent(window);

	prepare_glew();

	return window;
}

void Window::prepare_glew()
{
    glewExperimental = true;

    if( glewInit() != GLEW_OK )
		std::cerr << "failed to init GLEW\n";

    glViewport(0, 0, 800, 600);
}

void Window::glMainLoop(GLFWwindow* window)
{
    while(!glfwWindowShouldClose(window))
    {
    	glfwPollEvents();
		glfwSwapBuffers(window);
    }
}

Window::~Window() {
	// TODO Auto-generated destructor stub
	glfwTerminate();
}
