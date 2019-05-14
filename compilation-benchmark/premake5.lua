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
		files "angle/*.cpp"
	
	project "angle-pch"
		pchheader "angle.pch.h"
		pchsource "angle.pch.cpp"
		includedirs { ".", "../src" }
		kind "StaticLib"
		files { "angle-pch/*.cpp", "angle.pch.cpp" }
	
	project "angle-float"
		kind "StaticLib"
		files "angle-float/*.cpp"
	