project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files {
		"include/yaml-cpp/**.h",
		"src/**.cpp",
		"src/**.h"
	}

	includedirs {
		"include"
	}

--warnings "Off"

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
