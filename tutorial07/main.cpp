
#include <cstdio>
#include <new>
#include <assert.h>
#include "lua.hpp"

int main()
{
   std::printf("---- C++ constructors and destructors ----\n");
   static int numberOfSpritesExisting{};

   struct Sprite
   {
      int x{};
      int y{};
      Sprite() { numberOfSpritesExisting++; }
      ~Sprite() { numberOfSpritesExisting--; }

      void Move(int velX, int velY)  { x += velX, y += velY; }

      void Draw() { std::printf("sprite(%p):x = %d, y= %d\n", this, x, y); }
   };

   // create an instance of Sprite type (userdata in Lua)
   auto CreateSprite = [](lua_State* L) -> int
   {
      // lua will allocate and manage memory!
      void* pointerToASprite = lua_newuserdata(L, sizeof(Sprite));
      // we call constructor to initialize
      new (pointerToASprite) Sprite();
      luaL_getmetatable(L, "SpriteMetaTable");
      assert(lua_istable(L, -1));
      lua_setmetatable(L, -2);
      return 1;
   };

   auto DestroySprite = [](lua_State* L) -> int
   {
      Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
      sprite->~Sprite();
      return 0;
   };

   auto MoveSprite = [](lua_State* L) -> int
   {
      Sprite* sprite = (Sprite*)lua_touserdata(L, -3);
      lua_Number velX = lua_tonumber(L, -2);
      lua_Number velY = lua_tonumber(L, -1);
      sprite->Move(int(velX), int(velY));
      return 0;
   };

   auto DrawSprite = [](lua_State* L) -> int
   {
      Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
      sprite->Draw();
      return 0;
   };

   constexpr const char* LUA_FILE = R"(
   sprite = CreateSprite()
   MoveSprite(sprite, 5, 7)
   DrawSprite(sprite)
   MoveSprite(sprite, 1, 2)
   DrawSprite(sprite)
   )";
   lua_State* L = luaL_newstate();

   luaL_newmetatable(L, "SpriteMetaTable");
   lua_pushstring(L, "__gc");
   lua_pushcfunction(L, DestroySprite);
   lua_settable(L, -3);

   lua_pushcfunction(L, CreateSprite);
   lua_setglobal(L, "CreateSprite");
   lua_pushcfunction(L, MoveSprite);
   lua_setglobal(L, "MoveSprite");
   lua_pushcfunction(L, DrawSprite);
   lua_setglobal(L, "DrawSprite");

   int doResult = luaL_dostring(L, LUA_FILE);
   if (doResult != LUA_OK) {
      std::printf("Error: %s\n", lua_tostring(L, -1));
   }
   lua_close(L);

   assert(numberOfSpritesExisting==0);
   return 0;
}
