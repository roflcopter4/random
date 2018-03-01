workspace "MyWorkspace"
    configurations { "Debug", "Release" }

project "Assignment1"
    kind "ConsoleApp"
    language "C"
    targetname "assignment1"
    files { "**.c, **.h" }
    bindirs { "." }

    filter { "configurations:Debug" }
       defines { "DEBUG" }
       symbols "On"

    filter { "configurations:Release" }
       defines { "NDEBUG" }
       optimize "On"
