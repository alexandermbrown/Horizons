#pragma once

#include <memory>

#ifdef LI_DEBUG
#define LI_ENABLE_ASSERTS
#endif

#ifdef LI_ENABLE_ASSERTS
#	define LI_DEBUG_BREAK() __debugbreak()
#	define LI_RUN_ASSERT(x, ...) { if(!(x)) { LI_ERROR("Assertion Failed: {0}", __VA_ARGS__); LI_DEBUG_BREAK(); } }
#	define LI_CORE_RUN_ASSERT(x, ...) { if(!(x)) { LI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); LI_DEBUG_BREAK(); } }
#	define LI_ASSERT LI_RUN_ASSERT
#	define LI_CORE_ASSERT LI_CORE_RUN_ASSERT
#else
#	define LI_DEBUG_BREAK()
#	define LI_RUN_ASSERT(x, ...) x
#	define LI_CORE_RUN_ASSERT(x, ...) x

#	define LI_ASSERT(x, ...)
#	define LI_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define LI_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef LI_PLATFORM_WINDOWS
#	define LI_INCLUDE_OPENGL
#	define LI_INCLUDE_D3D11
#else
#	define LI_INCLUDE_OPENGL
#endif

namespace Li 
{
	template<typename T>
	using Unique = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Unique<T> MakeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}