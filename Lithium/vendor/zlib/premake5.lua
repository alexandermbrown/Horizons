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
    
    filter "system:windows"
        systemversion "latest"
        staticruntime "On"
