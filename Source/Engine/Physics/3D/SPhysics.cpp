// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics.h"
#include <common/PxTolerancesScale.h>
#include <PxSceneDesc.h>
#include <PxFiltering.h>

#include "World.h"
#include "Core/MultiThreading.h"
#include "Tools/Profiler.h"

#include "PhysicsTypes.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"


SPhysics::SPhysics() : Super()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *Memory::GetPhysicsAllocator(), pxErrorCallback);
	if (!foundation)
	{
		Log::Error("PhysX error: PxCreateFoundation failed");
		return;
	}

	physx::PxTolerancesScale scale;
	world = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, {});
	if (!world)
	{
		Log::Error("PhysX error: PxCreatePhysics failed");
		return;
	}
}

void SPhysics::BeginPlay()
{
	Super::BeginPlay();

	CreateScene();

	physicsWorld = ECS()->FindSingleton<CPhysicsWorld>();
}

void SPhysics::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	Step(deltaTime);
}

void SPhysics::EndPlay()
{
	scene->release();
	scene = nullptr;

	Super::EndPlay();
}

void SPhysics::BeforeDestroy()
{
	world->release();
	world = nullptr;

	foundation->release();
	foundation = nullptr;

	Super::BeforeDestroy();
}

void SPhysics::CreateScene()
{
	physx::PxSceneDesc sceneDesc(world->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	if (!sceneDesc.cpuDispatcher)
	{
		// Number of desired threads
		cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		if (!cpuDispatcher)
		{
			Log::Error("PhysX error: PxDefaultCpuDispatcherCreate failed!");
		}
		sceneDesc.cpuDispatcher = cpuDispatcher;
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	//sceneDesc.frictionType = PxFrictionType::eTWO_DIRECTIONAL;
	//sceneDesc.frictionType = PxFrictionType::eONE_DIRECTIONAL;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_AVERAGE_POINT;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
	//sceneDesc.flags |= PxSceneFlag::eADAPTIVE_FORCE;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;

	//sceneDesc.flags |= PxSceneFlag::eDISABLE_CONTACT_CACHE;
	//sceneDesc.broadPhaseType =  PxBroadPhaseType::eGPU;
	//sceneDesc.broadPhaseType = PxBroadPhaseType::eSAP;
	sceneDesc.gpuMaxNumPartitions = 8;


	//sceneDesc.solverType = PxSolverType::eTGS;

#ifdef USE_MBP
	sceneDesc.broadPhaseType = PxBroadPhaseType::eMBP;
#endif


	scene = world->createScene(sceneDesc);
	if (!scene)
	{
		Log::Error("PhysX error: CreateScene failed!");
	}
}

void SPhysics::Step(float deltaTime)
{
	ScopedGameZone("Step");
}
