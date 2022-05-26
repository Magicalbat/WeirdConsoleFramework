project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	location "GLFW"

	targetdir ("%{prj.location}/bin/" .. outputdir .. "%{prj.name}")
	objdir ("%{prj.location}/bin-int/" .. outputdir .. "%{prj.name}")

	file_paths = {
		"include/GLFW/glfw3.h",
		"include/GLFW/glfw3native.h",

		"src/context.c",
		"src/init.c",
		"src/input.c",
		"src/monitor.c",
		"src/vulkan.c",
		"src/window.c"
	}

	for i, path in ipairs(file_paths) do
		file_paths[i] = "%{prj.location}/" .. path
	end

	files(file_paths)

	filter "system:windows"
		systemversion "latest"

		file_paths = {
			"src/osmesa_context.c",
			"src/wgl_context.c",
			"src/egl_context.c",
			"src/win32_init.c",
			"src/win32_joystick.c",
			"src/win32_monitor.c",
			"src/win32_thread.c",
			"src/win32_time.c",
			"src/win32_window.c"
		}

		for i, path in ipairs(file_paths) do
			file_paths[i] = "%{prj.location}/" .. path
		end

		files(file_paths)

		defines
		{
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter {"system:windows", "configurations:Release"}
		buildoptions "/MT"
