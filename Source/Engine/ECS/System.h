// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"

class ECSManager;


class System : public Object {
	CLASS(System, Object)

public:

	PROP(bool, bTickOnEditor)
	bool bTickOnEditor = false;


	virtual void BeginPlay() {}
	virtual void Tick(float /*deltaTime*/) {}
	virtual void EndPlay() {}


protected:

	Ptr<ECSManager> ECS() const;
};
