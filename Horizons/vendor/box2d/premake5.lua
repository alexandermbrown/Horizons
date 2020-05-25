project "box2d"
kind "StaticLib"
language "C++"
staticruntime "on"

targetdir ("build/" .. outputdir .. "/%{prj.name}")
objdir ("build-int/" .. outputdir .. "/%{prj.name}")

files {
	"include/**.h",
	"src/**.h",
	"src/**.cpp"
}

includedirs {
	"include",
	"src"
}

warnings "Off"

filter "system:windows"
	systemversion "latest"
	staticruntime "On"

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

filter "configurations:Debug"
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
	