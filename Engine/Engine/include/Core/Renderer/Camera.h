#pragma once

#include "ComponentBase.h"
#include "ICamera.h"
#include "IUpdatable.hpp"
#include "CoreMinimal.h"

struct CameraPerspective
{
	float ratio = 1.714f;
	float fov = 60.0f;
	float near = 0.1f;
	float far = 100.f;

	REGISTER_CLASS()
};

BAKERS_API_CLASS Camera : public virtual Core::Datastructure::ComponentBase, public virtual Core::Datastructure::ICamera, public virtual Core::Datastructure::IUpdatable
{
private:
	CameraPerspective m_prevPersp;
protected:
	CameraPerspective m_persp;
	

	Core::Maths::Mat4	OnGenerateCamera() override;
	Core::Maths::Mat4	OnGeneratePerspective() override;

	virtual void OnCopy(IComponent * copyTo) const override;
	virtual void StartCopy(IComponent*& copyTo) const override;
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
	
	bool PerspectiveNeedUpdate() const noexcept;

	REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::ICamera, Core::Datastructure::IUpdatable)
};
