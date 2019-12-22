
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

   -- lua 5.1 interpreter (repl), uses shared library / dll
   project "repl"
      targetname "lua"
      targetdir ("../../repl")
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

   -- lua 5.1 compiler, standalone, no DLL required
   project "luac"
      targetname "luac"
      targetdir ("../../luac")
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

   --
   -- Related examples
   --

   -- stack-dump (demostrates stack interface)"
   project "stack-dump"
      targetname "stack-dump"
      targetdir ("../../stack-dump")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua51_SrcPath }
      files {
         "../../stack-dump/main.c"
      }
      links {"lua51"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

   -- read-config (demostrates reading a config file)"
   project "read-config"
      targetname "read-config"
      targetdir ("../../read-config")
      kind "ConsoleApp"
      language "C"
      includedirs { Lua51_SrcPath }
      files {
         "../../read-config/main.c",
         "../../read-config/config.lua"
      }
      links {"lua51"}
      if os.ishost("linux") then
         links {"dl", "m"}
      end

