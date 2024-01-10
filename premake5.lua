local BUILD_DIR = "build"

local BGFX_DIR    = "bgfx"
local BIMG_DIR    = "bimg"
local BX_DIR      = "bx"
local GLFW_DIR    = "glfw"

local OSTGINE_DIR = "ostgine"

language "C++"
cppdialect "C++17"
buildoptions{"/Zc:__cplusplus"}

filter "configurations:Debug"
    staticruntime "on"
	runtime "Debug"
    symbols "On"
	optimize "Off"
    defines {"_DEBUG"}

filter "configurations:Release"
    staticruntime "on"
	runtime "Release"
    symbols "Off"
    optimize "On"
    defines {"OSTGINE_RELEASE"}

filter "configurations:Final"
    staticruntime "on"
	runtime "Release"
    symbols "Off"
    optimize "Full"
    defines {"OSTGINE_FINAL"}

solution "OstGine"
	startproject "OstGine"
	configurations { "Release", "Debug" }
	targetdir "bin"
	
	if os.is64bit() and not os.istarget("windows") then
		platforms "x86_64"
	else
		platforms { "x86", "x86_64" }
	end

	filter "configurations:Release"
		defines
		{
			"NDEBUG",
			"BX_CONFIG_DEBUG=0"
			}
		optimize "Full"
	
	filter "configurations:Debug*"
		defines
		{
			"_DEBUG",
			"BX_CONFIG_DEBUG=1"
		}
		optimize "Debug"
		symbols "On"

function setBxCompat()
	filter "action:vs*"
		includedirs { path.join(BX_DIR, "include/compat/msvc") }
	filter { "system:windows", "action:gmake" }
		includedirs { path.join(BX_DIR, "include/compat/mingw") }
	filter { "system:macosx" }
		includedirs { path.join(BX_DIR, "include/compat/osx") }
		buildoptions { "-x objective-c++" }
end

project "OstGine"
	kind "ConsoleApp"
	language "C++"
	location ( OSTGINE_DIR )
	rtti "Off"

   	files 
   	{ 
		path.join( OSTGINE_DIR, "source/**.h" ), 
		path.join( OSTGINE_DIR, "source/**.cpp" )
   	}

	includedirs
	{
		path.join( BGFX_DIR, "include" ),
		path.join( BX_DIR,   "include" ),
		path.join( GLFW_DIR, "include" ),
		OSTGINE_DIR
	}

	links { "bgfx", "bimg", "bx", "glfw" }

   	filter "configurations:Release"
      prebuildcommands {
         "$(ProjectDir)BakeCompiler.exe bakedData source\\ResourceManager\\sBakedData.h"
      }

	filter "system:windows"
		links { "gdi32", "kernel32", "psapi" }
	filter "system:linux"
		links { "dl", "GL", "pthread", "X11" }
	filter "system:macosx"
		links { "QuartzCore.framework", "Metal.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }
	setBxCompat()

project "bgfx"
	kind "StaticLib"
	language "C++"
	exceptionhandling "Off"
	rtti "Off"
	defines "__STDC_FORMAT_MACROS"
	location "bgfx"
	files
	{
		path.join(BGFX_DIR, "include/bgfx/**.h"),
		path.join(BGFX_DIR, "src/*.cpp"),
		path.join(BGFX_DIR, "src/*.h"),
	}
	excludes
	{
		path.join(BGFX_DIR, "src/amalgamated.cpp"),
	}
	includedirs
	{
		path.join(BGFX_DIR, "3rdparty"),
		path.join(BIMG_DIR, "include"),
		path.join(BX_DIR, "include"),
		path.join(BGFX_DIR, "3rdparty/directx-headers/include/directx"),
		path.join(BGFX_DIR, "3rdparty/khronos"),
		path.join(BGFX_DIR, "include")
	}
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
		excludes
		{
			path.join(BGFX_DIR, "src/glcontext_glx.cpp"),
			path.join(BGFX_DIR, "src/glcontext_egl.cpp")
		}
	filter "system:macosx"
		files
		{
			path.join(BGFX_DIR, "src/*.mm"),
		}
	setBxCompat()

project "bimg"
	kind "StaticLib"
	language "C++"
	exceptionhandling "Off"
	rtti "Off"
	location "bimg"
	files
	{
		path.join(BIMG_DIR, "include/bimg/*.h"),
		path.join(BIMG_DIR, "src/image.cpp"),
		path.join(BIMG_DIR, "src/image_gnf.cpp"),
		path.join(BIMG_DIR, "src/*.h"),
		path.join(BIMG_DIR, "3rdparty/astc-encoder/source/*.cpp")
	}
	includedirs
	{
		path.join(BIMG_DIR, "include"),
		path.join(BIMG_DIR, "3rdparty/astc-encoder/include"),
		path.join(BIMG_DIR, "3rdparty/astc-encoder/source"),
		path.join(BIMG_DIR, "3rdparty/tinyexr/deps/miniz"),
		path.join(BX_DIR, "include")
	}
	setBxCompat()

project "bx"
	kind "StaticLib"
	language "C++"
	exceptionhandling "Off"
	rtti "Off"
	defines "__STDC_FORMAT_MACROS"
	location "bx"
	files
	{
		path.join(BX_DIR, "include/bx/*.h"),
		path.join(BX_DIR, "include/bx/inline/*.inl"),
		path.join(BX_DIR, "src/*.cpp")
	}
	excludes
	{
		path.join(BX_DIR, "src/amalgamated.cpp"),
		path.join(BX_DIR, "src/crtnone.cpp")
	}
	includedirs
	{
		path.join(BX_DIR, "3rdparty"),
		path.join(BX_DIR, "include")
	}

	filter "configurations:Release"
		defines "BX_CONFIG_DEBUG=0"
	filter "configurations:Debug"
		defines "BX_CONFIG_DEBUG=1"
	filter "action:vs*"
			defines "_CRT_SECURE_NO_WARNINGS"
	setBxCompat()


project "glfw"
	kind "StaticLib"
	language "C"
	location "glfw"
	files
	{
		path.join(GLFW_DIR, "include/GLFW/*.h"),
		path.join(GLFW_DIR, "src/context.c"),
		path.join(GLFW_DIR, "src/egl_context.*"),
		path.join(GLFW_DIR, "src/init.c"),
		path.join(GLFW_DIR, "src/input.c"),
		path.join(GLFW_DIR, "src/internal.h"),
		path.join(GLFW_DIR, "src/monitor.c"),
		path.join(GLFW_DIR, "src/null*.*"),
		path.join(GLFW_DIR, "src/osmesa_context.*"),
		path.join(GLFW_DIR, "src/platform.c"),
		path.join(GLFW_DIR, "src/vulkan.c"),
		path.join(GLFW_DIR, "src/window.c"),
	}
	includedirs 
	{ 
		path.join(GLFW_DIR, "include") 
	}

	filter "system:windows"
		defines "_GLFW_WIN32"
		files
		{
			path.join(GLFW_DIR, "src/win32_*.*"),
			path.join(GLFW_DIR, "src/wgl_context.*")
		}
	filter "system:linux"
		defines "_GLFW_X11"
		files
		{
			path.join(GLFW_DIR, "src/glx_context.*"),
			path.join(GLFW_DIR, "src/linux*.*"),
			path.join(GLFW_DIR, "src/posix*.*"),
			path.join(GLFW_DIR, "src/x11*.*"),
			path.join(GLFW_DIR, "src/xkb*.*")
		}
	filter "system:macosx"
		defines "_GLFW_COCOA"
		files
		{
			path.join(GLFW_DIR, "src/cocoa_*.*"),
			path.join(GLFW_DIR, "src/posix_thread.h"),
			path.join(GLFW_DIR, "src/nsgl_context.h"),
			path.join(GLFW_DIR, "src/egl_context.h"),
			path.join(GLFW_DIR, "src/osmesa_context.h"),

			path.join(GLFW_DIR, "src/posix_thread.c"),
			path.join(GLFW_DIR, "src/nsgl_context.m"),
			path.join(GLFW_DIR, "src/egl_context.c"),
			path.join(GLFW_DIR, "src/nsgl_context.m"),
			path.join(GLFW_DIR, "src/osmesa_context.c"),                       
		}

	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"

project "BakeCompiler"
   kind "ConsoleApp"
   language "C++"
   location "BakeCompiler"
   targetdir( OSTGINE_DIR )
   debugdir( OSTGINE_DIR .. "\\" )
   debugargs { 
      "bakedData",
      "source\\ResourceManager\\sBakedData.h"
   }

   files { "BakeCompiler/source/**.h", "BakeCompiler/source/**.cpp" }
   
   filter "configurations:Debug"
   defines { 
         "DEBUG",
      }
      optimize "off"

      filter "configurations:Release"
      defines { 
         "RELEASE",
      }
      optimize "On"