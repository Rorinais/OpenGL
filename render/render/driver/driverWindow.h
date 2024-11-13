#pragma once
#include"../../global/base.h"
#include"../../global/constant.h"

class Renderer;

class DriverWindow
{
public:
	using FrameSizeCallback = std::function<void(DriverWindow*pDriverWindow,int width,int height)>;
	using MouseMoveCallback = std::function<void(double xpos, double ypos)>;
	using KeyboardActionCallback = std::function<void(KeyBoardState)>;
	using MouseActionCallback = std::function<void(MouseAction)>;

	using Ptr = std::shared_ptr<DriverWindow>;

	static Ptr Create(Renderer* renderer, const int& width, const int& height)
	{
		return std::make_shared<DriverWindow>(renderer, width, height);
	}

	DriverWindow(Renderer* renderer, const int& width, const int& height)noexcept;

	~DriverWindow()noexcept;

	auto GetWindow()const noexcept { return mWindow; }

	bool ProcessEvent()noexcept;

	void Swap()noexcept;

	void SetFrameSizeCallBack(const FrameSizeCallback& callback)noexcept;

	void SetMouseMoveCallBack(const MouseMoveCallback& callback)noexcept;

	void SetMouseActionCallBack(const MouseActionCallback& callback)noexcept;

	void SetKeyboardActionCallBack(const KeyboardActionCallback& callback)noexcept;

private:
	static void FrameSizeCallBack(GLFWwindow* window, int width, int height)noexcept;
	static void MouseMoveCallBack(GLFWwindow* window, double xpos, double ypos)noexcept;
	static void MouseActionCallBack(GLFWwindow* window, int button, int action, int mods)noexcept;

public:
	Renderer* mRenderer = nullptr;
private:
	int mWidth{ 0 };
	int mHeight{ 0 };

	KeyBoardState	mKeyState{};

	FrameSizeCallback	mFrameSizeCallback{ nullptr };
	MouseMoveCallback	mMouseMoveCallback{ nullptr };
	KeyboardActionCallback	mKeyboardActionCallback{ nullptr };
	MouseActionCallback		mMouseActionCallback{ nullptr };

	GLFWwindow* mWindow{ NULL };
};