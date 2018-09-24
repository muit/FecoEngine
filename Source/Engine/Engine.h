// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Renderer.h"
#include <SDL.h>

#include "Object.h"
#include "Util/Time.h"
#include "World/World.h"
#include "UI/UIManager.h"


class Engine : public Object {
	CLASS(Engine, Object)

	FrameTime frameTime;
	GlobalPtr<World> world;
	GlobalPtr<UIManager> ui;
	GlobalPtr<Renderer> renderer;


public:

	Engine() {}


	/** Begin Object interface */
protected:
	virtual void BeforeDestroy() override { Shutdown(); }
public:
	virtual Ptr<World> GetWorld() const override { return world; }
	/** End Object interface */


	bool Start() {
		// Setup SDL
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
			return false;

		renderer = Create<Renderer>();
		if(renderer->GetState() == ERendererState::Failed)
			return false;

		world = Create<World>(ThisPtr());

		ui = Create<UIManager>(ThisPtr());
		ui->Prepare();

		frameTime = {};
		bool bFinish = false;
		while (!bFinish)
		{
			frameTime.Tick();

			// Process window and input events
			SDL_PumpEvents();
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					bFinish = true;
				else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == renderer->GetWindowId())
					bFinish = true;
			}

			renderer->PreTick();

			world->Tick(frameTime.deltaTime);
			ui->Tick(frameTime.deltaTime);

			// Rendering
			renderer->Render();
		}

		return true;
	}

	void Shutdown() {
		SDL_Quit();
	}


	static void StartEngine()
	{
		GEngine->Start();
	}

	static const GlobalPtr<Engine> GEngine;
};

#define GEngine Engine::GEngine
