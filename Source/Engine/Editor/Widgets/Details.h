// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Widget.h"
#include "PropertyWidget.h"
#include "Reflection/Class.h"


class Details : public Widget {

	Ptr<Object> object;


protected:

	virtual void Build() override;

	virtual void Tick() override;

public:

	void SetObject(Ptr<Object> inObject);
};

#endif