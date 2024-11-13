#pragma once
#include"../math3d/math3d.h"
#include"../global/base.h"
#include"../core/object3d.h"

class Camera:public Object3d
{
public:
	Camera(float width, float height, GLFWwindow* window);

	Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up, float width, float height, GLFWwindow* window);

	bool OnKeyboard(GLFWwindow* window, int Key);

	void OnScroll(double xoffset, double yoffset);

	const Vector3f& GetPos()const
	{
		return m_pos;
	}

	const Vector3f& GetTarget()const
	{
		return m_target;
	}

	const Vector3f& GetUp()const
	{
		return m_up;
	}
	const float& GetFOV()const
	{
		return m_fov;
	}
	void SetFOV(float fov);

	void OnMouse(GLFWwindow* window,double x, double y);
private:
	void init();
	void Update();

	double m_AngleH{ 0 };
	double m_AngleV{ 0 };

	double m_mousePosX{ 0 };
	double m_mousePosY{ 0 };

	float m_fov{45.0f};

	float m_WindowWidth{ 0 };
	float m_WindowHeight{ 0 };
	
	GLFWwindow* window;

	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;

	Vector3f Right;
	Vector3f Left;

public:
	using Ptr = std::shared_ptr<Camera>;

	Camera()noexcept;
	~Camera()noexcept;

	glm::mat4 UpdateWorldMatrix(bool updateParent = false, bool updateChildren = false)noexcept override;
	glm::mat4 GetWorldMatrixInverse()noexcept { return mWorldMatrixInverse; }
	glm::mat4 GetProjectionMatrix()noexcept { return mProjectionMatrix; }
	virtual glm::mat4 UpdateProjectionMatrix()noexcept { return mProjectionMatrix; }

protected:
	glm::mat4 mWorldMatrixInverse = glm::mat4(1.0f);
	glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
};
