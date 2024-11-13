#include"driverWindow.h"

DriverWindow::DriverWindow(Renderer* renderer, const int& width, const int& height)noexcept
{
	mWidth = width;
	mHeight = height;
	mRenderer = renderer;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);

	mWindow = glfwCreateWindow(mWidth, mHeight, "opengl", nullptr, nullptr);

	if (!mWindow) {
		std::cerr << "Error:failed to create window" << std::endl;
		glfwTerminate();
		exit(0);
	}
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(0);
	}

	glfwSetWindowUserPointer(mWindow, this);

	glfwSetFramebufferSizeCallback(mWindow, FrameSizeCallBack);

	glfwSetMouseButtonCallback(mWindow, MouseActionCallBack);

	glfwSetCursorPosCallback(mWindow, MouseMoveCallBack);

	//glfwSwapInterval(0);
}

DriverWindow::~DriverWindow()noexcept
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

bool DriverWindow::ProcessEvent()noexcept
{
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(mWindow))
	{
		glfwSetWindowShouldClose(mWindow, true);
		return false;
	}
	glfwPollEvents();

	bool needsNotify{ false };

	auto iter = KeyboardActionMap.begin();

	while (iter != KeyboardActionMap.end())
	{
		if (glfwGetKey(mWindow,iter->first) == GLFW_PRESS) {
			if (!mKeyState[iter->second])
			{
				mKeyState.set(iter->second);
				needsNotify = true;
			}
		}
		else if (glfwGetKey(mWindow, iter->first) == GLFW_RELEASE)
		{
			if (mKeyState[iter->second])
			{
				mKeyState.reset(iter->second);
				needsNotify = true;
			}
		}
		iter++;
	}
	if (mKeyboardActionCallback&&needsNotify)
	{
		mKeyboardActionCallback(mKeyState);
	}
	return true;
}

void DriverWindow::Swap()noexcept
{
	glfwSwapBuffers(mWindow);
}

void DriverWindow::SetFrameSizeCallBack(const FrameSizeCallback& callback)noexcept
{
	mFrameSizeCallback = callback;
}

void DriverWindow::SetMouseMoveCallBack(const MouseMoveCallback& callback)noexcept
{
	mMouseMoveCallback = callback;
}

void DriverWindow::SetMouseActionCallBack(const MouseActionCallback& callback)noexcept
{
	mMouseActionCallback = callback;
}

void DriverWindow::SetKeyboardActionCallBack(const KeyboardActionCallback& callback)noexcept
{
	mKeyboardActionCallback= callback;
}

void DriverWindow::FrameSizeCallBack(GLFWwindow* window, int width, int height)noexcept 
{
	auto self = static_cast<DriverWindow*>(glfwGetWindowUserPointer(window));
	self->mWidth = width;
	self->mHeight = height;

	if (self->mFrameSizeCallback)
	{
		self->mFrameSizeCallback(self, width, height);
	}
}

void DriverWindow::MouseMoveCallBack(GLFWwindow* window, double xpos, double ypos)noexcept
{
	auto self = static_cast<DriverWindow*>(glfwGetWindowUserPointer(window));
	if (self->mMouseMoveCallback)
	{
		self->mMouseMoveCallback(xpos, ypos);
	}
}

void DriverWindow::MouseActionCallBack(GLFWwindow* window, int button, int action, int mods)noexcept
{
	auto self = static_cast<DriverWindow*>(glfwGetWindowUserPointer(window));

	MouseAction mouseAction{ MouseAction::NONE };

	auto iter = MouseActionMap.begin();
	while (iter!=MouseActionMap.end())
	{
		if (action == GLFW_PRESS) {
			if (button == iter->first)
			{
				mouseAction = std::get<DOWN>(iter->second);
				break;
			}
		}
		else if (action==GLFW_RELEASE)
		{
			if (button == iter->first)
			{
				mouseAction = std::get<UP>(iter->second);
				break;
			}
		}
		iter++;
	}

	if (iter == MouseActionMap.end())
	{
		return;
	}
	if (self->mMouseActionCallback)
	{
		self->mMouseActionCallback(mouseAction);
	}
}