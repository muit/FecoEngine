// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "TextureData.h"
#include "SDL_surface.h"


void TextureData::Fill(Color color)
{
	const u32 pixelCount = size.x() * size.y();
	for (u32 i = 0; i < pixelCount; ++i)
	{
		buffer[i] = color;
	}
}

void TextureData::FromSurface(SDL_Surface* surface)
{
	if (surface)
	{
		bool reformated = false;
		if (surface->format->format != SDL_PIXELFORMAT_RGBA32)
		{
			surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
			reformated = true;
		}

		size = { surface->w, surface->h };

		const u32 pixelCount = size.x() * size.y();
		u32* const pixels = (u32*)surface->pixels;

		// Copy pixels
		buffer.Reserve(pixelCount);
		for (u32 i = 0; i < pixelCount; ++i)
		{
			const u32 pixel = pixels[i];
			buffer.Add(Color{ pixel });
		}

		if (reformated) {
			SDL_FreeSurface(surface);
		}
	}
}

void TextureData::ToSurface(SDL_Surface* surface)
{

}