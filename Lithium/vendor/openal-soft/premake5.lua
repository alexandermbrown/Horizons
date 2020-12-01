project "openal-soft"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
    
	targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files {

        "al/**.h",
        "al/**.cpp",

        "alc/*.h",
        "alc/*.cpp",
        "alc/effects/**.h",
        "alc/effects/**.cpp",
        "alc/filters/**.h",
        "alc/filters/**.cpp",

        "alc/mixer/mixer_c.cpp",
        "alc/mixer/mixer_sse.cpp",
        "alc/mixer/mixer_sse2.cpp",
        "alc/mixer/mixer_sse3.cpp",
        "alc/mixer/mixer_sse41.cpp",
        "alc/mixer/**.h",

        "alc/backends/base.h",
        "alc/backends/base.cpp",
        "alc/backends/null.h",
        "alc/backends/null.cpp",
        "alc/backends/loopback.h",
        "alc/backends/loopback.cpp",
        "alc/backends/sdl2.h",
        "alc/backends/sdl2.cpp",
        "alc/backends/wave.h",
        "alc/backends/wave.cpp",

        "common/**.h",
        "common/**.cpp",

        "./*.h"
    }

    includedirs {
        "include",
        "common",
        "alc",
        "al",
        "./",
        "../SDL2/include"
    }

    warnings "Off"

    defines {
        "AL_LIBTYPE_STATIC",
        "AL_BUILD_LIBRARY",
        "AL_ALEXT_PROTOTYPES",
        "NOMINMAX"
    }
    
	filter "system:windows"
        systemversion "latest"

        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "WIN32",
            "_WIN32",
            "_WINDOWS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        defines { "NDEBUG" }

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        defines { "NDEBUG" }
        