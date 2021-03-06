// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"

#include <EASTL/unique_ptr.h>

#include "EngineTypes.h"
#include "Core/Containers/Map.h"
#include "ReflectionTags.h"
#include "Property.h"


using PropertyMap = TMap<Name, Property*>;


/** Smallest reflection type that contains all basic class or struct data */
class BaseType
{

protected:

	Name name;
	ReflectionTags tags;

	BaseType* parent;
	TArray<BaseType*> children;

	TMap<Name, eastl::unique_ptr<Property>> properties{ Name::None() };


public:

	BaseType() : tags{}, parent{nullptr} {}
	BaseType(const BaseType&) = delete;
	BaseType& operator=(const BaseType&) = delete;
	virtual ~BaseType() {}


	/** Type */
	const Name& GetName() const { return name; }
	const String& GetSName() const { return GetName().ToString(); }

	bool HasTag(ReflectionTags tag) const { return (tags & tag) > 0; }

protected:

	void __GetAllChildren(TArray<BaseType*>& outChildren);
	BaseType* __FindChild(Name className) const;

public:

	bool IsChildOf(const BaseType* other) const;
	bool IsParentOf(const BaseType* other) const { return other->IsChildOf(this); }


	/** Properties */

	const Property* FindProperty(Name propertyName) const;
	void GetOwnProperties(PropertyMap& outProperties) const;
	void GetAllProperties(PropertyMap& outProperties) const;


	/** REGISTRY */

	/** Registry a class */
	void __Registry(Name inName) { name = inName; }

	/** Registry a class with a parent */
	template<typename Super>
	void __Registry(Name inName)
	{
		parent = Super::StaticType();
		parent->__RegistryChild(this);
		__Registry(inName);
	}

	/** Called internally to registry class tags */
	void __RegistryTags(ReflectionTags inTags) { tags = inTags; }

	/** Called internally to registry a property of a class */
	template<typename VarType>
	void __RegistryProperty(Name name, eastl::function<VarType* (BaseStruct*)>&& access, ReflectionTags tags)
	{
		properties.Insert(name, eastl::unique_ptr<Property>(
			new TProperty<VarType>(
				this,
				GetReflectableName<VarType>(),
				name,
				MoveTemp(access),
				tags
			)
		));
	}

private:

	void __RegistryChild(BaseType* child)
	{
		children.Add(child);
	}
};
