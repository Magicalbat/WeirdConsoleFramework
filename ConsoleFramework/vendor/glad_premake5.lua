project "glad"
	kind "StaticLib"
	language "C"
	location "glad"

	targetdir ("%{prj.location}/bin/" .. outputdir .. "%{prj.name}")
	objdir ("%{prj.location}/bin-int/" .. outputdir .. "%{prj.name}")

	file_paths = {
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	for i, path in ipairs(file_paths) do
		file_paths[i] = "%{prj.location}/" .. path
	end

    files (file_paths)

	includedirs
	{
		"%{prj.location}/include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter {"system:windows", "configurations:Release"}
		buildoptions "/MT"
