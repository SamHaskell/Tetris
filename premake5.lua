workspace "Tetris"
    configurations { "debug", "release" }
    platforms { "macosx", "web" }
    location "build"

project "Tetris"
    kind "WindowedApp"
    location "build"
    language "C++"
    cppdialect "C++11"

    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}/"
    architecture "x86_64"

    files {
        -- source
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp",

        -- soloud
        "vendor/soloud/src/**.h",
        "vendor/soloud/src/**.c",
        "vendor/soloud/src/**.cpp",

        "vendor/soloud/include/**.h",
        "vendor/soloud/include/**.c",
        "vendor/soloud/include/**.cpp",
    }

    includedirs {
        "src",
        "vendor"
    }

    defines {
        "WITH_SDL2_STATIC" -- select soloud backend
    }

    externalincludedirs {
        "vendor/soloud/include",
    }

    filter "configurations:debug"
        defines { "CORTEX_DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "CORTEX_RELEASE" }
        optimize "On"

    filter "platforms:macosx"
        targetextension ("")
        links { "SDL2", "SDL2_TTF" }


    filter "platforms:web"
        architecture "x86"
        targetname "index"
        targetsuffix ""
        targetextension (".html")

        defines { "CORTEX_NO_LOGGING" }

        buildoptions {
            "-s USE_SDL=2",
            "-s USE_SDL_TTF=2",
        }

        linkoptions {
            "-s USE_SDL=2",
            "-s USE_SDL_TTF=2",
            "-s ALLOW_MEMORY_GROWTH",
            "--preload-file ../assets@/",
        }