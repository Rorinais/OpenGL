#include"Application.h"
#include<glad/glad.h>
#include <GLFW/glfw3.h>

Application* Application::mInstance = nullptr;

Application* Application::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}
	return mInstance;
}

bool Application::init(const int& width,const int& height)
{
	mWidth = width;
	mHeight = height;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(mWidth, mHeight, "LearnOpenGL", NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);
	glfwSetKeyCallback(mWindow, keyBoardCallback);
	//glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
	glfwSetCursorPosCallback(mWindow, mouseCallback);
	glfwSetScrollCallback(mWindow, scrollCallback);

	glfwSetWindowUserPointer(mWindow, this);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool Application::update()
{

	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(mWindow, true);
	}
	if (glfwWindowShouldClose(mWindow))
	{
		return false;
	}
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
	return true;
}

void Application::destroy()
{
	glfwTerminate();
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mResizeCallback != nullptr) {
		self->mResizeCallback(width, height);
	}
}

void Application::keyBoardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mKeyBoardCallback != nullptr) {
		self->mKeyBoardCallback(key, scancode,action,mods);
	}
}

//void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
//{
//	Application* self = (Application*)glfwGetWindowUserPointer(window);
//	if (self->mMouseButtonCallback != nullptr) {
//		std::cout << "OnButton" << std::endl;
//		self->mMouseButtonCallback(button, action, mods);
//	}
//}

void Application::mouseCallback(GLFWwindow* window, double x, double y)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mMouseCallback != nullptr) {
		self->mMouseCallback( x, y);
	}
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mScrollCallback != nullptr) {
		self->mScrollCallback(xoffset, yoffset);
	}
}

Application::~Application()
{

}

Application::Application()
{

}