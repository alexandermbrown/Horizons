#pragma once

#ifdef LI_DEBUG
#define LI_ENABLE_ASSERTS
#endif

#ifdef LI_ENABLE_ASSERTS
#	define LI_DEBUG_BREAK() __debugbreak()
#	define LI_VERIFY(x, ...) { if(!(x)) { ::Li::Log::Fatal("Assertion Failed: {0}", __VA_ARGS__); LI_DEBUG_BREAK(); abort(); } }
#	define LI_CORE_VERIFY(x, ...) { if(!(x)) { ::Li::Log::CoreFatal("Assertion Failed: {0}", __VA_ARGS__); LI_DEBUG_BREAK(); abort(); } }
#	define LI_ASSERT LI_VERIFY
#	define LI_CORE_ASSERT LI_CORE_VERIFY
#else
#	define LI_DEBUG_BREAK() ((void)0)
#	define LI_VERIFY(x, ...) x
#	define LI_CORE_VERIFY(x, ...) x

#	define LI_ASSERT(x, ...) ((void)0)
#	define LI_CORE_ASSERT(x, ...) ((void)0)
#endif
