// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "Object.h"


class System : public Object {
	CLASS(System, Object)

    virtual void Tick(float /*deltaTime*/) override {}
}