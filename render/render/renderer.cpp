#include"renderer.h"

static void OnFrameSizeCallback(DriverWindow* dwindow, int width, int height) {
	if (dwindow->mRenderer != nullptr) {
		dwindow->mRenderer->SetSize(width, height);
	}
}

Renderer::Renderer(const Descriptor& descriptor)noexcept
{
	mWidth = descriptor.mWidth;
	mHeight=descriptor.mHeight;

	mViewport = { 0.0f,0.0f,mWidth,mHeight };

	mWindow = DriverWindow::Create(this, mWidth, mHeight);
	mWindow->SetFrameSizeCallBack(OnFrameSizeCallback);

	mInfos = DriverInfo::Create();
	mRenderList = DriverRenderList::Create();
	mAttributes = DriverAttributes::Create();
	mState = DriverState::Create();
	mBindingStates = DriverBindingStates::Create(mAttributes);
	mGeometries = DriverGeometries::Create(mAttributes, mInfos, mBindingStates);
	mObjects = DriverObjects::Create(mGeometries, mAttributes, mInfos);
	mPrograms = DriverPrograms::Create();
	mMaterials = DriverMaterials::Create(mPrograms);
	mBackground = DriverBackground::Create(this, mObjects);
	mRenderState = DriverRenderState::Create();
	mRenderTargets = DriverRenderTargets::Create();
	mTextures = DriverTextures::Create(mInfos, mRenderTargets);
	mShadowMap = DriverShadowMap::Create(this, mObjects, mState);

	mFrustum = Frustum::Create();
}

Renderer::~Renderer()noexcept{}

bool Renderer::Render(Scene::Ptr scene,Camera::Ptr camera)
{
	if (!mWindow->ProcessEvent())
	{
		return false;
	}

	if (mCurrentRenderTarget && mCurrentRenderTarget->mNeedsUpdate)
	{
		mCurrentRenderTarget->mNeedsUpdate = false;
		mTextures->SetupRenderTarget(mCurrentRenderTarget);
	}

	if (scene == nullptr) { scene = mDummyScene; }

	scene->UpdateWorldMatrix(true, true);
	camera->UpdateWorldMatrix(true, true);

	auto projectionMatrix = camera->GetProjectionMatrix();
	auto cameraInverseMatrix = camera->GetWorldMatrixInverse();

	mCurrentViewMatrix = projectionMatrix*cameraInverseMatrix;
	mFrustum->SetFormProjectionMatrix(mCurrentViewMatrix);

	mRenderState->Init();
	mRenderList->Init();

	ProjectObject(scene, 0, mSortObject);

	mRenderList->Finish();

	if (mSortObject)
	{
		mRenderList->Sort();
	}

	mInfos->Reset();

	mRenderState->setupLights();

	mShadowMap->render(mRenderState,scene,camera);

	mBackground->render(mRenderList,scene);

	RenderScene(mRenderList,scene,camera);

	return true;
}

void Renderer::ProjectObject(const Object3d::Ptr& object, uint32_t groupOrder, bool sortObjects) noexcept
{
	if (!object->mVisible) return;

	glm::vec4 toolVec(1.0f);
 
	if (object->mIsGroup) {
		auto group = std::static_pointer_cast<Group>(object);
		groupOrder = group->mGroupOrder;
	}
	else if (object->mIsLight) {
		auto light = std::static_pointer_cast<Light>(object);
		mRenderState->pushLight(light);
		if (light->mCastShadow) {
			mRenderState->pushShadow(light);
		}
	}
	else if (object->mIsRenderableObject) {

		if (object->mIsSkinnedMesh) {
			auto skinnedMesh = std::dynamic_pointer_cast<SkinnedMesh>(object);
			skinnedMesh->mSkeleton->update();
		}

		if (mSortObject) {
			toolVec = glm::vec4(object->GetWorldPosition(), 1.0);
			toolVec = mCurrentViewMatrix * toolVec;
		}

		auto renderableObject = std::static_pointer_cast<RenderableObject>(object);


		if (mFrustum->IntersectObject(renderableObject)) {

			auto geometry = mObjects->Update(renderableObject);

			auto material = renderableObject->GetMaterial();

			mRenderList->Push(
				renderableObject,
				geometry,
				material,
				groupOrder,
				toolVec.z);
		}
	}

	auto children = object->GetChildren();
	for (auto& child : children) {
		ProjectObject(child, groupOrder, sortObjects);
	}
}

void Renderer::RenderScene(const DriverRenderList::Ptr& currentRenderList, const Scene::Ptr& scene, const Camera::Ptr& camera) noexcept
{
	const auto opaqueObjects = currentRenderList->GetOpaques();
	const auto transparentObjects = currentRenderList->GetTransparents();

	mRenderState->SetupLightsView(camera);

	mState->Viewport(mViewport);

	if (!opaqueObjects.empty())RenderObjects(opaqueObjects, scene, camera);
	if (!transparentObjects.empty())RenderObjects(transparentObjects, scene, camera);
}

void Renderer::RenderObjects(const std::vector<RenderItem::Ptr>& renderItems, const Scene::Ptr& scene, const Camera::Ptr& camera) noexcept
{
	const auto overrideMaterial = scene->mIsScene ? scene->mOverrideMaterial : nullptr;

	for (const auto& renderItem : renderItems)
	{
		const auto object = renderItem->mObject;
		const auto geometry = renderItem->mGeometry;
		const auto material = overrideMaterial == nullptr ? renderItem->mMaterial : overrideMaterial;

		RenderObject(object, scene, camera, geometry, material);
	}
}

void Renderer::RenderObject(const RenderableObject::Ptr& object, const Scene::Ptr& scene,
	const Camera::Ptr& camera,
	const Geometry::Ptr& geometry,
	const Material::Ptr& material) noexcept{

	object->OnBeforeRender(this, scene.get(), camera.get());

	object->UpdateModelViewMatrix(camera->GetWorldMatrixInverse());
	object->UpdateNormalMatrix();

	RenderBufferDirect(object, scene, camera, geometry, material);
}

void Renderer::RenderBufferDirect(
	const RenderableObject::Ptr& object,
	const Scene::Ptr& scene,
	const Camera::Ptr& camera,
	const Geometry::Ptr& geometry,
	const Material::Ptr& material) noexcept {
	auto _scene = scene;
	if (_scene == nullptr) _scene = mDummyScene;

	auto index = geometry->GetIndex();
	auto position = geometry->GetAttribute("position");

	auto program = SetProgram(camera, _scene, geometry, material, object);

	mState->setMaterial(material);


	mBindingStates->Setup(geometry, index);


	if (index) {
		glDrawElements(toGL(material->mDrawMode), index->GetCount(), toGL(index->GetDataType()), 0);
	}
	else {
		glDrawArrays(toGL(material->mDrawMode), 0, position->GetCount());
	}
}

DriverProgram::Ptr Renderer::SetProgram(
	const Camera::Ptr& camera,
	const Scene::Ptr& scene,
	const Geometry::Ptr& geometry,
	const Material::Ptr& material,
	const RenderableObject::Ptr& object
) noexcept {
	auto lights = mRenderState->mLights;

	bool needsProgramChange = false;

	auto dMaterial = mMaterials->Get(material);

	if (material->mVersion!=dMaterial->mVersion)
	{
		if (material->mDiffuseMap != dMaterial->mDiffuseMap) {
			if (material->mDiffuseMap == nullptr || dMaterial->mDiffuseMap == nullptr) {
				needsProgramChange = true;
			}
		}

		if (material->mEnvMap != dMaterial->mEnvMap) {
			if (material->mEnvMap == nullptr || dMaterial->mEnvMap == nullptr) {
				needsProgramChange = true;
			}
		}

		if (material->mNormalMap != dMaterial->mNormalMap) {
			if (material->mNormalMap == nullptr || dMaterial->mNormalMap == nullptr) {
				needsProgramChange = true;
			}
		}

		if (material->mSpecularMap != dMaterial->mSpecularMap) {
			if (material->mSpecularMap == nullptr || dMaterial->mSpecularMap == nullptr) {
				needsProgramChange = true;
			}
		}

		if (dMaterial->mLightsStateVersion != lights->mState.mVersion) {
			needsProgramChange = true;
		}

		if (object->mIsSkinnedMesh != dMaterial->mSkinning) {
			dMaterial->mSkinning = object->mIsSkinnedMesh;
			needsProgramChange = true;
		}

		if (object->mIsSkinnedMesh) {
			auto skinnedMesh = std::dynamic_pointer_cast<SkinnedMesh>(object);
			if (skinnedMesh->mSkeleton->mBones.size() != dMaterial->mMaxBones) {
				needsProgramChange = true;
				dMaterial->mMaxBones = skinnedMesh->mSkeleton->mBones.size();
			}
		}
	}
	else {
		needsProgramChange = true;
		dMaterial->mVersion = material->mVersion;
	}

	auto dprogram = dMaterial->mCurrentProgram;
	if (needsProgramChange) {
		
		dprogram = GetProgram(material, scene, object);
	}

	bool refreshProgram = false;
	if (mState->UseProgram(dprogram->mProgram)) {
		refreshProgram = true;
	}

	//----------------------------------Uniforms-----------------------------------------

	auto uniforms = dMaterial->mUniforms;

	
	DriverMaterials::RefreshMaterialUniforms(uniforms, material);

	bool needsLights = MaterialNeedsLights(material);

	auto& lightUniforms = mRenderState->mLights->mState.mLightUniformHandles;

	if (needsLights) {
		MakeLightsNeedUpdate(lightUniforms);
		uniforms.insert(lightUniforms.begin(), lightUniforms.end());
	}

	if (object->mIsSkinnedMesh) {
		auto skinnedMesh = std::dynamic_pointer_cast<SkinnedMesh>(object);
		auto skeleton = skinnedMesh->mSkeleton;
		uniforms.insert(skeleton->mUniforms.begin(), skeleton->mUniforms.end());
	}

	uniforms["modelViewMatrix"].mValue = object->GetModelViewMatrix();
	uniforms["modelViewMatrix"].mNeedsUpdate = true;

	uniforms["normalMatrix"].mValue = object->GetNormalMatrix();
	uniforms["normalMatrix"].mNeedsUpdate = true;

	uniforms["projectionMatrix"].mValue = camera->GetProjectionMatrix();
	uniforms["projectionMatrix"].mNeedsUpdate = true;

	uniforms["modelMatrix"].mValue = object->GetWorldMatrix();
	uniforms["modelMatrix"].mNeedsUpdate = true;

	DebugLog::GetInstance()->BeginUpLoad(material->GetType());

	dprogram->UploadUniforms(uniforms, mTextures);

	DebugLog::GetInstance()->End();

	return dprogram;
}

DriverProgram::Ptr Renderer::GetProgram(
	const Material::Ptr& material,
	const Scene::Ptr& scene,
	const RenderableObject::Ptr& object
) noexcept {
	DriverProgram::Ptr program = nullptr;

	auto dMaterial = mMaterials->Get(material);
	auto lights = mRenderState->mLights;

	auto& programs = dMaterial->mPrograms;

	auto parameters = mPrograms->GetParameters(material, object, lights, mShadowMap);

	auto cacheKey = mPrograms->GetProgramCacheKey(parameters);

	auto pIter = programs.find(cacheKey);
	if (pIter != programs.end()) {
		dMaterial->mCurrentProgram = pIter->second;
		dMaterial->mUniforms = mPrograms->GetUniforms(material);
		program = pIter->second;
	}
	else {
		dMaterial->mUniforms = mPrograms->GetUniforms(material);

		program = mPrograms->AcquireProgram(parameters, cacheKey);

		programs.insert(std::make_pair(cacheKey, program));

		dMaterial->mCurrentProgram = program;
	}

	UpdateCommonMaterialProperties(material, parameters);

	dMaterial->mNeedsLight = MaterialNeedsLights(material);
	dMaterial->mLightsStateVersion = lights->mState.mVersion;

	return program;
}

void Renderer::UpdateCommonMaterialProperties(
	const Material::Ptr& material,
	const DriverProgram::Parameters::Ptr& parameters
) noexcept {
	auto dMaterial = mMaterials->Get(material);

	dMaterial->mInstancing = parameters->mInstancing;
	dMaterial->mDiffuseMap = material->mDiffuseMap;
	dMaterial->mEnvMap = material->mEnvMap;
	dMaterial->mNormalMap = material->mNormalMap;
	dMaterial->mSpecularMap = material->mSpecularMap;
}

bool Renderer::MaterialNeedsLights(const Material::Ptr& material) noexcept {
	if (material->mIsMeshPhongMaterial) {
		return true;
	}

	return false;
}

void Renderer::MakeLightsNeedUpdate(UniformHandleMap& lightsUniformMap) noexcept {
	for (auto& iter : lightsUniformMap) {
		iter.second.mNeedsUpdate = true;
	}
}

void Renderer::Swap()noexcept
{
	mWindow->Swap();
}

void Renderer::SetSize(int width, int height) noexcept
{
	mWidth = width;
	mHeight = height;

	mViewport.z = width;
	mViewport.w = height;

	mState->Viewport(mViewport);

	if (mOnSizeCallback) {
		mOnSizeCallback(width, height);
	}
}

void Renderer::SetRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept
{
	mCurrentRenderTarget = renderTarget;

	if (renderTarget == nullptr) {
		mState->BindFrameBuffer(0);
		return;
	}

	auto dRenderTarget = mRenderTargets->get(renderTarget);

	if (!dRenderTarget->mFrameBuffer) {
		mTextures->SetupRenderTarget(renderTarget);
	}

	mState->BindFrameBuffer(dRenderTarget->mFrameBuffer);
}

RenderTarget::Ptr Renderer::GetRenderTarget() const noexcept
{
	return mCurrentRenderTarget;
}

void Renderer::SetClearColor(float r, float g, float b, float a) noexcept
{
	mState->SetClearColor(r, g, b, a);
}

glm::vec4 Renderer::GetClearColor() const noexcept
{
	return mState->getClearColor();
}

void Renderer::EnableShadow(bool enable) noexcept
{
	mShadowMap->mEnabled = enable;
}

void Renderer::Clear(bool color = true, bool depth = true, bool stencil = true) noexcept
{
	GLbitfield bits = 0;

	if (color) bits |= GL_COLOR_BUFFER_BIT;
	if (depth) bits |= GL_DEPTH_BUFFER_BIT;
	if (stencil) bits |= GL_STENCIL_BUFFER_BIT;

	glClear(bits);
}

void Renderer::SetFrameSizeCallBack(const OnSizeCallback& callback) noexcept {
	mOnSizeCallback = callback;
}

void Renderer::SetMouseMoveCallBack(const DriverWindow::MouseMoveCallback& callback) noexcept {
	mWindow->SetMouseMoveCallBack(callback);
}

void Renderer::SetMouseActionCallBack(const DriverWindow::MouseActionCallback& callback) noexcept {
	mWindow->SetMouseActionCallBack(callback);
			
}

void Renderer::SetKeyboardActionCallBack(const DriverWindow::KeyboardActionCallback& callback) noexcept {
	mWindow->SetKeyboardActionCallBack(callback);
}