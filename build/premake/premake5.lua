
--
-- If premake command is not supplied an action (target compiler), exit!
--
-- Targets of interest:
--     vs2019     (Visual Studio 2019)
--     gmake      (Linux make)
--
if (_ACTION == nil) then
    return
end

Lua51_Root     = "../../lua-5.1.5/"
Lua51_SrcPath  = Lua51_Root .. "src/"
Lua51_LibPath  = Lua51_Root .. "lib/"

Lua53_Root     = "../../lua-5.3.5/"
Lua53_SrcPath  = Lua53_Root .. "src/"
Lua53_LibPath  = Lua53_Root .. "lib/"

workspace "embed-lua"

   -- destination directory for generated solution/project files
   location ("../" .. _ACTION)

   -- don't automatically prefix the name of generated targets
   targetprefix ""

   -- compile for 64 bits (no 32 bits for now)
   architecture "x86_64"

   --
   -- Build (solution) configuration options:
   --     Release        (Runtime library is Multi-threaded DLL)
   --     Debug          (Runtime library is Multi-threaded Debug DLL)
   --
   configurations { "Release", "Debug" }

   -- common release configuration flags and symbols
   filter "configurations:Release"
      optimize "On"

   filter "system:windows"
         -- favor speed over size
         buildoptions { "/Ot" }
         defines { "WIN32", "_LIB", "NDEBUG" }

   -- common debug configuration flags and symbols
   filter "configurations:Debug"
      symbols "On"

   filter "system:windows"
         defines { "WIN32", "_LIB", "_DEBUG" }

   --
   -- stock lua library, interpreter and compiler
   --

   -- lua 5.1 library (compiled as a shared library)
   project "lua51"
      targetname "lua"
      kind "SharedLib"
      language "C"
      targetdir ( Lua51_LibPath )
      includedirs { Lua51_SrcPath }
      files {
         Lua51_SrcPath .. "**.*"    -- include all source files
      }
      excludes {
         Lua51_SrcPath .. "lua.c",  -- but not the repl
         Lua51_SrcPath .. "lua.h",  -- or it's associated header file
         Lua51_SrcPath .. "luac.c"  -- or the compiler
      }
      if os.ishost("windows") then
         defines { "LUA_BUILD_AS_DLL" }
      end
      if os.ishost("linux") then
         defines { "LUA_USE_LINUX" }
      end

   -- lua 5.3 library (compiled as a shared library)
   project "lua53"
      targetname "lua"
      kind "SharedLib"
      language "C"
      targetdir ( Lua53_LibPath )
      includedirs { Lua53_SrcPath }
      files {
         Lua53_SrcPath .. "**.*"    -- include all source files
      }
      excludes {
         Lua53_SrcPath .. "lua.c",  -- but not the repl
         Lua53_SrcPath .. "lua.h",  -- or it's associated header file
         Lua53_SrcPath .. "luac.c"  -- or the compiler
      }
      if os.ishost("windows") then
         defines { "LUA_BUILD_AS_DLL" }
      end
      if os.ishost("linux") then
         defines { "LUA_USE_LINUX" }
      end

   -- lua 5.1 interpreter (repl), uses shared library / dll
   project "repl51"
      targetname "lua"
      targetdir ("../../repl51")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua_SrcPath }
      libdirs     { Lua_LibPath }
      files {
         Lua51_SrcPath .. "lua.c"
      }
      links {"lua51"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   -- lua 5.3 interpreter (repl), uses shared library / dll
   project "repl53"
      targetname "lua"
      targetdir ("../../repl53")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua_SrcPath }
      libdirs     { Lua_LibPath }
      files {
         Lua53_SrcPath .. "lua.c"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   -- lua 5.1 compiler, standalone, no DLL required
   project "luac51"
      targetname "luac"
      targetdir ("../../luac51")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua_SrcPath }
      files {
         Lua51_SrcPath .. "**.*"
      }
      excludes {
         Lua51_SrcPath .. "lua.c"
      }
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   -- lua 5.3 compiler, standalone, no DLL required
   project "luac53"
      targetname "luac"
      targetdir ("../../luac53")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua_SrcPath }
      files {
         Lua53_SrcPath .. "**.*"
      }
      excludes {
         Lua53_SrcPath .. "lua.c"
      }
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   --
   -- Related examples
   --

   -- module, a C-based Lua library
   project "module53"
      targetname "module"
      targetdir ("../../module53")
      kind "SharedLib"
      language "C"
      includedirs { Lua53_SrcPath }
      includedirs { Lua53_SrcPath }
      files {
         "../../module53/**.c",
         "../../module53/test.lua"
      }
      links {"lua53"}
      if os.ishost("windows") then
         defines { "LUA_BUILD_AS_DLL" }
      end

   -- bare lua/repl interpreter
   project "repl53-bare"
      targetname "lua"
      targetdir ("../../repl53-bare")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua53_SrcPath }
      files {
         "../../repl53-bare/main.c"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   -- stack-dump (demostrates stack interface)"
   project "stack-dump"
      targetname "stack-dump"
      targetdir ("../../stack-dump")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua53_SrcPath }
      files {
         "../../stack-dump/main.c"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   -- read-config (demostrates reading a config file)"
   project "read-config"
      targetname "read-config"
      targetdir ("../../read-config")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua53_SrcPath }
      files {
         "../../read-config/main.c",
         "../../read-config/config.lua"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   --
   -- examples from YouTube Lua tutorial
   --

   project "tutorial01"
      targetname "tutorial"
      targetdir ("../../tutorial01/")
      kind "ConsoleApp"
      language "C++"
      cppdialect "C++11"
      includedirs { Lua53_SrcPath }
      libdirs     { Lua53_LibPath }
      files {
         "../../tutorial01/**.cpp"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   project "tutorial02"
      targetname "tutorial"
      targetdir ("../../tutorial02/")
      kind "ConsoleApp"
      language "C++"
      cppdialect "C++11"
      includedirs { Lua53_SrcPath }
      libdirs     { Lua53_LibPath }
      files {
         "../../tutorial02/**.cpp"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   project "tutorial03"
      targetname "tutorial"
      targetdir ("../../tutorial03/")
      kind "ConsoleApp"
      language "C++"
      cppdialect "C++11"
      includedirs { Lua53_SrcPath }
      libdirs     { Lua53_LibPath }
      files {
         "../../tutorial03/**.cpp"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   project "tutorial04"
      targetname "tutorial"
      targetdir ("../../tutorial04/")
      kind "ConsoleApp"
      language "C++"
      cppdialect "C++11"
      includedirs { Lua53_SrcPath }
      libdirs     { Lua53_LibPath }
      files {
         "../../tutorial04/**.cpp"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   project "tutorial05"
      targetname "tutorial"
      targetdir ("../../tutorial05/")
      kind "ConsoleApp"
      language "C++"
      cppdialect "C++11"
      includedirs { Lua53_SrcPath }
      libdirs     { Lua53_LibPath }
      files {
         "../../tutorial05/**.cpp"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   project "tutorial06"
      targetname "tutorial"
      targetdir ("../../tutorial06/")
      kind "ConsoleApp"
      language "C++"
      cppdialect "C++11"
      includedirs { Lua53_SrcPath }
      libdirs     { Lua53_LibPath }
      files {
         "../../tutorial06/**.cpp"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   project "tutorial07"
      targetname "tutorial"
      targetdir ("../../tutorial07/")
      kind "ConsoleApp"
      language "C++"
      cppdialect "C++11"
      includedirs { Lua53_SrcPath }
      libdirs     { Lua53_LibPath }
      files {
         "../../tutorial07/**.cpp"
      }
      links {"lua53"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end


