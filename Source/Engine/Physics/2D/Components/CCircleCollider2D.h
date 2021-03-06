// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "../PhysicsTypes2D.h"
#include "CCollider2D.h"


/**
 * A Circle collider is a collider with a circular shape.
 * @see CCollider2D
 */
class CCircleCollider2D : public CCollider2D {
	STRUCT(CCircleCollider2D, CCollider2D)

protected:

	PROP(float, radius, DetailsEdit);
	float radius = 0.5f;


public:

	void SetRadius(float inRadius)
	{
		MarkDirty();
		radius = inRadius;
	}

	virtual void FillDefinition(b2FixtureDef& def, Shape* shape, float angle) const override
	{
		CCollider2D::FillDefinition(def, shape, angle);

		auto* cShape = static_cast<CircleShape*>(shape);
		cShape->m_radius = radius;
	}
};
