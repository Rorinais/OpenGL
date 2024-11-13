#pragma once
#include "../global/base.h"
#include "../pipeline/Camera.h"
#include "../core/object3d.h"
#include "../object/mesh.h"
#include "../scene/scene.h"
#include"../object/group.h"
#include "renderTarget.h"
#include "driver/driverAttributes.h"
#include "driver/driverBindingState.h"
#include "driver/driverPrograms.h"
#include "driver/driverWindow.h"
#include "driver/driverRenderList.h"
#include "driver/driverTextures.h"
#include "driver/driverObjects.h"
#include "driver/driverInfo.h"
#include "driver/driverState.h"
#include "driver/driverMaterials.h"
#include "driver/driverBackground.h"
#include "driver/driverRenderState.h"
#include "driver/driverRenderTargets.h"
#include "driver/driverShadowMap.h"
#include "../math3d/frustum.h"

class Renderer :public std::enable_shared_from_this<Renderer>
{
public:
	friend class DriverShadowMap;

	struct Descriptor
	{
		uint32_t mWidth{ 800 };
		uint32_t mHeight{ 600 };
	};

	using OnSizeCallback = std::function<void(int width, int height)>;

	using Ptr = std::shared_ptr<Renderer>;

	static Ptr Create(const Descriptor& descriptor)
	{
		return std::make_shared<Renderer>(descriptor);
	}

	Renderer(const Descriptor& descriptor)noexcept;

	~Renderer()noexcept;

	bool Render(Scene::Ptr scene, Camera::Ptr camera);

	void Swap() noexcept;

	void SetSize(int width, int height) noexcept;

	void SetRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept;

	void SetFrameSizeCallBack(const OnSizeCallback& callback) noexcept;

	void SetMouseMoveCallBack(const DriverWindow::MouseMoveCallback& callback) noexcept;

	void SetMouseActionCallBack(const DriverWindow::MouseActionCallback& callback) noexcept;

	void SetKeyboardActionCallBack(const DriverWindow::KeyboardActionCallback& callback) noexcept;

	RenderTarget::Ptr GetRenderTarget() const noexcept;

	void SetClearColor(float r, float g, float b, float a) noexcept;

	glm::vec4 GetClearColor() const noexcept;

	void EnableShadow(bool enable) noexcept;

	void Clear(bool color = true, bool depth = true, bool stencil = true) noexcept;

public:
	bool mAutoClear{ true };

private:

	void ProjectObject(const Object3d::Ptr& object, uint32_t groupOrder, bool sortObjects) noexcept;

	void RenderScene(const DriverRenderList::Ptr& currentRenderList, const Scene::Ptr& scene, const Camera::Ptr& camera) noexcept;

	void RenderObjects(const std::vector<RenderItem::Ptr>& renderItems,const Scene::Ptr& scene,const Camera::Ptr& camera) noexcept;

	void RenderObject(const RenderableObject::Ptr& object,const Scene::Ptr& scene,
	const Camera::Ptr& camera,
	const Geometry::Ptr& geometry,
	const Material::Ptr& material) noexcept;

	void RenderBufferDirect(
	const RenderableObject::Ptr& object,
	const Scene::Ptr& scene,
	const Camera::Ptr& camera,
	const Geometry::Ptr& geometry,
	const Material::Ptr& material) noexcept;

	DriverProgram::Ptr SetProgram(
		const Camera::Ptr& camera,
		const Scene::Ptr& scene,
		const Geometry::Ptr& geometry,
		const Material::Ptr& material,
		const RenderableObject::Ptr& object) noexcept;

	DriverProgram::Ptr GetProgram(
		const Material::Ptr& material,
		const Scene::Ptr& scene,
		const RenderableObject::Ptr& object) noexcept;

	void UpdateCommonMaterialProperties(
		const Material::Ptr& material,
		const DriverProgram::Parameters::Ptr& parameters) noexcept;

	bool MaterialNeedsLights(const Material::Ptr& material) noexcept;

	void MakeLightsNeedUpdate(UniformHandleMap& lightsUniformMap) noexcept;

private:
	int mWidth{ 800 };
	int mHeight{ 600 };

	OnSizeCallback mOnSizeCallback{ nullptr };

	bool		mSortObject{ true };

	glm::mat4	mCurrentViewMatrix = glm::mat4(1.0f);

	glm::vec4	mViewport{};

	RenderTarget::Ptr	mCurrentRenderTarget{ nullptr };

	DriverWindow::Ptr		mWindow{ nullptr };
	DriverRenderList::Ptr	mRenderList{ nullptr };
	DriverTextures::Ptr		mTextures{ nullptr };
	DriverAttributes::Ptr   mAttributes{ nullptr };
	DriverInfo::Ptr			mInfos{ nullptr };
	DriverState::Ptr		mState{ nullptr };
	DriverObjects::Ptr		mObjects{ nullptr };
	DriverGeometries::Ptr	mGeometries{ nullptr };
	DriverPrograms::Ptr		mPrograms{ nullptr };
	DriverBindingStates::Ptr	mBindingStates{ nullptr };
	DriverMaterials::Ptr	mMaterials{ nullptr };
	DriverBackground::Ptr	mBackground{ nullptr };
	DriverRenderState::Ptr	mRenderState{ nullptr };
	DriverRenderTargets::Ptr mRenderTargets{ nullptr };
	DriverShadowMap::Ptr	mShadowMap{ nullptr };

	Frustum::Ptr			mFrustum{ nullptr };

	Scene::Ptr				mDummyScene = Scene::Create();
};