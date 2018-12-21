// Copyright 2015-2019 Piperift - All rights reserved

#include "Details.h"
#include "Core/Reflection/Runtime/HandleHelper.h"
#include "Core/Reflection/ReflectionTags.h"


#if WITH_EDITOR

void Details::Build()
{
	Super::Build();
	windowName = "Details";

	if (object && object->GetClass())
	{
		windowName = String("Details: ").append(object->GetName().ToString());

		Class* objectClass = object->GetClass();
		for (const auto& property : objectClass->GetAllProperties())
		{
			if (property.second->HasTag(DetailsEdit) || property.second->HasTag(DetailsView))
			{
				auto handle = HandleHelper::CreatePropertyHandle(object, property.second.get());
				Add(PropertyWidget::NewPropertyWidget(handle));
			}
		}
	}

	eastl::vector<Class*> children{};
	Object::StaticClass()->GetAllChildClasses(children);

	/*for (auto* child : children) {
		W(PropertyWidget, child);
	}*/
}

void Details::Tick()
{
	if (bOpen)
	{
		BeginWindow();
		if (object.IsValid())
		{
			TickChilds();
		}
		EndWindow();
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