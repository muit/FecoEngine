// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "eastl/unique_ptr.h"
#include <entt/entity/registry.hpp>

#include "Component.h"
#include "System.h"

#include "Gameplay/Components/CEntity.h"
#include "Core/Events/Broadcast.h"


using EntityId = u32;
class Archive;


class ECSManager : public Object {
	CLASS(ECSManager, Object)


	entt::Registry<EntityId> registry;

	TArray<GlobalPtr<System>> systems;

	TArray<eastl::unique_ptr<Component>> singletonComponents;

public:


	void BeginPlay() {
		RegistrySingletons();
		RegistrySystems();

		IterateSystems([](Ptr<System> system) {
			system->BeginPlay();
		});
	}

	void Tick(float deltaTime) {
		IterateSystems([deltaTime](Ptr<System> system) {
			system->Tick(deltaTime);
		});
	}

	void EndPlay() {
		IterateSystems([](Ptr<System> system) {
			system->EndPlay();
		});
	}


	/**************************************************************
	 * Begin ENTITIES
	 */

	EntityId CreateEntity(Name entityName, bool bTransient = false)
	{
		EntityId entity = registry.create();
		Assign<CEntity>(entity, entityName, bTransient);

		onEntityCreated.DoBroadcast(entity);
		return entity;
	}

	void DestroyEntity(EntityId entity)
	{
		onEntityDestroyed.DoBroadcast(entity);
		registry.destroy(entity);
	}

	bool IsValid(EntityId entity) const
	{
		return registry.valid(entity);
	}

	template<typename C, typename... Args>
	C& Assign(EntityId entity, Args... args)
	{
		if (registry.has<C>(entity))
			return registry.replace<C>(entity, eastl::forward<Args>(args)...);

		return registry.assign<C>(entity, eastl::forward<Args>(args)...);
	}


	/**************************************************************
	 * Begin SERIALIZATION
	 */

	virtual bool Serialize(Archive& ar) override;

	void SerializeEntity(Archive& ar, EntityId entity);

private:

	template<typename CompType>
	void SerializeComponent(Archive& ar, const EntityId& entity) {
		static_assert(eastl::is_convertible< CompType, Component >::value, "Type is not a Component!");

		if (ar.IsLoading())
		{
			ar.BeginObject(CompType::StaticStruct()->GetSName());
			if(ar.IsObjectValid())
			{
				CompType& comp = Assign<CompType>(entity);
				comp.SerializeReflection(ar);

				if constexpr (ClassTraits<CompType>::HasPostSerialize)
					comp.PostSerialize(true);
			}
			ar.EndObject();
		}
		// Saving
		else if (registry.has<CompType>(entity))
		{
			ar.BeginObject(CompType::StaticStruct()->GetSName());
			{
				CompType& comp = registry.get<CompType>(entity);
				comp.SerializeReflection(ar);

				if constexpr (ClassTraits<CompType>::HasPostSerialize)
					comp.PostSerialize(false);
			}
			ar.EndObject();
		}
	}


public:

	/**************************************************************
	 * Begin SYSTEMS
	 */

	void IterateSystems(eastl::function<void(Ptr<System>)> callback) const {
		for (const GlobalPtr<System>& system : systems)
		{
			if (system)
			{
				callback(system);
			}
		}
	}

	template<typename... Components>
	auto View() {
		return registry.view<Components...>();
	};

	u32 GetEntityCount() {
		return (i32)registry.size();
	};

private:

	void RegistrySystems();

	template <typename T>
	void RegistrySystem() {
		systems.Add(Create<T>(GetSelf()));
	}


public:

	/**************************************************************
	 * Begin COMPONENT SINGLETONS
	 * Singleton components are global and shared between all systems
	 */

	void RegistrySingletons();

	template <typename C>
	void RegistrySingleton() {
		singletonComponents.Add(eastl::make_unique<C>());
	}

	template<typename C>
	C* FindSingleton() {
		static_assert(eastl::is_convertible<C, Component>::value, "Type is not a Component!");

		const auto* foundPtr = singletonComponents.Find([](const auto& comp) {
			return comp->GetStruct() == C::StaticStruct();
		});

		return foundPtr ? static_cast<C*>(foundPtr->get()) : nullptr;
	}

	template<typename C>
	bool HasSingleton() {
		static_assert(eastl::is_convertible<C, Component>::value, "Type is not a Component!");

		return singletonComponents.Contains([](const auto& comp) {
			return comp->GetStruct() == C::StaticStruct();
		});
	}


private:

	Broadcast<EntityId> onEntityCreated;
	Broadcast<EntityId> onEntityDestroyed;

public:

	const Broadcast<EntityId>& OnEntityCreated() const { return onEntityCreated; }
	const Broadcast<EntityId>& OnEntityDestroyed() const { return onEntityDestroyed; }
};
