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
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "vendor"
    }

    filter "configurations:debug"
        defines { "CORTEX_DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "CORTEX_RELEASE" }
        optimize "On"

    filter "platforms:macosx"
        targetextension ("")
        links { "SDL2" }


    filter "platforms:web"
        architecture "x86"
        targetname "index"
        targetsuffix ""
        targetextension (".html")

        buildoptions {
            "-s USE_SDL=2",
        }

        linkoptions {
            "-s USE_SDL=2",
            "-s ALLOW_MEMORY_GROWTH",
            "--preload-file ../assets@/",
        }