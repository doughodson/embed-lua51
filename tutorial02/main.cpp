
#include <cstdio>

#include "lua.hpp"

int main()
{
   {
      lua_State* L = luaL_newstate();
      luaL_dostring(L, "x = 42");
      // push "x" (from lua) to stack
      lua_getglobal(L, "x");
      // get value at stack position 1
      // lua_Nummber is defined to be a double
      lua_Number x = lua_tonumber(L, 1);
      std::printf("lua says x = %d\n\n", int(x));
      lua_close(L);
   }

   // lua types (nil, boolean, light userdata, number, string,
   // table, function, userdata, thread
   {
      lua_State* L = luaL_newstate();
      // push some numbers on stack
      lua_pushnumber(L, 42);
      lua_pushnumber(L, 52);
      lua_pushnumber(L, 62);

      lua_Number x = lua_tonumber(L, 1);
      lua_Number y = lua_tonumber(L, 2);
      lua_Number z = lua_tonumber(L, -1);
      std::printf("lua says x = %d\n", int(x));
      std::printf("lua says y = %d\n", int(y));
      std::printf("lua says z = %d\n", int(z));

      std::printf("\n");
      // remove 2nd elements from stack
      lua_remove(L, 2);  // removed number 52
      lua_Number yy = lua_tonumber(L, -2);
      lua_Number zz = lua_tonumber(L, -1);
      std::printf("lua says y = %d\n", int(yy));
      std::printf("lua says z = %d\n", int(zz));

      lua_close(L);
   }
   return 0;
}
