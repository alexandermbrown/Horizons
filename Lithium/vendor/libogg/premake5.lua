project "libogg"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
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
        staticruntime "On"
