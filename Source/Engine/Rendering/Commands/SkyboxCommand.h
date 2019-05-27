// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Tools/Profiler.h"

#include "Core/MultiThreading.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Mesh.h"
#include "../Renderer.h"
#include "../Frame.h"

#include "RenderCommand.h"


class DrawSkyboxCommand : public RenderCommand {

	AssetInfo material;
	AssetInfo texture;

public:

	DrawSkyboxCommand(AssetInfo material, AssetInfo texture)
		: material{material}, texture { texture }
	{}

	virtual void Execute(FrameRender& render, Frame& frame) override;
};
