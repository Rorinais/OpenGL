#pragma once
#include"../global/base.h"

class ObjectTypeChecker
{
public:
	bool mIsRenderableObject{ false };
	bool mIsMesh{ false };
	bool mIsSkinnedMesh{ false };
	bool mIsBone{ false };
	bool mIsScene{ false };
	bool mIsCamera{ false };
	bool mIsOrthographicCamera{ false };
	bool mIsPerpectiveCamera{ false };
	bool mIsGroup{ false };
	bool mIsLight{ false };
	bool mIsAmbientLight{ false };
	bool mIsDirectionalLight{ false };
};

class Object3d :public std::enable_shared_from_this<Object3d>, public ObjectTypeChecker
{
public:
	using Ptr = std::shared_ptr<Object3d>;
	static Ptr Create()
	{
		return std::make_shared<Object3d>();
	}

	Object3d()noexcept;
	~Object3d()noexcept;

	virtual void fakeFunction()noexcept{}

	void SetPosition(float x, float y, float z)noexcept;

	void SetPosition(const glm::vec3& position)noexcept;

	void SetQuaternion(float x, float y, float z, float w)noexcept;

	void SetScale(float x, float y, float z)noexcept;

	void RotateX(float angle)noexcept;

	void RotateY(float angle)noexcept;

	void RotateZ(float angle)noexcept;

	void SetRotateAroundAxis(const glm::vec3& axis, float angle)noexcept;

	void RotateArroundAxis(const glm::vec3& axis, float angle)noexcept;

	void LookAt(const glm::vec3& target, const glm::vec3& up)noexcept;

	void SetLocalMatrix(const glm::mat4& localMatrix)noexcept;

	void SetWorldMatrix(const glm::mat4& worldMatrix)noexcept;

	void AddChild(const Object3d::Ptr& child)noexcept;

	virtual void UpdateMatrix()noexcept;

	virtual glm::mat4 UpdateWorldMatrix(bool updateParent = false, bool updateChildren = false)noexcept;

	glm::mat4 UpdateModelViewMatrix(const glm::mat4& viewMatrix)noexcept;

	glm::mat3 UpdateNormalMatrix()noexcept;

	glm::vec3 GetPosition()const noexcept;

	glm::vec3 GetWorldPosition() const noexcept;

	glm::vec3 GetLocalDirection() const noexcept;

	glm::vec3 GetWorldDirection() const noexcept;

	glm::vec3 GetUp() const noexcept;

	glm::vec3 GetRight() const noexcept;

	glm::mat4 GetLocalMatrix() noexcept;

	glm::mat4 GetWorldMatrix() noexcept;

	glm::mat4 GetModelViewMatrix() noexcept;

	glm::mat3 GetNormalMatrix() noexcept;

	const std::vector<Object3d::Ptr>& GetChildren() const noexcept;

	ID GetId() const noexcept;

protected:
	void Decompose() noexcept;

public:
	bool mVisible{ true };
	bool mCastShadow{ true };
	std::string mName;
	bool mNeedsUpdateMatrix{ true };

protected:
	ID mId{ 0 };

	glm::vec3 mPosition{ glm::vec3(0.0f) };
	glm::quat mQuaternion{ glm::quat(1.0f,0.0f,0.0f,0.0f) };
	glm::vec3 mScale{ glm::vec3(1.0f) };
	glm::mat4 mLocalMatrix = glm::mat4(1.0f);
	glm::mat4 mWorldMatrix = glm::mat4(1.0f);

	bool mNeedsUpdata{ false };

	std::weak_ptr<Object3d> mParent;
	std::vector<Object3d::Ptr> mChildren{};

	glm::mat4 mModelViewMatrix= glm::mat4(1.0f);
	glm::mat3 mNormalMatrix = glm::mat3(1.0f);
};