workspace "Horizons"
    architecture "x86_64"
    startproject "Horizons"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    flags {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "Lithium/vendor/spdlog/include"
IncludeDir["SDL2"] = "Lithium/vendor/SDL2/include"
IncludeDir["glad"] = "Lithium/vendor/glad/include"
IncludeDir["glm"] = "Lithium/vendor/glm"
IncludeDir["imgui"] = "Lithium/vendor/imgui"
IncludeDir["stb_image"] = "Lithium/vendor/stb_image"
IncludeDir["zlib"] = "Lithium/vendor/zlib"

IncludeDir["freetype"] = "AssetBase/vendor/freetype/include"
IncludeDir["msdfgen"] = "AssetBase/vendor/msdfgen"

IncludeDir["yojimbo"] = "GameServer/vendor/yojimbo"

include "Lithium/vendor/glad"
include "Lithium/vendor/imgui"
include "Lithium/vendor/zlib"
include "AssetBase/vendor/msdfgen"
include "GameServer/vendor/yojimbo"

------------------------------ Lithium ----------------------------------

project "Lithium"
    location "Lithium"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

    pchheader "lipch.h"
    pchsource "Lithium/src/lipch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/glm/glm/**.h",
        "%{prj.name}/vendor/zstr/zstr/**.hpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.zlib}",
        "Lithium/vendor/zstr",
    }

    links {
        "glad",
        "opengl32.lib",
        "imgui",
        "zlib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "LI_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LI_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "LI_DIST"
        runtime "Release"
        optimize "on"

------------------------------ Horizons ------------------------------

project "Horizons"
    location "Horizons"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Lithium/src",
        "Lithium/vendor",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.SDL2}"
    }

    links {
        "Lithium",
        "Lithium/vendor/SDL2/lib/x64/SDL2.lib"
    }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "LI_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LI_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "LI_DIST"
        runtime "Release"
        optimize "on"

------------------------------ AssetBase ------------------------------

project "AssetBase"
    location "AssetBase"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/rapidxml/rapidxml/**.hpp",
        "%{prj.name}/vendor/zstr/zstr/**.hpp",
        "%{prj.name}/vendor/zlib/**.h",
        "%{prj.name}/vendor/zlib/**.c"
    }

    includedirs {
        "Lithium/src",
        "AssetBase/vendor/rapidxml",
        "AssetBase/vendor/zstr",
        "AssetBase/vendor/zlib",
        "%{IncludeDir.glm}",
        "%{IncludeDir.freetype}",
        "%{IncludeDir.msdfgen}"
    }

    links {
        "msdfgen"
    }
    
    debugargs { "./resources.xml", "./resources.lab" }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "LI_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LI_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "LI_DIST"
        runtime "Release"
        optimize "on"

------------------------------ GameServer ------------------------------

project "GameServer"
    location "GameServer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "GameServer/src/pch.cpp"
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "GameServer/src",
        "GameServer/vendor",
        "%{IncludeDir.yojimbo}",
    }

    links {
        "yojimbo"
    }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "LI_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LI_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "LI_DIST"
        runtime "Release"
        optimize "on"

------------------------------ ServerManager ------------------------------

project "ServerManager"
    location "ServerManager"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "ServerManager/src/pch.cpp"
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "ServerManager/src",
        "ServerManager/vendor",
        "%{IncludeDir.yojimbo}",
    }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "LI_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LI_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "LI_DIST"
        runtime "Release"
        optimize "on"