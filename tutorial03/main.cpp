
#include <cstdio>

#include "lua.hpp"

// return the number of values being left on the stack
int NativePythagoras2(lua_State* L)
{
   lua_Number a = lua_tonumber(L, -2);
   lua_Number b = lua_tonumber(L, -1);
   lua_Number csqr = (a*a) + (b*b);
   lua_pushnumber(L, csqr);
   return 1;
};

int main()
{
   // C++ calling a lua function with parameters and return values
   {
      std::printf("Calculate using Lua\n");
      constexpr const char* LUA_FILE = R"(
      Pythagoras = function(a, b)
         return (a*a) + (b*b), a, b
      end
      )";
      lua_State* L = luaL_newstate();
      luaL_dostring(L, LUA_FILE);  // parses string/file
      lua_getglobal(L, "Pythagoras");
      if (lua_isfunction(L, -1)) {
         lua_pushnumber(L, 3);   // a
         lua_pushnumber(L, 4);   // b
         constexpr int NUM_ARGS = 2;
         constexpr int NUM_RETURNS = 3;
         lua_pcall(L, NUM_ARGS, NUM_RETURNS, 0);     // calls Pythagoras
         lua_Number csqr = lua_tonumber(L, -3);
         std::printf("csqr = %d\n", int(csqr));
         lua_Number a = lua_tonumber(L, -2);
         std::printf("a = %d\n", int(a));
         lua_Number b = lua_tonumber(L, -1);
         std::printf("b = %d\n", int(b));
      }
      lua_close(L);
   }

   // Lua calling a C++ function (i.e., native) with parameters and return value
   // using std c++ function
   {
      std::printf("Calculate using c++ std function\n");
      constexpr const char* LUA_FILE = R"(
      Pythagoras = function(a, b)
         csqr = NativePythagoras(a, b)
         return csqr, a, b
      end
      )";
      lua_State* L = luaL_newstate();
      lua_pushcfunction(L, NativePythagoras2);
      lua_setglobal(L, "NativePythagoras");
      luaL_dostring(L, LUA_FILE);  // parses string/file
      lua_getglobal(L, "Pythagoras");
      if (lua_isfunction(L, -1)) {
         lua_pushnumber(L, 3);   // a
         lua_pushnumber(L, 4);   // b
         constexpr int NUM_ARGS = 2;
         constexpr int NUM_RETURNS = 3;
         lua_pcall(L, NUM_ARGS, NUM_RETURNS, 0);     // calls Pythagoras
         lua_Number csqr = lua_tonumber(L, -3);
         std::printf("csqr = %d\n", int(csqr));
         lua_Number a = lua_tonumber(L, -2);
         std::printf("a = %d\n", int(a));
         lua_Number b = lua_tonumber(L, -1);
         std::printf("b = %d\n", int(b));
      }
      lua_close(L);
   }

   // Lua calling a C++ function (i.e., native) with parameters and return value
   // using c++ lambda
   {
      std::printf("Calculate using c++ lambda\n");
      // return the number of values being left on the stack
      auto NativePythagoras = [](lua_State* L) -> int
      {
         lua_Number a = lua_tonumber(L, -2);
         lua_Number b = lua_tonumber(L, -1);
         lua_Number csqr = (a*a) + (b*b);
         lua_pushnumber(L, csqr);
         return 1;
      };

      constexpr const char* LUA_FILE = R"(
      Pythagoras = function(a, b)
         csqr = NativePythagoras(a, b)
         return csqr, a, b
      end
      )";
      lua_State* L = luaL_newstate();
      lua_pushcfunction(L, NativePythagoras);
      lua_setglobal(L, "NativePythagoras");
      luaL_dostring(L, LUA_FILE);  // parses string/file
      lua_getglobal(L, "Pythagoras");
      if (lua_isfunction(L, -1)) {
         lua_pushnumber(L, 3);   // a
         lua_pushnumber(L, 4);   // b
         constexpr int NUM_ARGS = 2;
         constexpr int NUM_RETURNS = 3;
         lua_pcall(L, NUM_ARGS, NUM_RETURNS, 0);     // calls Pythagoras
         lua_Number csqr = lua_tonumber(L, -3);
         std::printf("csqr = %d\n", int(csqr));
         lua_Number a = lua_tonumber(L, -2);
         std::printf("a = %d\n", int(a));
         lua_Number b = lua_tonumber(L, -1);
         std::printf("b = %d\n", int(b));
      }
      lua_close(L);
   }

   return 0;
}
