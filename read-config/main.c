
/* from chapter 28 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

void error(lua_State *L, const char *fmt, ...)
{
   va_list argp;
   va_start(argp, fmt);
   vfprintf(stderr, fmt, argp);
   va_end(argp);
   lua_close(L);
   exit(EXIT_FAILURE);
}

int getglobint(lua_State *L, const char *var)
{
   int isnum, result;
   lua_getglobal(L, var);
   result = (int)lua_tointegerx(L, -1, &isnum);
   if (!isnum)
      error(L, "'%s' should be a number\n", var);
   lua_pop(L, 1); /* remove result from the stack */
   return result;
}

void load(lua_State *L, const char *fname, int *w, int *h)
{
   if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
      error(L, "cannot run config. file: %s", lua_tostring(L, -1));
   *w = getglobint(L, "width");
   *h = getglobint(L, "height");
}

int main(void)
{
   char buff[256];
   int error;
   lua_State *L = luaL_newstate(); /* opens Lua */
   luaL_openlibs(L); /* opens the standard libraries */
   while (fgets(buff, sizeof(buff), stdin) != NULL) {
      error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);
      if (error) {
         fprintf(stderr, "%s\n", lua_tostring(L, -1));
         lua_pop(L, 1); /* pop error message from the stack */
      }
   }
   lua_close(L);
   return 0;
}
