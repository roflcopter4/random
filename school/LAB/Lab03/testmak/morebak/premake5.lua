workspace "MyWorkspace"
    configurations { "Debug", "Release" }

project "TicTacToe"
    kind "ConsoleApp"
    language "C"
    files { "**.c", "**.h" }
    bindirs { "." }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
