workspace "MyWorkspace"
   configurations { "Debug", "Release" }

project "Assignment1_Part2"
   kind "ConsoleApp"
   language "C"
   files { "**.h", "**.c" }

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"

   filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"
