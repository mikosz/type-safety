local m = {}

local headers = {}
local all_projects = {}
local library_projects = {}
local test_projects = {}

-- directories

objdir("%{wks.location}/obj/%{prj.name}/%{cfg.shortname}")

local target_dir_path = function(parent)
	return "%{wks.location}/%{cfg.shortname}/"..parent.."/"
end

-- header_project and library_project

local source_patterns = function(src_dir)
	return { src_dir.."/**.hpp", src_dir.."/**.cpp" }
end

local create_source_vpaths = function(patterns, source_directory)
	for _, pattern in pairs(patterns) do
		for _, file in pairs(os.matchfiles(pattern)) do
			if not path.hasextension(path.getbasename(file), ".pch") then
				local file_with_namespace = file:sub(string.len(source_directory) + 1)
				local namespace_path = path.getdirectory(file_with_namespace)
				vpaths { [ namespace_path:gsub("/", "::") ] = file }
			end
		end
	end
end

local add_sources = function(src_dir)
	local patterns = source_patterns(src_dir)
	create_source_vpaths(patterns, src_dir..'/')
	files(patterns)
end

local create_source_project = function(name, src_dir)
	project(name)
		add_sources(src_dir)
		
		table.insert(all_projects, name)
end

local gather_headers = function(src_dir)
	for _, file in pairs(os.matchfiles(src_dir.."/**.hpp")) do
		local source = path.getabsolute(file)
		local target = file:gsub(src_dir.."/", "")
		headers[source] = target
	end
end

local create_main_project = function(name, src_dir)
	create_source_project(name, src_dir)
	project "*"
		
	includedirs(src_dir)
	gather_headers(src_dir)
end

function m.header_project(name, src_dir, common_settings)
	create_main_project(name, src_dir)

	project(name)
		kind "Utility"
		
		if common_settings then
			common_settings()
		end
	project "*"
end

function m.library_project(name, src_dir, common_settings)
	create_main_project(name, src_dir)

	project(name)
		kind "StaticLib"
		targetprefix "lib"

		filter "configurations:*Shared"
			kind "SharedLib"
			targetprefix ""
		filter {}
		
		targetdir(target_dir_path("lib"))
		
		filter "configurations:Debug*"
			targetsuffix(".d")
		filter {}
		
		if common_settings then
			common_settings()
		end
	project "*"
	
	table.insert(library_projects, name)

	create_test_projects(name, true, common_settings)
end

function m.executable_project(name, src_dir, is_windowed, common_settings)
	create_main_project(name, src_dir)

	project(name)
		if is_windowed then
			kind "WindowedApp"
		else
			kind "ConsoleApp"
		end
		
		targetdir(target_dir_path("bin"))
		
		filter "configurations:Debug*"
			targetsuffix(".d")
		filter {}
		
		if common_settings then
			common_settings()
		end
	project "*"
end

-- install target

newoption {
   trigger = "install-prefix",
   value = "path",
   description = "Directory the install action will copy files into"
}
local install_prefix = _OPTIONS["install-prefix"]

function m.create_install_project()
	project "INSTALL"
		kind "Utility"
		dependson(library_projects)
		
		if not install_prefix then
			premake.error("install-prefix not specified")
		end

		local lib_directory = path.join(install_prefix, "lib")
		
		for _, library in pairs(library_projects) do
			postbuildcommands {
				"{ECHO} installing "..library
			}
		
			filter "configurations:DebugStatic"
				postbuildcommands {
					"{COPY} %{wks.location}%{cfg.shortname}/lib/lib"..library..".d.lib "..lib_directory
				}
			filter "configurations:DebugShared"
				postbuildcommands {
					"{COPY} %{wks.location}%{cfg.shortname}/lib/"..library..".d.lib "..lib_directory,
					"{COPY} %{wks.location}%{cfg.shortname}/lib/"..library..".d.dll "..lib_directory
				}
			filter "configurations:ReleaseStatic"
				postbuildcommands {
					"{COPY} %{wks.location}%{cfg.shortname}/lib/lib"..library..".lib "..lib_directory
				}
			filter "configurations:ReleaseShared"
				postbuildcommands {
					"{COPY} %{wks.location}%{cfg.shortname}/lib/"..library..".lib "..lib_directory,
					"{COPY} %{wks.location}%{cfg.shortname}/lib/"..library..".dll "..lib_directory
				}
			filter {}
		end

		-- TODO: auto dll exports
		-- TODO: install and run_tests require rebuild to run
		-- TODO: add rmdir before installing headers
		-- TODO: mkdir only if necessary, otherwise spewing warning messages
		
		for file, target in pairs(headers) do
			local target_dir = path.getdirectory(path.join(install_prefix, "include", target))
			postbuildcommands {
				"{ECHO} installing "..file,
				"{MKDIR} "..target_dir,
				"{COPY} "..file.." "..target_dir
			}
		end
	project "*"
end

function m.create_run_tests_project()
	project "RUN_TESTS"
		kind "Utility"
		dependson(test_projects)
		
		for _, test in pairs(test_projects) do
			postbuildcommands {
				"{ECHO} Running "..test,
				"%{wks.location}%{cfg.shortname}/tests/"..test..".exe" -- ".exe" could come from a token?
			}
		end
	project "*"
end

function m.create_build_all_project()
	project "BUILD_ALL"
		kind "Utility"
		dependson(all_projects)
	project "*"
end

return m
