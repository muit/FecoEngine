// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <unordered_map>

#include "EngineTypes.h"
#include "Property.h"


class Class {
protected:

	Name name;
	Class* parent;
	std::vector<Class*> children;

	typedef std::unordered_map<Name, std::unique_ptr<PropertyBase>> PropertyMap;

	PropertyMap properties;

public:

	Class() {}
	Class(const Class&) = delete;
	Class& operator=(const Class&) = delete;
	virtual ~Class() {}

	Name GetName() const { return name; }
	Class* GetParentClass() const { return parent; }

	template<bool bIncludeSelf = false, bool bIsFirstCall = true>
	void GetAllChildClasses(std::vector<Class*>& outChildren) {
		if(bIsFirstCall)
			outChildren.clear();

		if (bIncludeSelf)
			outChildren.push_back(this);

		for (auto* child : children)
		{
			outChildren.push_back(child);
			child->GetAllChildClasses<false, false>(outChildren);
		}
	}

	template<typename VarType>
	const Property<VarType>* FindProperty(const Name& name) const
	{
		const auto propIt = properties.find(name);
		if (propIt == properties.end())
			return nullptr;
		else
			return dynamic_cast<const Property<VarType>*>((*propIt).second.get());
	}

	const PropertyMap& GetAllProperties() { return properties; }

public:

	/** GENERATION */
	void RegistryChildren(Class* child)
	{
		children.push_back(child);
	}
};
