// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Core/Containers/Array.h"


class StaticMesh;

struct Vertex
{
	v3 position;
	v3 normal;
	v2 uv;
};

using VertexBuffer = TArray<Vertex>;
using Triangle = v3_u32;
using TriangleBuffer = TArray<Triangle>;

struct MeshData
{
	VertexBuffer vertices;
	TriangleBuffer triangles;

public:

	MeshData() = default;

	void FromAssimpScene(const struct aiScene* scene, StaticMesh& asset);

	FORCEINLINE const VertexBuffer& GetVertices() const { return vertices; }
	FORCEINLINE const TriangleBuffer& GetTriangles() const { return triangles; }

private:

	void ProcessNode(struct aiNode *node, const struct aiScene* scene, StaticMesh& asset);
	void ProcessMesh(struct aiMesh *mesh, const struct aiScene* scene, StaticMesh& asset);
};
