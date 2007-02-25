//
// C++ Implementation: luabinding
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "luabinding.h"

#include "level.h"

LuaInterpreter::LuaInterpreter(World& world, TileLib& tile_lib) :
		world(world),
		tile_lib(tile_lib)
{
	L = luaL_newstate();
	luaL_openlibs(L);
}

LuaInterpreter::~LuaInterpreter()
{
	lua_close(L);
}

void LuaInterpreter::RunFile(const char* filename)
{
	if (luaL_dofile(L, filename))
	{
		std::cout << "There have been errors\n";
		//db_errorfb(L);
		//const char* traceback = luaL_checkstring(L, 1);
		//std::cout << traceback;
	}
	//lua
}

#define LUA_CLOSURE(name) int lua_##name(lua_State *L) \
{\
	LuaInterpreter* li = static_cast<LuaInterpreter*>(lua_touserdata(L, lua_upvalueindex(1)));\
	return li->name(L);\
}

#define LUA_REGISTER(name) {\
	lua_pushstring(L, #name);\
	lua_pushlightuserdata(L, this);\
	lua_pushcclosure(L, lua_##name, 1);\
	lua_settable(L, LUA_GLOBALSINDEX);\
}


LUA_CLOSURE(NewTile)
int LuaInterpreter::NewTile(lua_State *L)
{
	const char* tilename = luaL_checkstring(L, 1);
	int tileposx = luaL_checkint(L, 2);
	int tileposy = luaL_checkint(L, 3);
	int flags = luaL_checkint(L, 4);
	Tile& tile = tile_lib.NewTile(tilename, tileposx, tileposy, flags);
	lua_pushlightuserdata(L, &tile);
	return 1;
}


LUA_CLOSURE(SetStartingDungeon)
int LuaInterpreter::SetStartingDungeon(lua_State *L)
{
	const char* dungeon_id = luaL_checkstring(L, 1);
	if (dungeon_id) world.SetStartingDungeon(dungeon_id);
	return 0;
}

LUA_CLOSURE(MakeLevel)
int LuaInterpreter::MakeLevel(lua_State *L)
{
	//lua_gettable(
	return 0;
}

LUA_CLOSURE(TownLevel)
int LuaInterpreter::TownLevel(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	return 0;
}

static const luaL_reg engine_funcs[] = {
//{"add", add},
{NULL, NULL}
};

void LuaInterpreter::RegisterEngine()
{
	// FLAGS
	lua_newtable(L);

	lua_pushliteral(L, "BLOCKS_MOVEMENT");
	lua_pushnumber(L, FLAG_BLOCKS_MOVEMENT); lua_settable(L, -3);

	lua_pushliteral(L, "BLOCKS_VIEW");
	lua_pushnumber(L, FLAG_BLOCKS_VIEW); lua_settable(L, -3);

	luaL_openlib(L, "FLAGS", engine_funcs, 1);


	LUA_REGISTER(NewTile);
	LUA_REGISTER(SetStartingDungeon);

	// others?

}

