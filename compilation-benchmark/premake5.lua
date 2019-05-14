location "build"

workspace "angle"

	configurations { "Debug", "Release" }
	
	platforms { "Win64" }
	
	language "C++"
	system "Windows"
	architecture "x64"
		
	-- Configuration specific settings
	
	filter "configurations:Debug*"
		symbols "On"
		
	filter "configurations:Release*"
		defines { "NDEBUG" }
		optimize "On"
	
	-- Warnings
	
	filter "action:vs*"
		defines { "_SCL_SECURE_NO_WARNINGS" }
		buildoptions { "/std:c++latest" }
		defines { "GTEST_LANG_CXX11=1" }
	filter {}

	project "angle"
		includedirs "../src"
		kind "StaticLib"
		files "build/cobe/angle/*.cpp"
	
	project "angle-pch"
		pchheader "angle/angle.pch.h"
		pchsource "angle/angle.pch.cpp"
		includedirs { ".", "../src" }
		kind "StaticLib"
		files { "build/cobe/angle-pch/*.cpp", "angle/angle.pch.cpp" }
	
	project "angle-float"
		kind "StaticLib"
		files "build/cobe/angle-float/*.cpp"
	