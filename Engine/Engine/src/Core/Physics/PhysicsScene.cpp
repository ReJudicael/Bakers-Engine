#include <iostream>
#include "PhysicsScene.h"

#include "PhysicsSceneSimulationEventCallback.h"
#include "PxPhysicsAPI.h"
#include "PxMaterial.h"
#include "PxDefaultSimulationFilterShader.h"
#include "Collider.h"
#include "StaticMesh.h"
#include "Transform.hpp"
#include "Model.h"
#include "Vec3.hpp"
#include "PxRigidActor.h"
#include "Object.hpp"


#define PVD_HOST "127.0.0.1"

namespace Core::Physics
{

	bool PhysicsScene::InitPhysX()
	{
		m_pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_pxDefaultAllocatorCallback,
			m_pxDefaultErrorCallback);

		if (!m_pxFoundation)
			std::cout << "PxCreateFoundation failed!" << std::endl;

		m_pxTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);

#ifdef NEED_PVD
		m_pxPvd = physx::PxCreatePvd(*m_pxFoundation);
		m_pxPvd->connect(*m_pxTransport, physx::PxPvdInstrumentationFlag::eALL);
#endif 
		m_pxPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *m_pxFoundation,
			physx::PxTolerancesScale(), true, m_pxPvd);
		if (!m_pxPhysics)
		{
			std::cout << "PxCreatePhysics failed!" << std::endl;
			return false;
		}


		physx::PxCookingParams params(m_pxPhysics->getTolerancesScale());
		params.buildGPUData = true;
		m_pxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pxFoundation, params);

		if (!m_pxCooking)
		{
			std::cout << "Cooking failed!" << std::endl;
			return false;
		}

		if (!PxInitExtensions(*m_pxPhysics, m_pxPvd))
		{
			std::cout << "extention failed!" << std::endl;
			return false;
		}

		CreateScene();
		return true;
	}

	void PhysicsScene::CreateScene()
	{
		physx::PxSceneDesc sceneDesc(m_pxPhysics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
		physx::PxCudaContextManagerDesc cudaContextManagerDesc;
		sceneDesc.cudaContextManager = PxCreateCudaContextManager(*m_pxFoundation, cudaContextManagerDesc);
		PhysicsSceneSimulationEventCallback* eventCallBack = new PhysicsSceneSimulationEventCallback();
		sceneDesc.filterShader = &Core::Physics::PhysicsSceneSimulationEventCallback::filterShader;
		//sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;

		sceneDesc.simulationEventCallback = eventCallBack;
		//sceneDesc.filterCallback = filterCallback;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
		//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
		sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;

		m_pxScene = m_pxPhysics->createScene(sceneDesc);

		if (!m_pxScene)
		{
			std::cout << "Scene failed!" << std::endl;
		}
	}

	void PhysicsScene::CreatePhysicsShape(Collider& collider)
	{
		collider.CreateShape(m_pxPhysics);
	}

	void HitResultQuery::initHitResult(const physx::PxRaycastHit raycastHit)
	{
		Core::Datastructure::IComponent* physicsMesh{ static_cast<Core::Datastructure::IComponent*>(raycastHit.actor->userData) };
		objectHit = physicsMesh->GetParent();
		physicsMeshHit = dynamic_cast<Core::Datastructure::IPhysics*>(physicsMesh);

		physx::PxVec3 posHit{ raycastHit.position };
		hitPoint = { posHit.x, posHit.y, posHit.z };
		distance = raycastHit.distance;
	}

	void HitResultQuery::initHitResult(const physx::PxOverlapHit overlapHit)
	{
		Core::Datastructure::IComponent* physicsMesh{ static_cast<Core::Datastructure::IComponent*>(overlapHit.actor->userData) };
		objectHit = physicsMesh->GetParent();
		physicsMeshHit = dynamic_cast<Core::Datastructure::IPhysics*>(physicsMesh);

		//physx::PxU32 face{ overlapHit.faceIndex };
	}

	bool PhysicsScene::Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, HitResultQuery& result, const float Distance)
	{
		physx::PxVec3 origin{ OriginPos.x, OriginPos.y, OriginPos.z };
		physx::PxVec3 dir{ Direction.x, Direction.y, Direction.z };

		physx::PxRaycastBuffer hit;
		bool status = m_pxScene->raycast(origin, dir, static_cast<physx::PxReal>(Distance), hit);
		if (status)
		{
			result.initHitResult(hit.block);
		}
		return status;
	}

	bool PhysicsScene::Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, std::vector<HitResultQuery>& results, const float Distance)
	{
		physx::PxVec3 origin{ OriginPos.x, OriginPos.y, OriginPos.z };
		physx::PxVec3 dir{ Direction.x, Direction.y, Direction.z };

		physx::PxRaycastBuffer hit;
		bool status = m_pxScene->raycast(origin, dir, static_cast<physx::PxReal>(Distance), hit);
		if (status)
		{
			for (physx::PxU32 i{ 0 }; i < hit.nbTouches; i++)
			{
				HitResultQuery result;
				result.initHitResult(hit.touches[i]);
				results.push_back(result);
			}
		}
		return status;
	}

	bool PhysicsScene::Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, HitResultQuery& result, physx::PxU32 filterRaycast, const float Distance)
	{
		physx::PxVec3 origin{ OriginPos.x, OriginPos.y, OriginPos.z };
		physx::PxVec3 dir{ Direction.x, Direction.y, Direction.z };

		physx::PxQueryFilterData filterData{};
		for (int i = 0; i < 4; i++)
			filterData.data.word0 |= 1 << i;
		filterData.data.word0 &= ~filterRaycast;

		physx::PxRaycastBuffer hit;
		bool status = m_pxScene->raycast(origin, dir, static_cast<physx::PxReal>(Distance), hit, physx::PxHitFlag::eDEFAULT, filterData);
		if (status)
		{
			result.initHitResult(hit.block);
		}
		return status;
	}

	bool PhysicsScene::Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, std::vector<HitResultQuery>& results, physx::PxU32 filterRaycast, const float Distance)
	{
		physx::PxVec3 origin{ OriginPos.x, OriginPos.y, OriginPos.z };
		physx::PxVec3 dir{ Direction.x, Direction.y, Direction.z };

		physx::PxRaycastBuffer hit;
		physx::PxQueryFilterData filterData{};
		for (int i = 0; i < 4; i++)
			filterData.data.word0 |= 1 << i;
		filterData.data.word0 &= ~filterRaycast;

		bool status = m_pxScene->raycast(origin, dir, static_cast<physx::PxReal>(Distance), hit, physx::PxHitFlag::eDEFAULT, filterData);
		if (status)
		{
			for (physx::PxU32 i{ 0 }; i < hit.nbTouches; i++)
			{
				HitResultQuery result;
				result.initHitResult(hit.touches[i]);
				results.push_back(result);
			}
		}
		return status;
	}

	bool PhysicsScene::CheckOverlap(const physx::PxGeometry& overlapGeometry, const Core::Datastructure::Transform& transform, HitResultQuery& overlapResult)
	{

		physx::PxTransform pose;
		Core::Maths::Vec3 globalPos{ transform.GetGlobalPos() };
		pose.p = { globalPos.x, globalPos.y, globalPos.z };
		Core::Maths::Quat globalRot{ transform.GetGlobalRot() };
		pose.q = { globalRot.x, globalRot.y, globalRot.z, globalRot.w };

		physx::PxOverlapCallback* result{};
		bool status = m_pxScene->overlap(overlapGeometry, pose, *result);
		if (status)
			overlapResult.initHitResult(result->block);

		return status;
	}
	
	bool PhysicsScene::CheckOverlap(const physx::PxGeometry& overlapGeometry, const Core::Datastructure::Transform& transform, std::vector<HitResultQuery>& overlapResults)
	{
		physx::PxTransform pose;
		Core::Maths::Vec3 globalPos{ transform.GetGlobalPos() };
		pose.p = { globalPos.x, globalPos.y, globalPos.z };
		Core::Maths::Quat globalRot{ transform.GetGlobalRot() };
		pose.q = { globalRot.x, globalRot.y, globalRot.z, globalRot.w };

		physx::PxOverlapCallback* overlapCallback{};
		bool status = m_pxScene->overlap(overlapGeometry, pose, *overlapCallback);
		if (status)
		{
			for (physx::PxU32 i{ 0 }; i < overlapCallback->nbTouches; i++)
			{
				HitResultQuery result;
				result.initHitResult(overlapCallback->touches[i]);
				overlapResults.push_back(result);
			}
		}
		return status;
	}
	
	void PhysicsScene::UpdatePoseOfActor(physx::PxRigidActor* actor)
	{
		Core::Datastructure::IComponent* comp = static_cast<Core::Datastructure::IComponent*>(actor->userData);
		Maths::Vec3 vec = comp->GetParent()->GetGlobalPos();
		physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
		Maths::Quat quat = comp->GetParent()->GetGlobalRot();
		physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };

		actor->setGlobalPose(physx::PxTransform(position, rotation));
	}

	void PhysicsScene::AttachActor(Core::Datastructure::IPhysics* physics)
	{
		physics->CreateActor(m_pxPhysics, m_pxScene);
	}

	physx::PxRigidActor* PhysicsScene::CreateEditorPhysicsActor(void* useDataPtr,
																const Core::Datastructure::Transform& transform,
																std::shared_ptr<Resources::Model> model)
	{
		physx::PxShape* shape{ CreateEditorBoxShape(transform, model) };

		if (shape == nullptr)
			return nullptr;

		physx::PxVec3 globalPos { transform.GetGlobalPos().x,transform.GetGlobalPos().y, transform.GetGlobalPos().z};
		physx::PxQuat globalRot { transform.GetGlobalRot().x,transform.GetGlobalRot().y, transform.GetGlobalRot().z, transform.GetGlobalRot().w };
		physx::PxTransform pxTransform{ globalPos, globalRot};

		physx::PxRigidStatic* rigidStatic =  m_pxPhysics->createRigidStatic(pxTransform);
		rigidStatic->attachShape(*shape);
		shape->release();
		rigidStatic->userData = useDataPtr;
		rigidStatic->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
		m_pxScene->addActor(*rigidStatic);

		return rigidStatic;
	}

	physx::PxShape* PhysicsScene::CreateEditorBoxShape(const Core::Datastructure::Transform& transform, std::shared_ptr<Resources::Model> model)
	{
		physx::PxMaterial* material = m_pxPhysics->createMaterial(0.f, 0.f, 0.f);

		Core::Maths::Vec3 minG{ model->min * transform.GetGlobalScale() };
		Core::Maths::Vec3 maxG{ model->max * transform.GetGlobalScale() };

		Core::Maths::Vec3 pos{ (minG - maxG) };
		pos = maxG + pos / 2;

		Core::Maths::Vec3 extent{ abs(minG.x) + abs(maxG.x), abs(minG.y) + abs(maxG.y), abs(minG.z) + abs(maxG.z) };
		extent /= 2;

		if (extent.x == 0 || extent.y == 0 || extent.x == 0)
			return nullptr;

		physx::PxShape* shape = m_pxPhysics->createShape(physx::PxBoxGeometry(physx::PxVec3{ extent.x, extent.y, extent.z }), *material, true);
		physx::PxTransform shapeTransform{ shape->getLocalPose() };
		shapeTransform.p = { pos.x, pos.y, pos.z };
		shape->setLocalPose(shapeTransform);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);

		return shape;
	}

	void PhysicsScene::BeginSimulate(const float deltaTime)
	{
		m_accumulator += deltaTime;
		if (m_accumulator < m_stepSimulation)
			m_IsSimulating = false;

		m_accumulator -= m_stepSimulation;

		m_pxScene->simulate(m_stepSimulation);
		m_IsSimulating = true;
	}

	void PhysicsScene::EndSimulate()
	{
		m_pxScene->fetchResults(m_IsSimulating);
	}

	void PhysicsScene::ReleasePhysXSDK()
	{
		m_IsSimulating = true;

		EndSimulate();

		if (m_pxPhysics != nullptr)
		{
			PxCloseExtensions();

			m_pxScene->release();
			//m_pxDispatcher->release();
			m_pxPhysics->release();
			m_pxCooking->release();
			if (m_pxPvd)
			{
				m_pxPvd->release();
				m_pxPvd = nullptr;
			}

			//m_pxCudaContextManager->release();

			m_pxFoundation->release();
			m_pxScene = nullptr;
			m_pxPhysics = nullptr;
			m_pxCooking = nullptr;
			m_pxFoundation = nullptr;
			//m_pxDispatcher = nullptr;
			//m_pxCudaContextManager = nullptr;
		}
	}

	void PhysicsScene::RemoveActorFromPhysicsScene(physx::PxRigidActor* actor)
	{
		m_pxScene->removeActor(*actor);
	}

	PhysicsScene::~PhysicsScene()
	{
		ReleasePhysXSDK();
	}
}