workspace "ResourceManager"
   configurations { "Debug", "Release" }
   cppdialect "C++20"
   architecture ("x86_64")
   symbols "On"

project "ResourceManager"
   kind "ConsoleApp"
   language "C++"
   location "ResourceManager"
   targetdir "bin/%{cfg.buildcfg}"

   files { "ResourceManager/source/**.h", "ResourceManager/source/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "off"

   filter "configurations:Release"
      defines { "RELEASE" }
      optimize "On"
      prebuildcommands
      {
        "$(ProjectDir)BakeCompiler.exe bakedData source\\ResourceManager\\sBakedData.h"
      }

project "BakeCompiler"
   kind "ConsoleApp"
   language "C++"
   location "BakeCompiler"
   targetdir "ResourceManager"
   debugdir "$(SolutionDir)ResourceManager\\"
   debugargs 
   { 
      "bakedData",
      "source\\ResourceManager\\sBakedData.h"
   }

   files { "BakeCompiler/source/**.h", "BakeCompiler/source/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      optimize "off"

   filter "configurations:Release"
      defines { "RELEASE" }
      optimize "On"