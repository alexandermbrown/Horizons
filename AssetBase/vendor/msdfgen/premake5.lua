project "msdfgen"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files {
		"msdfgen/**.h",
		"msdfgen/**.hpp",
		"msdfgen/**.cpp"
	}

	includedirs {
		"freetype/include"
	}

	links {
		"freetype/win64/freetype.lib"
	}

	warnings "Off"

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
