project "libogg"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
	targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.c",
		"src/**.h",
		"include/**.h"
	}
	
	includedirs {
		"include"
	}
	
	warnings "Off"

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
