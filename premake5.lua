-- A solution
workspace "lua-opcua"
	configurations { "Debug", "Release"}

project "opcua"
	kind "SharedLib"
	language "C++"
	location "build"
	targetprefix ""
	targetdir "bin/%{cfg.buildcfg}"

	includedirs { "/usr/include/lua5.3", ".", "../../build/" }
	files { "**.hpp", "**.cpp"}

	-- buildoptions { '-Wno-unknown-warning', '-Wno-unknown-warning-option', '-Wall', '-Wextra', '-Wpedantic', '-pedantic', '-pedantic-errors', '-Wno-noexcept-type', '-std=c++14', '-ftemplate-depth=1024' }
	buildoptions { '-Wpedantic', '-pedantic', '-pedantic-errors', '-Wno-noexcept-type', '-std=c++14', '-ftemplate-depth=2048', '-DUA_ARCHITECTURE_POSIX'}

	libdirs { "../../build/bin" }
	links { "pthread", "open62541" }
	linkoptions { "-Wl,--whole-archive -lmbedtls -lmbedx509 -lmbedcrypto -Wl,--no-whole-archive" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

