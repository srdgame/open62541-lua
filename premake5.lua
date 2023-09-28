-- A solution
workspace "lua-opcua"
	configurations { "Debug", "Release"}

project "opcua"
	kind "SharedLib"
	language "C++"
	location "build"
	targetprefix ""
	targetdir "bin/%{cfg.buildcfg}"

	--includedirs { "/usr/include/lua5.3", ".", "../../build/" }
	includedirs { "/home/cch/mycode/skynet/3rd/lua/", ".", "../..", "../../build/" }
	files { "./src/**.hpp", "./src/**.cpp"}

	-- buildoptions { '-Wno-unknown-warning', '-Wno-unknown-warning-option', '-Wall', '-Wextra', '-Wpedantic', '-pedantic', '-pedantic-errors', '-Wno-noexcept-type', '-std=c++14', '-ftemplate-depth=1024' }
	buildoptions { '-Wpedantic', '-pedantic', '-pedantic-errors', '-Wno-noexcept-type', '-std=c++14', '-ftemplate-depth=2048'}

	-- defines { 'UA_ARCHITECTURE_POSIX', 'UA_ENABLE_ENCRYPTION' }

	libdirs { "../../build/bin" }
	links { "pthread", "open62541", "ssl", "crypto", "uuid" }
	--linkoptions { "-Wl,--whole-archive -lmbedtls -lmbedx509 -lmbedcrypto -Wl,--no-whole-archive" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

