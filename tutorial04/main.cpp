
#include <cstdio>

#include "lua.hpp"

int main()
{
   // define a new Sprite type
   struct Sprite
   {
      int x;
      int y;

      void Move(int velX, int velY)  { x += velX, y += velY; }
   };

   // create an instance of Sprite type (userdata in Lua)
   auto CreateSprite = [](lua_State* L) -> int
   {
      // lua will manage memory!
      Sprite* sprite = (Sprite*)lua_newuserdata(L, sizeof(Sprite));
      sprite->x = 0;
      sprite->y = 0;
      return 1;
   };

   auto MoveSprite = [](lua_State* L) -> int
   {
      Sprite* sprite = (Sprite*)lua_touserdata(L, -3);
      lua_Number velX = lua_tonumber(L, -2);
      lua_Number velY = lua_tonumber(L, -1);
      sprite->Move(int(velX), int(velY));
      return 0;
   };

   constexpr const char* LUA_FILE = R"(
   sprite = CreateSprite()
   MoveSprite(sprite, 5, 7)
   )";
   lua_State* L = luaL_newstate();
   lua_pushcfunction(L, CreateSprite);
   lua_setglobal(L, "CreateSprite");
   lua_pushcfunction(L, MoveSprite);
   lua_setglobal(L, "MoveSprite");
   luaL_dostring(L, LUA_FILE);
   lua_getglobal(L, "sprite");
   if (lua_isuserdata(L, -1)) {
      std::printf("We got a sprite from Lua\n");
      Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
      std::printf("x = %d, y = %d\n", sprite->x, sprite->y);
   }
   else {
      std::printf("We DIDNT a sprite from Lua\n");
   }
   lua_close(L);

   return 0;
}
