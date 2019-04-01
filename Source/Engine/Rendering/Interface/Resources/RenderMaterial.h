// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Strings/String.h"
#include "Core/Log.h"


struct RenderMaterial {
	GLint programId;

	RenderMaterial() {}
	RenderMaterial(const String& vertexCode, const String& fragmentCode) {
		CompileProgram(vertexCode, fragmentCode);
		glUseProgram(programId);
	}

	RenderMaterial(RenderMaterial&& other) : programId{other.programId} {
		other.programId = 0;
	}
	RenderMaterial& operator=(RenderMaterial&& other) {
		programId = other.programId;
		other.programId = 0;
		return *this;
	}

	~RenderMaterial() {
		glDeleteProgram(programId);
	}

	void Use() {
		glUseProgram(programId);
	}

private:

	void CompileProgram(const String& vertexCode, const String& fragmentCode);

	void LogShaderError(GLint shaderId);
	void LogProgramError();
};
