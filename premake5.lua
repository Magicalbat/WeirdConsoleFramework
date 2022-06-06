workspace "WeirdConsoleFramework"
    configurations {"Debug", "Release"}
    architecture "x64"
    startproject "WeirdConsoleFramework"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "%{prj.name}/vendor/GLFW/include"
IncludeDirs["glad"] = "%{prj.name}/vendor/glad/include"
IncludeDirs["stb_image"] = "%{prj.name}/vendor/stb_image/include"
IncludeDirs["glm"] = "%{prj.name}/vendor/glm"

include "WeirdConsoleFramework/vendor/GLFW_premake5.lua"
include "WeirdConsoleFramework/vendor/glad_premake5.lua"

project "WeirdConsoleFramework"
    location "WeirdConsoleFramework"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

        "%{prj.name}/vendor/stb_image/include/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",

        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDirs['GLFW']}",
        "%{IncludeDirs['glad']}",
        "%{IncludeDirs['stb_image']}",
        "%{IncludeDirs['glm']}"
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