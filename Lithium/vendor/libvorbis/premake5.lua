include "../libogg"

project "libvorbis"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
	targetdir ("build/" .. outputdir .. "/%{prj.name}")
    objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files {
        "src/analysis.c",
        "src/backends.h",
        "src/bitrate.c",
        "src/bitrate.h",
        "src/block.c",
        "src/codebook.c",
        "src/codebook.h",
        "src/codec_internal.h",
        "src/envelope.c",
        "src/envelope.h",
        "src/modes/floor_all.h",
        "src/books/floor/floor_books.h",
        "src/floor0.c",
        "src/floor1.c",
        "src/highlevel.h",
        "src/info.c",
        "src/lookup.c",
        "src/lookup.h",
        "src/lookup_data.h",
        "src/lpc.c",
        "src/lpc.h",
        "src/lsp.c",
        "src/lsp.h",
        "src/mapping0.c",
        "src/masking.h",
        "src/mdct.c",
        "src/mdct.h",
        "src/misc.h",
        "src/os.h",
        "src/psy.c",
        "src/psy.h",
        "src/modes/psych_8.h",
        "src/modes/psych_11.h",
        "src/modes/psych_16.h",
        "src/modes/psych_44.h",
        "src/registry.c",
        "src/registry.h",
        "src/books/coupled/res_books_stereo.h",
        "src/books/uncoupled/res_books_uncoupled.h",
        "src/res0.c",
        "src/modes/residue_8.h",
        "src/modes/residue_16.h",
        "src/modes/residue_44.h",
        "src/modes/residue_44u.h",
        "src/scales.h",
        "src/modes/setup_8.h",
        "src/modes/setup_11.h",
        "src/modes/setup_16.h",
        "src/modes/setup_22.h",
        "src/modes/setup_32.h",
        "src/modes/setup_44.h",
        "src/modes/setup_44u.h",
        "src/modes/setup_X.h",
        "src/sharedbook.c",
        "src/smallft.c",
        "src/smallft.h",
        "src/synthesis.c",
        "src/vorbisenc.c",
        "src/vorbisfile.c",
        "src/window.c",
        "src/window.h",
		"include/**.h"
    }

    links {
        "libogg"
    }

    includedirs {
        "../libogg/include",
        "include"
    }

    defines {
        "LIBVORBIS_EXPORTS"
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