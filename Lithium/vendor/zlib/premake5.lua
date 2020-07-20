project "zlib"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
    targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "**.h",
        "**.c"
    }

    warnings "Off"
    
    filter "system:windows"
        systemversion "latest"

        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		runtime "Release"
		optimize "on"

