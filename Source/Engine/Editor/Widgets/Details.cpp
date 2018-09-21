// Copyright 2015-2019 Piperift - All rights reserved

#include "Details.h"

#if WITH_EDITOR

void Details::Build()
{
	std::vector<Class*> children{};
	Object::StaticClass()->GetAllChildClasses(children);

	for (auto* child : children) {
		W(PropertyWidget, child);
	}
}

void Details::Tick()
{
	//if (object.IsValid())
	{
		ImGui::Begin("Details");
		{
			ImGui::Text("Name  ");
			ImGui::SameLine();
			static char str0[128] = "Object!";
			static int i0 = 123;
			ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
			ImGui::Separator();
			TickChilds();
		}
		ImGui::End();
	}
}

void Details::SetObject(Ptr<Object> inObject)
{
	if (inObject != object)
	{
		object = inObject;
		ForceRebuild();
	}
}

#endif