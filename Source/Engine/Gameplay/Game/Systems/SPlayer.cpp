// Copyright 2015-2019 Piperift - All rights reserved

#include "SPlayer.h"
#include "Core/Log.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CCamera.h"

#include "Core/Engine.h"
#include "../Components/CPlayer.h"
#include "Physics/2D/Components/CBody2D.h"
#include "Physics/3D/Components/CBody.h"
#include "Tools/Profiler.h"


void SPlayer::BeginPlay()
{
	Super::BeginPlay();

	input = GEngine->GetInput();

	input->CreateAxisAction({ "MoveRight" }, {
		{ EKey::A, EKeyModifier::None, -1.f },
		{ EKey::D, EKeyModifier::None,  1.f }
	}, {})
	.Bind(this, &SPlayer::MoveRight);

	input->CreateAxisAction({ "MoveForward" }, {
		{ EKey::W, EKeyModifier::None,  1.f },
		{ EKey::S, EKeyModifier::None, -1.f }
		}, {})
	.Bind(this, &SPlayer::MoveForward);


	// Store home locations
	auto view = ECS()->View<CPlayer, CTransform>();
	for (EntityId entity : view)
	{
		CPlayer&    player    = view.get<CPlayer>(entity);
		const CTransform& transform = view.get<CTransform>(entity);

		player.homeLocation = transform.GetWLocation();
	}
}

void SPlayer::Tick(float deltaTime)
{
	ScopedGameZone("Player");

	Movement2D();
	Movement3D();
	movementDelta = v2::Zero();

	// Player Dead
	auto playerDeadView = ECS()->View<CPlayer, CTransform>();
	for (EntityId entity : playerDeadView)
	{
		CPlayer&    player    = playerDeadView.get<CPlayer>(entity);
		CTransform& transform = playerDeadView.get<CTransform>(entity);

		if (transform.GetWLocation().z <= player.dieHeight)
		{
			Log::Message("Player died");
			transform.SetWLocation(player.homeLocation);
		}
	}

	MoveCameras(deltaTime);
}

void SPlayer::MoveForward(float delta)
{
	movementDelta.y += delta;
}

void SPlayer::MoveRight(float delta)
{
	movementDelta.x += delta;
}

void SPlayer::Movement2D()
{
	auto view = ECS()->View<CPlayer, CBody2D>();
	for (EntityId entity : view)
	{
		CPlayer& player = view.get<CPlayer>(entity);
		CBody2D& body   = view.get<CBody2D>(entity);
		body.body.ApplyLinearImpulse({ movementDelta * player.impulse });
		body.body.ApplyTorque(movementDelta.x * player.impulse);
	}
}

void SPlayer::Movement3D()
{
	auto view = ECS()->View<CPlayer, CBody>();
	for (EntityId entity : view)
	{
		CPlayer& player = view.get<CPlayer>(entity);
		CBody&   body   = view.get<CBody>(entity);

		body.AddAcceleration(movementDelta.xy() * player.impulse);
	}
}

void SPlayer::MoveCameras(float deltaTime)
{
	auto ecs = ECS();

	// Find camera target
	EntityId cameraTarget = NoEntity;
	CPlayer* playerPtr = nullptr;
	auto camTargetView = ecs->View<CPlayer>();
	for (EntityId entity : camTargetView)
	{
		if (cameraTarget == NoEntity)
		{
			cameraTarget = entity;
			playerPtr = &camTargetView.get(entity);
		}
	}

	// Smooth camera follow
	if (cameraTarget != NoEntity && ecs->Has<CTransform>(cameraTarget))
	{
		auto cameraView = ECS()->View<CTransform, CCamera>();
		for (EntityId entity : cameraView)
		{
			CTransform& targetTransform = ecs->Get<CTransform>(cameraTarget);
			CTransform& transform = cameraView.get<CTransform>(entity);

			const v3 targetLocation = targetTransform.GetWLocation() + v3{ 0.f, -playerPtr->cameraDistance, 1.f };
			transform.SetWLocation(
				Math::Lerp(transform.GetWLocation(), targetLocation, deltaTime * playerPtr->cameraSpeed)
			);

			// Look at player
			//transform.SetWRotation(Quat::LookAt(transform.GetWLocation(), targetTransform.GetWLocation()));
		}
	}
}
