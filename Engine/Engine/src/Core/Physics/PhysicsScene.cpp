#include <iostream>
#include "PhysicsScene.h"

#include "PxPhysicsAPI.h"
#include "PxDefaultAllocator.h"
#include "PxMaterial.h"
#include "PxDefaultSimulationFilterShader.h"
#include "PxRigidDynamic.h"
// physX test
static physx::PxDefaultErrorCallback	m_pxDefaultErrorCallback;
static physx::PxDefaultAllocator		m_pxDefaultAllocatorCallback;
static const physx::PxVec3 convexVerts[] = { physx::PxVec3(0,1,0),physx::PxVec3(1,0,0),physx::PxVec3(-1,0,0),physx::PxVec3(0,0,1),
physx::PxVec3(0,0,-1) };
#define PVD_HOST "127.0.0.1"

// test physx
/*{
	physx::PxFoundation* gFoundation;

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	if (!gFoundation)
		std::cout << "PxCreateFoundation failed!" << std::endl;

	bool recordMemoryAllocations = true;

	physx::PxPvd* mPvd = physx::PxCreatePvd(*gFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	physx::PxPhysics* mPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *gFoundation,
		physx::PxTolerancesScale(), recordMemoryAllocations, mPvd);
	if (!mPhysics)
		std::cout << "PxCreatePhysics failed!" << std::endl;

	if (!PxInitExtensions(*mPhysics, mPvd))
		std::cout << "extention failed!" << std::endl;

	physx::PxCookingParams params(mPhysics->getTolerancesScale());
	params.buildGPUData = true;
	physx::PxCooking* gPxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, params);

	if (!gPxCooking)
	{
		std::cout << "Cooking failed!" << std::endl;
	}

	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	physx::PxCudaContextManagerDesc cudaContextManagerDesc;
	sceneDesc.cudaContextManager = PxCreateCudaContextManager(*gFoundation, cudaContextManagerDesc);
	//sceneDesc.filterShader = &filterShader;
	//sceneDesc.flags |= physx::PxSceneFlag::eMUTABLE_FLAGS;
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	//sceneDesc.simulationEventCallback = eventCallBack;
	//sceneDesc.filterCallback = filterCallback;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
	//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
	sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;

	physx::PxScene* mScene = mPhysics->createScene(sceneDesc);

	if (!mScene)
	{
		std::cout << "Scene failed!" << std::endl;
	}

	physx::PxMaterial* myMaterial;
	myMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.1f); // static friction, dynamic friction, restitution


	physx::PxRigidDynamic* myActor2 = mPhysics->createRigidDynamic(physx::PxTransform(0.f, 10.f, 0.f));

	physx::PxShape* shape = mPhysics->createShape(physx::PxSphereGeometry(1.0f), *myMaterial, true);
	physx::PxShape* shape2 = mPhysics->createShape(physx::PxSphereGeometry(1.0f), *myMaterial, true);
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	shape2->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	physx::PxRigidStatic* myActor = mPhysics->createRigidStatic(physx::PxTransform(0.f, 0.f, 0.f));
	//shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
	myActor->attachShape(*shape2);
	myActor2->attachShape(*shape);

	//setupFiltering(myActor, EFilterCollision::LAVIE, EFilterCollision::NATHAN, EFilterCollision::JUDICAEL | EFilterCollision::VALENTIN);
	//mScene->addActor(*myActor);
	//setupFiltering(myActor2, EFilterCollision::VALENTIN, EFilterCollision::NATHAN, EFilterCollision::LAVIE | EFilterCollision::JUDICAEL);
	//setupFiltering(myActor2, EFilterCollision::VALENTIN, EFilterCollision::JUDICAEL, EFilterCollision::LAVIE | EFilterCollision::NATHAN);
	mScene->addActor(*myActor2);
	shape->release();


	physx::PxConvexMeshDesc convexDesc;
	convexDesc.points.count = 5;
	convexDesc.points.stride = sizeof(physx::PxVec3);
	convexDesc.points.data = convexVerts;
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream buf;
	physx::PxConvexMeshCookingResult::Enum result;

	if (!gPxCooking->cookConvexMesh(convexDesc, buf, &result))
		return;
	physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	physx::PxConvexMesh* convexMesh = mPhysics->createConvexMesh(input);

	physx::PxShape* shape3 = mPhysics->createShape(physx::PxConvexMeshGeometry(convexMesh), *myMaterial, true);
	physx::PxRigidStatic* myActor3 = mPhysics->createRigidStatic(physx::PxTransform(0.f, 0.f, 0.f));
	//shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	myActor3->attachShape(*shape3);
	//setupFiltering(myActor3, EFilterCollision::JUDICAEL, EFilterCollision::VALENTIN, EFilterCollision::LAVIE | EFilterCollision::NATHAN);
	//setupFiltering(myActor, EFilterCollision::JUDICAEL, EFilterCollision::NATHAN, EFilterCollision::LAVIE | EFilterCollision::VALENTIN);

	mScene->addActor(*myActor3);
	shape3->release();

	physx::PxShape** shapeBuff;
	float jij = 0;
}*/

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
#endif // NEED_PVD
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
		return true;
	}

	void PhysicsScene::Simulate(const float deltaTime)
	{
		m_accumulator += deltaTime;
		if (m_accumulator < m_stepSimulation)
			m_IsSimulating = false;

		m_accumulator -= m_stepSimulation;

		m_pxScene->simulate(m_stepSimulation);
		m_IsSimulating = true;
	}
}