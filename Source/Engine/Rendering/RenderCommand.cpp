// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "RenderCommand.h"
#include "Frame.h"


void CameraCommand::Execute(FrameRender& render, Frame& frame)
{
	// Store camera transform matrix
	Eigen::Transform<float, 3, Eigen::Affine> a{};
	a.pretranslate(cameraTransform.location)
		.prerotate(cameraTransform.rotation)
		.scale(cameraTransform.scale);
}
