#pragma once

#include "ComponentBase.h"
#include "ICamera.h"
#include "IUpdatable.hpp"
#include "CoreMinimal.h"


BAKERS_API_CLASS Camera : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::ICamera, public virtual Core::Datastructure::IUpdatable
{
private:
	float m_pRatio;
	float m_pFov;
	float m_pNear;
	float m_pFar;
protected:
	float m_ratio;
	float m_fov;
	float m_near;
	float m_far;

	Core::Maths::Mat4	OnGenerateCamera() override;
	Core::Maths::Mat4	OnGeneratePerspective() override;

	virtual void OnCopy(void* toCopy) const override;
	virtual void StartCopy(void*& copyTo) const override;
public:
	/**
	 * Default Constructor
	 */
	Camera();

	/**
	 * Constructor initializing camera variables
	 */
	Camera(const float ratio, const float fov, const float near, const float far);

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnStart() override;

	void SetRatio(const float newRatio) override;
	
	/**
	 * Create Perspective Matrix
	 * @param ratio: Ratio (width / height) of the window
	 * @param near: Distance where objects start to be rendered
	 * @param far: Distance where objects are no longer rendered
	 * @param fov: Angle of the field of view of the camera
	 * @return Perspective matrix
	 */
	static Core::Maths::Mat4 CreatePerspectiveMatrix(const float ratio, const float near, const float far, const float fov);
	
	REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::ICamera, Core::Datastructure::IUpdatable)
};
