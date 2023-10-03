workspace "raytracer"
    architecture "x64"
    location "build"
    configurations { "Release", "Debug" }

project "raytracer"
    kind "ConsoleApp"
    language "C++"
    location "build"

    files { "src/**.cpp", "src/**.hpp", "src/**.h", "libraries/include/ImGui/**.cpp" }

    includedirs "libraries/*"
    libdirs "libraries/lib"
    links { "SDL2", "SDL2main", "SDL2_image", "winmm" }

    filter "configurations:Release"
        optimize "Full"
        defines "RELEASE"    

    filter "configurations:Debug"
        defines "Debug"