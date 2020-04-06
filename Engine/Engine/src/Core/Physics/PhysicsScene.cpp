#include <iostream>
#include "PhysicsScene.h"

#include "PhysicsSceneSimulationEventCallback.h"
#include "PxPhysicsAPI.h"
#include "PxMaterial.h"
#include "PxDefaultSimulationFilterShader.h"
#include "PxRigidDynamic.h"
#include "Collider.h"
#include "StaticMesh.h"


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
		PhysicsSceneSimulationEventCallback* eventCallBack = new PhysicsSceneSimulationEventCallback();
		sceneDesc.cudaContextManager = PxCreateCudaContextManager(*m_pxFoundation, cudaContextManagerDesc);
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

	void HitResult::initHitResult(physx::PxRaycastHit raycastHit)
	{
		Core::Datastructure::IPhysics* physicsMesh{ static_cast<Core::Datastructure::IPhysics*>(raycastHit.actor->userData) };
		physicsMeshHit = physicsMesh;
		objectHit = physicsMesh->GetParent();

		physx::PxVec3 posHit{ raycastHit.position };
		hitPoint = { posHit.x, posHit.y, posHit.z };
		distance = raycastHit.distance;
	}

	bool PhysicsScene::Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, HitResult& result, const float Distance)
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

	bool PhysicsScene::Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, std::vector<HitResult>& results, const float Distance)
	{
		physx::PxVec3 origin{ OriginPos.x, OriginPos.y, OriginPos.z };
		physx::PxVec3 dir{ Direction.x, Direction.y, Direction.z };

		physx::PxRaycastBuffer hit;
		bool status = m_pxScene->raycast(origin, dir, static_cast<physx::PxReal>(Distance), hit);
		if (status)
		{
			for (physx::PxU32 i{ 0 }; i < hit.nbTouches; i++)
			{
				HitResult result;
				result.initHitResult(hit.touches[i]);
				results.push_back(result);
			}
		}
		return status;
	}

	void PhysicsScene::AttachActor(Core::Datastructure::IPhysics* physics)
	{
		physics->CreateActor(m_pxPhysics, m_pxScene);
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