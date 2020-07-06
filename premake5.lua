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
IncludeDir["zstr"] = "Lithium/vendor/zstr"
IncludeDir["openal"] = "Lithium/vendor/openal-soft/include"
IncludeDir["libogg"] = "Lithium/vendor/libogg/include"
IncludeDir["libvorbis"] = "Lithium/vendor/libvorbis/include"
IncludeDir["simpleini"] = "Lithium/vendor/simpleini/include"
IncludeDir["readerwriterqueue"] = "Lithium/vendor/readerwriterqueue/include"
IncludeDir["layout"] = "Lithium/vendor/layout/include"

IncludeDir["freetype"] = "AssetBase/vendor/freetype/include"
IncludeDir["msdfgen"] = "AssetBase/vendor/msdfgen"

IncludeDir["yojimbo"] = "GameServer/vendor/yojimbo"

IncludeDir["entt"] = "Horizons/vendor/entt/include"
IncludeDir["box2d"] = "Horizons/vendor/box2d/include"

group "vendor"
include "Lithium/vendor/glad"
include "Lithium/vendor/imgui"
include "Lithium/vendor/zlib"
include "Lithium/vendor/libvorbis"
include "Lithium/vendor/openal-soft"
include "AssetBase/vendor/msdfgen"
include "GameServer/vendor/yojimbo"
include "Horizons/vendor/box2d"
group  ""

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
        "%{IncludeDir.zstr}",
        "%{IncludeDir.openal}",
        "%{IncludeDir.libogg}",
        "%{IncludeDir.libvorbis}",
        "%{IncludeDir.readerwriterqueue}",
        "%{IncludeDir.layout}"
    }

    links {
        "glad",
        "imgui",
        "zlib",
        "libvorbis",
        "openal-soft"
    }

    filter "system:windows"
        systemversion "latest"

        links {
            "Lithium/vendor/SDL2/lib/x64/SDL2.lib"
        }

        defines {
            "LI_PLATFORM_WINDOWS"
        }

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
    dpiawareness "HighPerMonitor"

    targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Horizons/src/pch.cpp"
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Lithium/src",
        "Horizons/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.box2d}",
        "%{IncludeDir.readerwriterqueue}",
        "%{IncludeDir.layout}",
        "%{IncludeDir.simpleini}"
    }

    links {
        "Lithium",
        "box2d"
    }

    filter "system:windows"
        systemversion "latest"
        
        defines {
            "LI_PLATFORM_WINDOWS",
            "HZ_PLATFORM_WINDOWS"
        }
        
    filter "configurations:Debug"
        defines {
            "LI_DEBUG",
            "HZ_DEBUG"
        }
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

    pchheader "pch.h"
    pchsource "AssetBase/src/pch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/rapidxml/rapidxml/**.hpp",
        "%{prj.name}/vendor/zstr/zstr/**.hpp",
        "%{prj.name}/vendor/zlib/**.h",
        "%{prj.name}/vendor/zlib/**.c"
    }

    includedirs {
        "AssetBase/src",
        "Lithium/src",
        "AssetBase/vendor/rapidxml",
        "%{IncludeDir.zlib}",
        "%{IncludeDir.zstr}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.freetype}",
        "%{IncludeDir.msdfgen}",
        "%{IncludeDir.simpleini}"
    }

    links {
        "msdfgen",
        "zlib"
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