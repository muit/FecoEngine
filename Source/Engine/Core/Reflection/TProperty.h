// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Property.h"
#include <EASTL/shared_ptr.h>

#include "Core/Strings/Name.h"
#include "Class.h"
#include "StructType.h"
#include "ReflectionTags.h"
#include "Runtime/TPropertyHandle.h"


/**
 * Static information about a property
 */
template <typename VarType>
class TProperty : public Property {
public:

	using Access = eastl::function<VarType*(BaseStruct*)>;

private:

	Access access;


public:

	TProperty(BaseType* typePtr, const Name& typeName, Name&& name, Access&& access, ReflectionTags tags)
		: Property(typePtr, typeName, eastl::move(name), tags), access(access)
	{}

	virtual eastl::shared_ptr<PropertyHandle> CreateHandle(const Ptr<BaseObject>& instance) const override {
		if (instance->GetClass() == GetType())
		{
			return eastl::shared_ptr<PropertyHandle>(new TPropertyHandle<VarType>(instance, this, access));
		}
		return {};
	}

	virtual eastl::shared_ptr<PropertyHandle> CreateHandle(BaseStruct* instance) const override {
		if (instance->GetStruct() == GetType())
		{
			return eastl::shared_ptr<PropertyHandle>(new TPropertyHandle<VarType>(instance, this, access));
		}
		return {};
	}
};
