workspace "MarsEngine"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "MarsEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "MarsEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "MarsEngine/vendor/imgui"
IncludeDir["glm"] = "MarsEngine/vendor/glm"
IncludeDir["stb_image"] = "MarsEngine/vendor/stb_image"
IncludeDir["entt"] = "MarsEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "MarsEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "MarsEngine/vendor/ImGuizmo"
IncludeDir["box2d"] = "MarsEngine/vendor/box2d/include"

group "Dependencies"
	include "MarsEngine/vendor/GLFW"
	include "MarsEngine/vendor/Glad"
	include "MarsEngine/vendor/imgui"
	include "MarsEngine/vendor/yaml-cpp"
	include "MarsEngine/vendor/box2d"
group ""

project "MarsEngine"
	location "MarsEngine"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "MarsEngine/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.box2d}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",
		"box2d"
	}

	filter "MarsEngine/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ME_PLATFORM_WINDOWS",
			"ME_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
	
	filter "configurations:Debug"
		defines "ME_DEBUG"
		buildoptions "/MTd"
		symbols "on"

	filter "configurations:Release"
		defines "ME_RELEASE"
		buildoptions "/MT"
		optimize "on"

	filter "configurations:Dist"
		defines "ME_DIST"
		buildoptions "/MT"
		optimize "on"


project "Runtime"
	location "Runtime"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"MarsEngine/vendor/spdlog/include",
		"MarsEngine/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"MarsEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ME_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "ME_DEBUG"
		buildoptions "/MTd"
		symbols "on"

	filter "configurations:Release"
		defines "ME_RELEASE"
		buildoptions "/MT"
		optimize "on"

	filter "configurations:Dist"
		defines "ME_DIST"
		buildoptions "/MT"
		optimize "on"


project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"MarsEngine/vendor/spdlog/include",
		"MarsEngine/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"

	}

	links
	{
		"MarsEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ME_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "ME_DEBUG"
		buildoptions "/MTd"
		symbols "on"

	filter "configurations:Release"
		defines "ME_RELEASE"
		buildoptions "/MT"
		optimize "on"

	filter "configurations:Dist"
		defines "ME_DIST"
		buildoptions "/MT"
		optimize "on"