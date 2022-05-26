workspace "ConsoleFramework"
    configurations {"Debug", "Release"}
    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "%{prj.name}/vendor/GLFW/include"
IncludeDirs["glad"] = "%{prj.name}/vendor/glad/include"

include "ConsoleFramework/vendor/GLFW_premake5.lua"
include "ConsoleFramework/vendor/glad_premake5.lua"

project "ConsoleFramework"
    location "ConsoleFramework"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDirs['GLFW']}",
        "%{IncludeDirs['glad']}"
    }

    links
    {
        "GLFW",
        "glad",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
            "GLFW_INCLUDE_NONE",
            "CF_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "DEBUG"
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines "NDEBUG"
        buildoptions "/MD"
        optimize "On"