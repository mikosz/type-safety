function use_googletest()
	local googletest_dir = path.join(ROOT_DIR, "external/googletest")

	local googletest_include_dir = path.join(googletest_dir, "include")
	
	includedirs(googletest_include_dir)

	local gmock_all_file = googletest_include_dir.."/src/gmock-all.cc"
	local gtest_all_file = googletest_include_dir.."/src/gtest-all.cc"
	files { gmock_all_file, gtest_all_file }
	vpaths { [ ".external" ] = { gmock_all_file, gtest_all_file } }
end
