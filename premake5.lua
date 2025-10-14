workspace "ArtNetSender"
    configurations { "Debug", "Release" , "Dist" }
    architecture "x86_64"
    startproject "ArtNet-Sender"
    
project "ArtNet-Sender"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    staticruntime "Off"
    characterset "Unicode"
    exceptionhandling "On"
    warnings "Extra"
    flags { "MultiProcessorCompile" }

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/bin-int/%{cfg.buildcfg}"

    files { "Source/**.h", "Source/**.cpp" }
    includedirs { "Source" }

    filter "system:windows"
        systemversion "latest"
        defines { "PLATFORM_WINDOWS" }

    filter "system:linux"
        systemversion "latest"
        defines { "PLATFORM_LINUX" }
        links { "pthread" }
    
    filter "system:macosx"
        systemversion "latest"
        defines { "PLATFORM_MAC" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "configurations:Dist"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"
            