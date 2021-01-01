#include "lipch.h"
#include "AudioCore.h"

#define AL_LIBTYPE_STATIC
#include "AL/al.h"

#ifdef LI_ENABLE_ASSERTS
namespace Li
{
	void ALClearError()
	{
		while (alGetError() != AL_NO_ERROR);
	}

	bool ALCheckError(const char* function, const char* file, int line)
	{
		while (ALenum error = alGetError())
		{
			LI_CORE_ERROR("[OpenAL Error] {} : {} : {} : {}", error, function, file, line);
			return false;
		}
		return true;
	}
}
#endif