
#include <cstdio>

#include "lua.hpp"

int main()
{
   lua_State* L = luaL_newstate();
   luaL_dostring(L, "x = 42");
   // pushed "x" on stack
   lua_getglobal(L, "x");
   // get value at stack position 1
   // lua_Nummber is defined to be a double
   lua_Number x = lua_tonumber(L, 1);
   std::printf("lua says x = %d\n", int(x));

   lua_close(L);
   return 0;
}
