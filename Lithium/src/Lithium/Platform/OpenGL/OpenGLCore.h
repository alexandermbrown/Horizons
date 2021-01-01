#pragma once

#include "Lithium/Core/Core.h"

#ifdef LI_ENABLE_ASSERTS
#	define GLCall(x) GLClearError();\
	x;\
	LI_CORE_ASSERT(GLCheckError(#x, __FILE__, __LINE__), "OpenGL error - see log for details")

namespace Li
{
	void GLClearError();
	bool GLCheckError(const char* function, const char* file, int line);
}

#else
#	define GLCall(x) x
#endif