workspace "MyWorkspace"
    configurations { "Debug", "Release" }

project "myStrLib"
    kind "sharedlib"
    language "C"
    targetname "mystr"
    files { "myStrLib.**" }
    bindirs { "." }

    filter { "configurations:Debug" }
       defines { "DEBUG" }
       symbols "On"

    filter { "configurations:Release" }
       defines { "NDEBUG" }
       optimize "On"

project "ggets"
    kind "sharedlib"
    language "C"
    files { "ggets.**" }

    filter { "configurations:Debug" }
       defines { "DEBUG" }
       symbols "On"

    filter { "configurations:Release" }
       defines { "NDEBUG" }
       optimize "On"
