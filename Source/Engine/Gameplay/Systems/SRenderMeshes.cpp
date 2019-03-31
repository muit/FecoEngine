// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMeshes.h"
#include "Rendering/Commands/ResourceCommands.h"
#include "Core/World.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"


void SRenderMeshes::BeginPlay()
{
	Super::BeginPlay();
	texture.Load();
}

void SRenderMeshes::Tick(float /*deltaTime*/)
{
	Ptr<World> world{ GetWorld() };

	ECS()->View<CTransform, CMesh>().each(
	[world](const EntityId e, CTransform& t, CMesh& c)
	{
		if (c.model.IsValid())
		{
			world->QueueRender<DrawMeshCommand>(c.model->GetId(), t.transform, c.color);
		}
	});

	if (texture)
	{
		world->QueueRender<LoadTextureCommand>(0, texture->GetTextureData());
		world->QueueRender<Draw2DTextureCommand>(0, v2_u32{ 10, 20 });
	}
}

void SRenderMeshes::BeforeDestroy()
{
	Super::BeforeDestroy();
}
