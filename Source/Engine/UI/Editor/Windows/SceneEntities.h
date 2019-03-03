// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Editor/EditorWindow.h"
#include "UI/Editor/Widgets/PropertyWidget.h"

#include "ECS/ECSManager.h"
#include "Details.h"


class SceneEntities : public EditorWindow {
	CLASS(SceneEntities, EditorWindow)

	ImGuiTextFilter filter;

	TArray<EntityId> selectedEntities;

public:

	Ptr<Details> sceneDetailsWindow;


	SceneEntities() : Super(), filter{} {}

protected:

	virtual void Build() override;

	virtual void Tick() override;

	void OnEntityClicked(EntityId entity);
};

#endif
