#include "lipch.h"
#include "OpenGLCore.h"

#include <glad/glad.h>

#ifdef LI_ENABLE_ASSERTS
namespace Li
{
	const char* GLErrorToString(GLenum error)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:
			return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:
			return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_CONTEXT_LOST:
			return "GL_CONTEXT_LOST";
		default:
			return "UNKNOWN";
		}
	}

	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLCheckError(const char* function, const char* file, int line)
	{
		GLenum error = glGetError();
		while (error != GL_NO_ERROR)
		{
			Log::CoreError("[OpenGL Error] {} : {} : {} : {}", GLErrorToString(error), function, file, line);
			return false;
		}
		return true;
	}
}
#endif