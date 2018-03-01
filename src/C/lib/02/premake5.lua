workspace "MyWorkspace"
    configurations { "Debug", "Release" }

project "myStrLib"
    kind "ConsoleApp"
    language "C"
    files { "myStrLib.**" }

    filter { "configurations:Debug" }
       defines { "DEBUG" }
       symbols "On"

    filter { "configurations:Release" }
       defines { "NDEBUG" }
       optimize "On"
