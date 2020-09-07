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
IncludeDir["openal"] = "Lithium/vendor/openal-soft/include"
IncludeDir["libogg"] = "Lithium/vendor/libogg/include"
IncludeDir["libvorbis"] = "Lithium/vendor/libvorbis/include"
IncludeDir["simpleini"] = "Lithium/vendor/simpleini/include"
IncludeDir["readerwriterqueue"] = "Lithium/vendor/readerwriterqueue/include"
IncludeDir["layout"] = "Lithium/vendor/layout/include"
IncludeDir["harfbuzz"] = "Lithium/vendor/harfbuzz/src"
IncludeDir["utfcpp"] = "Lithium/vendor/utfcpp/include"

IncludeDir["freetype"] = "AssetBase/vendor/freetype/include"
IncludeDir["msdfgen"] = "AssetBase/vendor/msdfgen"
IncludeDir["flatbuffers"] = "AssetBase/vendor/flatbuffers/include"
IncludeDir["lab_serial"] = "FlatBuffers/include"

IncludeDir["yojimbo"] = "GameServer/vendor/yojimbo"
IncludeDir["nlohmann_json"] = "GameServer/vendor/nlohmann_json/include"

IncludeDir["httplib"] = "ServerManager/vendor/httplib/include"
IncludeDir["openssl"] = "ServerManager/vendor/OpenSSL/vc-win64a/include"
IncludeDir["libb64"] = "ServerManager/vendor/libb64/include"

IncludeDir["entt"] = "Horizons/vendor/entt/include"
IncludeDir["box2d"] = "Horizons/vendor/box2d/include"
IncludeDir["steam"] = "Horizons/vendor/steam/include"
IncludeDir["lua"] = "Horizons/vendor/lua/include"

group "vendor"
include "Lithium/vendor/glad"
include "Lithium/vendor/imgui"
include "Lithium/vendor/zlib"
include "Lithium/vendor/libvorbis"
include "Lithium/vendor/openal-soft"
include "Lithium/vendor/harfbuzz"
include "AssetBase/vendor/msdfgen"
include "GameServer/vendor/yojimbo"
include "ServerManager/vendor/libb64"
include "Horizons/vendor/box2d"
group  ""

------------------------------ Lithium ----------------------------------

project "Lithium"
    location "Lithium"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

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
    }

    includedirs {
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.zlib}",
        "%{IncludeDir.openal}",
        "%{IncludeDir.libogg}",
        "%{IncludeDir.libvorbis}",
        "%{IncludeDir.readerwriterqueue}",
        "%{IncludeDir.layout}",
        "%{IncludeDir.flatbuffers}",
        "%{IncludeDir.lab_serial}",
        "%{IncludeDir.harfbuzz}",
        "%{IncludeDir.utfcpp}"
    }

    links {
        "glad",
        "zlib",
        "libvorbis",
        "openal-soft",
        "SDL2",
        "harfbuzz"
    }

    filter "system:windows"
        systemversion "latest"

        libdirs {
            "Lithium/vendor/SDL2/lib/x64"
        }

        defines {
            "LI_PLATFORM_WINDOWS"
        }

        links {
            "d3d11.lib",
            "dxgi.lib",
            "d3dcompiler.lib"
        }

    filter "configurations:Debug"
        defines "LI_DEBUG"
        runtime "Debug"
        symbols "on"
        links "imgui"
        includedirs { "%{IncludeDir.imgui}" }

    filter "configurations:Release"
        defines "LI_RELEASE"
        runtime "Release"
        optimize "on"
        links "imgui"
        includedirs { "%{IncludeDir.imgui}" }
    
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
    staticruntime "off"
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
        "%{IncludeDir.entt}",
        "%{IncludeDir.box2d}",
        "%{IncludeDir.readerwriterqueue}",
        "%{IncludeDir.layout}",
        "%{IncludeDir.simpleini}",
        "%{IncludeDir.steam}",
        "%{IncludeDir.flatbuffers}",
        "%{IncludeDir.lab_serial}",
        "%{IncludeDir.harfbuzz}",
        "%{IncludeDir.lua}"
    }

    links {
        "Lithium",
        "box2d",
        "lua51",
        "luajit"
    }

    filter "system:windows"
        systemversion "latest"
        
        defines {
            "LI_PLATFORM_WINDOWS",
            "HZ_PLATFORM_WINDOWS"
        }

        libdirs { 
            "Horizons/vendor/steam/lib/win64",
            "Horizons/vendor/lua/src/build/win64"
         }
        links { "steam_api64" }
        
    filter "configurations:Debug"
        defines {
            "LI_DEBUG",
            "HZ_DEBUG"
        }
        runtime "Debug"
        symbols "on"
        includedirs { "%{IncludeDir.imgui}" }

    filter "configurations:Release"
        defines "LI_RELEASE"
        runtime "Release"
        optimize "on"
        includedirs { "%{IncludeDir.imgui}" }

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
    staticruntime "off"

    targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "AssetBase/src/pch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/rapidxml/rapidxml/**.hpp",
        "%{prj.name}/vendor/zlib/**.h",
        "%{prj.name}/vendor/zlib/**.c"
    }

    includedirs {
        "AssetBase/src",
        "Lithium/src",
        "AssetBase/vendor/rapidxml",
        "%{IncludeDir.zlib}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.freetype}",
        "%{IncludeDir.msdfgen}",
        "%{IncludeDir.simpleini}",
        "%{IncludeDir.flatbuffers}",
        "%{IncludeDir.lab_serial}"
    }

    links {
        "msdfgen",
        "zlib"
    }
    
    debugargs { "./resources.xml", "./resources.lab" }

    filter "system:windows"
        systemversion "latest"

        links {
            "d3d11.lib",
            "dxgi.lib",
            "d3dcompiler.lib"
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

------------------------------ GameServer ------------------------------

project "GameServer"
    location "GameServer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

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
        "%{IncludeDir.nlohmann_json}",
        "%{IncludeDir.libb64}",
        "%{IncludeDir.steam}"
    }

    links {
        "yojimbo",
        "libb64"
    }

    filter "system:windows"
        systemversion "latest"

        libdirs { "Horizons/vendor/steam/lib/win64" }
        links { "steam_api64" }
        
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
    staticruntime "off"

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
        "%{IncludeDir.httplib}",
        "%{IncludeDir.yojimbo}",
        "%{IncludeDir.nlohmann_json}",
        "%{IncludeDir.openssl}",
        "%{IncludeDir.libb64}",
        "%{IncludeDir.steam}"
    }

    links {
        "libcrypto",
        "libssl",
        "libb64"
    }

    filter "system:windows"
        systemversion "latest"

        libdirs {
            "ServerManager/vendor/OpenSSL/vc-win64a/lib",
            "Horizons/vendor/steam/lib/win64"
        }
        links { "steam_api64" }
        
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