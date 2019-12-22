
/* from chapter 27 */

#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"


/*
Mental picture of stack

+-----------------------+
| element with index 6  | <-- top ("relative" index -1)
+-----------------------+
| element with index 5  | <-- -2
+-----------------------+
| element with index 4  | <-- -3
+-----------------------+
| element with index 3  | <-- -4
+-----------------------+
| element with index 2  | <-- -5
+-----------------------+
| element with index 1  | <-- bottom ("relative" index -6 )
+-----------------------+
*/

static void stack_dump(lua_State *L)
{
   int i;
   int top = lua_gettop(L);      /* depth of the stack                             */
   for (i = 1; i <= top; i++) {  /* repeat for each level - from first pushed, etc */
      int t = lua_type(L, i);
      switch (t) {
         case LUA_TSTRING: {     /* strings */
            printf("'%s'", lua_tostring(L, i));
            break;
         }
         case LUA_TBOOLEAN: {    /* Booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
         }
         case LUA_TNUMBER: {     /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
         }
         default: {              /* other values */
            printf("%s", lua_typename(L, t));
            break;
         }
      }
      printf(" ");              /* put a separator */
   }
   printf("\n");
}

int main (void)
{
   lua_State *L = luaL_newstate();

   lua_pushboolean(L, 1);        /* absolute index: 1 */
   lua_pushnumber(L, 10);        /* absolute index: 2 */
   lua_pushnil(L);               /* absolute index: 3 */
   lua_pushstring(L, "hello");   /* absolute index: 4 */
   stack_dump(L);                            /* will print: true 10 nil 'hello' */

   /*
      pushes a copy of the element at the given valid index onto the stack

      for this example: pushes a copy of value at index -4 (true) to top of stack

   */
   lua_pushvalue(L, -4); stack_dump(L);      /* will print: true 10 nil 'hello' true */

   /* 
      moves the top element into the given position (and pops it), without shifting any element

      for this example: true on top of stack was moved to index 3, replacing nil and then popped
   */
   lua_replace(L, 3);    stack_dump(L);      /* will print: true 10 true 'hello' */

   /*
      accepts any acceptable index, or 0, and sets the stack top to this index. If the new top is
      larger than the old one, then the new elements are filled with nil. If index is 0, then all
      stack elements are removed.

      for this example: grow the stack by 2 elements
   */
   lua_settop(L, 6);     stack_dump(L);      /* will print: true 10 true 'hello' nil nil */

   /*
      rotates the stack elements between the valid index idx and the top of the stack.

      for this example: rotates nil from top to position 3 once
   */
   lua_rotate(L, 3, 1);  stack_dump(L);      /* will print: true 10 nil true 'hello' nil */

   /*
      removes the element at the given valid index, shifting down the elements above this
      index to fill the gap.

      for this example: removes the true at index -3
   */
   lua_remove(L, -3);    stack_dump(L);      /* will print: true 10 nil 'hello' nil */

   /*
      for this example: only true remains as the top has been cut off
   */
   lua_settop(L, -5);    stack_dump(L);      /* will print: true */

   lua_close(L);
   return 0;
}
