// © 2019 Miguel Fernández Arce - All rights reserved

#include "Texture.h"
#include <SDL_surface.h>
#include <SDL_image.h>

#include "Core/Engine.h"
#include "Core/Assets/AssetPtr.h"
#include "Core/Files/FileSystem.h"
#include "Rendering/Commands/MaterialCommands.h"


bool Material::PostLoad()
{
	data = {};

	Path rawTexturePath = FileSystem::FindRawFile(FileSystem::FromString(GetInfo().GetSPath()));
	if(!rawTexturePath.empty())
	{
		String code;
		if (!FileSystem::LoadStringFile(rawTexturePath, code))
		{
			// Shader file not found or loaded
			return false;
		}

		if (!data.Parse(code))
		{
			// Invalid code
			return false;
		}

		LoadTextures();

		QueueRenderCommand<LoadMaterialCommand>(GetInfo(), GetData());

		return Super::PostLoad();
	}
	return false;
}

void Material::OnUnload()
{
	QueueRenderCommand<FreeMaterialCommand>(GetInfo());
}

void Material::LoadTextures()
{
	baseColor.Load();
	normal.Load();
	rmao.Load();

	data.textureParams.Reserve(3);
	if (baseColor.IsValid())
	{
		data.textureParams.Add({ TX("base_color"), baseColor.GetInfo() });
	}
	if (normal.IsValid())
	{
		data.textureParams.Add({ TX("normal"), normal.GetInfo() });
	}
	if (rmao.IsValid())
	{
		data.textureParams.Add({ TX("rmao"), rmao.GetInfo() });
	}
}
