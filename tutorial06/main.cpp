
#include <cstdio>
#include "lua.hpp"
#include "assert.h"

int main()
{
   std::printf("----- metatables and metamethod(s) -----\n");

   struct Vec
   {
      static int CreateVector2D(lua_State* L) {
         lua_newtable(L);
         lua_pushstring(L, "x");
         lua_pushnumber(L, 0);
         lua_settable(L, -3);          // x = 0

         lua_pushstring(L, "y");
         lua_pushnumber(L, 0);
         lua_settable(L, -3);          // y = 0

         luaL_getmetatable(L, "VectorMetaTable");
         lua_setmetatable(L, -2);
         return 1;
      }

      static int __add(lua_State* L) {
         std::printf("__add was called\n");
         assert(lua_istable(L, -2));  // left table
         assert(lua_istable(L, -1));  // right table

         lua_pushstring(L, "x");
         lua_gettable(L, -3);
         lua_Number xLeft = lua_tonumber(L, -1);
         lua_pop(L, 1);

         lua_pushstring(L, "x");
         lua_gettable(L, -2);
         lua_Number xRight = lua_tonumber(L, -1);
         lua_pop(L, 1);

         lua_Number xAdded = xLeft + xRight;
         std::printf("xAdded = %d\n", int(xAdded));

         Vec::CreateVector2D(L);
         lua_pushstring(L, "x");
         lua_pushnumber(L, xAdded);
         lua_rawset(L, -3);
         return 1;
      }
   };

   constexpr const char* LUA_FILE = R"(
   v1 = CreateVector()  -- v1 is a table
   v2 = CreateVector()  -- v2 is a table
   v1.x = 10
   v2.x = 42
   v3 = v1 + v2
   result = v3.x
   )";

   lua_State* L = luaL_newstate();

   lua_pushcfunction(L, Vec::CreateVector2D);
   lua_setglobal(L, "CreateVector");

   luaL_newmetatable(L, "VectorMetaTable");
   lua_pushstring(L, "__add");
   lua_pushcfunction(L, Vec::__add);
   lua_settable(L, -3);

   int x = luaL_dostring(L, LUA_FILE);
   if (x != LUA_OK) {
      std::printf("Error: %s\n", lua_tostring(L,-1));
   }

   lua_getglobal(L, "result");
   lua_Number result = lua_tonumber(L, -1);
   std::printf("result = %d\n", int(result));
   lua_close(L);

   return 0;
}
