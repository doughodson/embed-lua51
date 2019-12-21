
#include <cstdio>
#include "lua.hpp"
#include "assert.h"

int main()
{
   std::printf("---------- tables ----------\n");

   constexpr const char* LUA_FILE = R"(
   x = { dave="busy", ian="idle" }
   )";

   lua_State* L = luaL_newstate();
   luaL_dostring(L, LUA_FILE);

   lua_getglobal(L, "x");
   assert(lua_istable(L, -1));    // check to see if table is on stack
   lua_pushstring(L, "dave");     // push on key if interest
   lua_gettable(L, -2);           // pops key "dave" and leave value
   assert(lua_isstring(L, -1));
   const char* daveIs = lua_tostring(L, -1);
   std::printf("dave is %s\n", daveIs);

   // simplier using getfield
   lua_getglobal(L, "x");
   lua_getfield(L, -1, "ian");
   const char* ianIs = lua_tostring(L, -1);
   std::printf("ian is %s\n", ianIs);

   lua_getglobal(L, "x");
   lua_pushstring(L, "sleeping");
   lua_setfield(L, -2, "john");

   lua_getglobal(L, "x");
   lua_getfield(L, -1, "john");
   const char* johnIs = lua_tostring(L, -1);
   std::printf("john is %s\n", johnIs);

   lua_close(L);

   return 0;
}
