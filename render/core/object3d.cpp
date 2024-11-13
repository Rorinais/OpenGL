#include"../global/base.h"
#include"object3d.h"
#include "../tools/identity.h"

Object3d::Object3d()noexcept
{
	mId = Identity::generateID();
}

Object3d::~Object3d()noexcept
{

}

void Object3d::SetPosition(float x, float y, float z)noexcept
{
	SetPosition(glm::vec3(x, y, z));
}

void Object3d::SetPosition(const glm::vec3& position)noexcept
{
	mLocalMatrix[3].x = position.x;
	mLocalMatrix[3].y = position.y;
	mLocalMatrix[3].z = position.z;

	mPosition = position;
}

void Object3d::SetQuaternion(float x, float y, float z, float w)noexcept
{
	glm::quat quanternion(w,x, y, z);
	
	float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
	float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
	float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));

	glm::mat4 rotateMatrix = glm::mat4_cast(quanternion);

	mLocalMatrix[0] = rotateMatrix[0] * scaleX;
	mLocalMatrix[1] = rotateMatrix[1] * scaleY;
	mLocalMatrix[2] = rotateMatrix[2] * scaleZ;

	Decompose();
}

void Object3d::SetScale(float x, float y, float z)noexcept
{
	auto col0 = glm::normalize(glm::vec3(mLocalMatrix[0])) * x;
	auto col1 = glm::normalize(glm::vec3(mLocalMatrix[1])) * y;
	auto col2 = glm::normalize(glm::vec3(mLocalMatrix[2])) * z;

	mLocalMatrix[0] = glm::vec4(col0, 0.0f);
	mLocalMatrix[1] = glm::vec4(col1, 0.0f);
	mLocalMatrix[2] = glm::vec4(col2, 0.0f);

	Decompose();
}

void Object3d::RotateX(float angle)noexcept
{
	glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[0]);
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
	mLocalMatrix = rotateMatrix * mLocalMatrix;

	Decompose();

}

void Object3d::RotateY(float angle)noexcept
{
	glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[1]);
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
	mLocalMatrix = rotateMatrix * mLocalMatrix;

	Decompose();
}

void Object3d::RotateZ(float angle)noexcept 
{
	glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[2]);
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), rotateAxis);
	mLocalMatrix = rotateMatrix * mLocalMatrix;

	Decompose();
}

void Object3d::SetRotateAroundAxis(const glm::vec3& axis, float angle)noexcept
{
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), axis);

	float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
	float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
	float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scaleX, scaleY, scaleZ));

	mLocalMatrix[0] = rotateMatrix[0];
	mLocalMatrix[1] = rotateMatrix[1];
	mLocalMatrix[2] = rotateMatrix[2];

	mLocalMatrix *= scaleMatrix;

	Decompose();
}

void Object3d::RotateArroundAxis(const glm::vec3& axis, float angle)noexcept
{
	mLocalMatrix = glm::rotate(mLocalMatrix, glm::radians(angle), axis);

	Decompose();
}

void Object3d::LookAt(const glm::vec3& target, const glm::vec3& up)noexcept
{
	float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
	float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
	float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));

	glm::vec3 position = glm::vec3(mLocalMatrix[3]);

	auto nTarget = glm::normalize(target - position) * scaleX;
	auto nRight = glm::normalize(glm::cross(up, -nTarget)) * scaleY;
	auto nUp = glm::normalize(glm::cross(nRight, nTarget)) * scaleZ;

	mLocalMatrix[0] = glm::vec4(nRight, 0.0f);
	mLocalMatrix[1] = glm::vec4(nUp, 0.0f);
	mLocalMatrix[2] = glm::vec4(-nTarget, 0.0f);
	mLocalMatrix[3] = glm::vec4(position, 1.0f);

	Decompose();
}

void Object3d::SetLocalMatrix(const glm::mat4& localMatrix)noexcept
{
	mLocalMatrix = localMatrix;

	Decompose();
}

void Object3d::SetWorldMatrix(const glm::mat4& worldMatrix)noexcept
{
	mWorldMatrix = worldMatrix;

	Decompose();
}

void Object3d::AddChild(const Object3d::Ptr& child)noexcept
{
	if (child == shared_from_this())
	{
		return;
	}
	child->mParent = shared_from_this();

	auto iter = std::find(mChildren.begin(), mChildren.end(), child);

	if (iter != mChildren.end())return;

	mChildren.push_back(child);
}

const std::vector<Object3d::Ptr>& Object3d::GetChildren() const noexcept
{
	return mChildren;
}

void Object3d::UpdateMatrix()noexcept
{
	if (mNeedsUpdateMatrix)
	{
		mNeedsUpdateMatrix = false;
		auto translateMatrix = glm::translate(glm::mat4(1.0f), mPosition);
		auto rotateMatrix = glm::mat4_cast(mQuaternion);
		auto scaleMatrix = glm::scale(glm::mat4(1.0f), mScale);

		mLocalMatrix = translateMatrix * rotateMatrix * scaleMatrix;
	}
}

glm::mat4 Object3d::UpdateWorldMatrix(bool updateParent, bool updateChildren)noexcept
{
	if (!mParent.expired()&&updateParent)
	{
		auto parent = mParent.lock();
		parent->UpdateWorldMatrix(true, false);
	}

	UpdateMatrix();

	mWorldMatrix = mLocalMatrix;

	if (!mParent.expired())
	{
		auto parent = mParent.lock();
		mLocalMatrix = parent->mWorldMatrix * mWorldMatrix;
	}
	if (updateChildren)
	{
		for (auto& child : mChildren) {
			child->UpdateWorldMatrix(false, true);
		}
	}

	return mWorldMatrix;
}

glm::mat4 Object3d::UpdateModelViewMatrix(const glm::mat4& viewMatrix)noexcept
{
	mModelViewMatrix = viewMatrix * mWorldMatrix;
	return mModelViewMatrix;
}

glm::mat3 Object3d::UpdateNormalMatrix()noexcept
{
	mNormalMatrix = glm::transpose(glm::inverse(glm::mat3(mModelViewMatrix)));
	
	return mNormalMatrix;
}

glm::vec3 Object3d::GetPosition()const noexcept
{
	return glm::vec3(mLocalMatrix[3]);
}

glm::vec3 Object3d::GetWorldPosition() const noexcept
{
	return glm::vec3(mWorldMatrix[3]);
}

glm::vec3 Object3d::GetLocalDirection() const noexcept
{
	return glm::normalize(-glm::vec3(mLocalMatrix[2]));
}

glm::vec3 Object3d::GetWorldDirection() const noexcept
{
	return glm::normalize(-glm::vec3(mWorldMatrix[2]));
}

glm::vec3 Object3d::GetUp() const noexcept
{
	return glm::normalize(glm::vec3(mLocalMatrix[1]));
}

glm::vec3 Object3d::GetRight() const noexcept
{
	return glm::normalize(glm::vec3(mLocalMatrix[0]));
}

glm::mat4 Object3d::GetLocalMatrix() noexcept
{
	return mLocalMatrix;
}

glm::mat4 Object3d::GetWorldMatrix() noexcept
{
	return mWorldMatrix;
}

glm::mat4 Object3d::GetModelViewMatrix() noexcept
{
	return mModelViewMatrix;
}

glm::mat3 Object3d::GetNormalMatrix() noexcept
{
	return mNormalMatrix;
}

ID Object3d::GetId() const noexcept
{
	return mId;
}

void Object3d::Decompose() noexcept
{
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(mLocalMatrix, mScale, mQuaternion,mPosition ,skew, perspective);
}