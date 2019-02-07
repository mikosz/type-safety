function googlebenchmark_include_dir()
	local benchmark_dir = path.join(_MAIN_SCRIPT_DIR, "external/benchmark")
	return path.join(benchmark_dir, "include")
end

function add_googlebenchmark_libdir()
	local benchmark_dir = path.join(_MAIN_SCRIPT_DIR, "external/benchmark")
	
	filter { "action:vs*", "configurations:Debug*" }
		libdirs(path.join(benchmark_dir, "vs/Debug/lib"))
	filter { "action:vs*", "configurations:Release*" }
		libdirs(path.join(benchmark_dir, "vs/Release/lib"))
	filter {}
end

function link_googlebenchmark_lib()
	links "benchmark"
	filter "platforms:Win*"
		links "Shlwapi"
	filter {}
end

function use_googlebenchmark()
	includedirs(googlebenchmark_include_dir())
	add_googlebenchmark_libdir()
	link_googlebenchmark_lib()
end