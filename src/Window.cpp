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
	    	std::cerr << "Can't initialize GLFW!\n";

	glfwWindowHint(GLFW_SAMPLES, 4);
	// set the version of openGL to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// set GLFW to use the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::setWindowProp(GLuint target, GLuint value)
{
	glfwWindowHint(target, value);
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

bool Window::createWindow(unsigned int width, unsigned int height, string title)
{

	// create window and return a pointer to the created window
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if( window == NULL )
	{
		std::cerr << "Failed to open GLFW window\n";
		return false;
	}

	// make the context of this window the current to be called
	glfwMakeContextCurrent(window);

	prepare_glew();

	VIEW_WIDTH  = width;
	VIEW_HEIGHT = height;

	return true;
}

void enable_property( GLenum property )
{
	glEnable(property);
}

void disable_property( GLenum property )
{
	glDisable(property);
}

void Window::set_viewport(unsigned int width, unsigned int height)
{
	this->VIEW_WIDTH = width;
	this->VIEW_HEIGHT = height;
}

void Window::prepare_glew()
{
    glewExperimental = true;

    if( glewInit() != GLEW_OK )
		std::cerr << "failed to init GLEW\n";

    glViewport(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
}

//void Window::glMainLoop()
//{
//    while(glfwWindowShouldClose(window) == GL_FALSE)
//    {
//    	glfwPollEvents();
//		glfwSwapBuffers(window);
//    }
//}

Window::~Window() {
	// TODO Auto-generated destructor stub
	glfwTerminate();
}
