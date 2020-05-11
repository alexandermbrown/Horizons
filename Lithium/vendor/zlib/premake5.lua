project "zlib"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
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
        staticruntime "On"

        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }
