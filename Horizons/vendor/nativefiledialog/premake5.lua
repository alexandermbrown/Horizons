project "nativefiledialog"
kind "StaticLib"
staticruntime "off"

targetdir ("build/" .. outputdir .. "/%{prj.name}")
objdir ("build-int/" .. outputdir .. "/%{prj.name}")

files {
	"src/*.h",
	"src/include/*.h",
	"src/nfd_common.c",
}

includedirs {
	"src/include/"
}

warnings "Off"

filter "system:windows"
	language "C++"
	systemversion "latest"
	files { "src/nfd_win.cpp" }

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

filter "system:macosx"
	language "C"
	files {"src/nfd_cocoa.m"}

filter "system:linux"
	language "C"
	files { "src/nfd_gtk.c" }
	buildoptions {"`pkg-config --cflags gtk+-3.0`"}

filter "configurations:Debug"
	defines { "DEBUG" }
	runtime "Debug"
	symbols "on"

filter "configurations:Release"
	runtime "Release"
	optimize "on"
	defines { "NDEBUG" }

filter "configurations:Dist"
	runtime "Release"
	optimize "on"
	defines { "NDEBUG" }
	
filter "action:vs*"
    defines { "_CRT_SECURE_NO_WARNINGS" }    