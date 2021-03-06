// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "../../Data/MeshData.h"
#include "Core/Assets/AssetInfo.h"
#include "../OpenGL.h"


struct RenderMesh
{
	u32 glVerticesId = GL_INVALID_INDEX;
	u32 glTrianglesId = GL_INVALID_INDEX;

	u32 glVertexArrayId = GL_INVALID_INDEX;

	u32 indicesCount = 0;
	u32 verticesCount = 0;


	RenderMesh() = default;
	RenderMesh(Name id, const MeshData& mesh) { Load(id, mesh); }
	RenderMesh(RenderMesh&& other)            { MoveFrom(MoveTemp(other)); }
	RenderMesh& operator=(RenderMesh&& other) { MoveFrom(MoveTemp(other)); return *this; }
	~RenderMesh() { Free(); }

	void Bind() const
	{
		glBindVertexArray(glVertexArrayId);
		glCheckError();
	}

	void Draw() const
	{
		glBindVertexArray(glVertexArrayId);
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		glCheckError();
	}

	static void Unbind() { glBindVertexArray(0); }

private:

	void Load(Name id, const MeshData& mesh);

	void Free();

	void MoveFrom(RenderMesh&& other)
	{
		glVertexArrayId = other.glVertexArrayId;
		glVerticesId    = other.glVerticesId;
		glTrianglesId   = other.glTrianglesId;
		indicesCount    = other.indicesCount;
		verticesCount   = other.verticesCount;
		other.glVertexArrayId = GL_INVALID_INDEX;
		other.glVerticesId    = GL_INVALID_INDEX;
		other.glTrianglesId   = GL_INVALID_INDEX;
		other.indicesCount = 0;
		other.glVerticesId = 0;
	}
};
