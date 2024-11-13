#pragma once
#include<iostream>
#define App Application::GetInstance()

struct GLFWwindow;

using ResizeCallback = void(*)(int width, int height);
using KeyBoardCallback = void(*)(int key,int scancode,int action, int mods);
//using MouseButtonCallback = void(*)(int key, int action, int mods);
using MouseCallback = void(*)(double x, double y);
//using ScrollCallback = void(*)(double x, double y);

class Application
{
public:
	static Application* GetInstance();

	bool init(const int &width=800,const int &height=600);
	bool update();
	void destroy();

	void setResizeCallback(ResizeCallback callback) { mResizeCallback = callback; }
	void setKeyBoardCallback(KeyBoardCallback callback) { mKeyBoardCallback = callback; }
	//void setMouseButtonCallback(MouseButtonCallback callback) { mMouseButtonCallback = callback; }
	void setMouseCallback(MouseCallback callback) { mMouseCallback = callback; }
	void setScrollCallback(MouseCallback callback) { mScrollCallback = callback; }

	uint32_t GetWidth()const { return mWidth; }
	uint32_t GetHeight()const { return mHeight; }

	GLFWwindow* mWindow{ nullptr };
private:
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void keyBoardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseCallback(GLFWwindow* window, double x, double y);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	ResizeCallback mResizeCallback{ nullptr };
	KeyBoardCallback mKeyBoardCallback{ nullptr };
	//MouseButtonCallback mMouseButtonCallback{ nullptr };
	MouseCallback mMouseCallback{ nullptr };
	MouseCallback mScrollCallback{ nullptr };
private:
	uint32_t mWidth{ 0 };
	uint32_t mHeight{ 0 };

	static Application* mInstance;

	Application();
	~Application();
};