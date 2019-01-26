// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "RenderCommand.h"
#include "Frame.h"


void CameraCommand::Execute(FrameRender& render, Frame& frame)
{
	// Store camera transform matrix
	render.camera = eastl::move(camera);
}
