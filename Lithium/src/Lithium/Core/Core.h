#pragma once

#include <memory>

#ifdef LI_DEBUG
#define LI_ENABLE_ASSERTS
#endif

#ifdef LI_ENABLE_ASSERTS
#	define LI_RUN_ASSERT(x, ...) { if(!(x)) { LI_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#	define LI_CORE_RUN_ASSERT(x, ...) { if(!(x)) { LI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#	define LI_ASSERT LI_RUN_ASSERT
#	define LI_CORE_ASSERT LI_CORE_RUN_ASSERT
#else
#	define LI_RUN_ASSERT(x, ...) x
#	define LI_CORE_RUN_ASSERT(x, ...) x

#	define LI_ASSERT(x, ...)
#	define LI_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define LI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#ifdef LI_PLATFORM_WINDOWS
#	define LI_INCLUDE_OPENGL
#	define LI_INCLUDE_D3D11
#else
#	define LI_INCLUDE_OPENGL
#endif

namespace li 
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}