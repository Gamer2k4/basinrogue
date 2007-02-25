//
// C++ Interface: luabinding
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef INCLUDE_LUABINDING_H
#define INCLUDE_LUABINDING_H

#include "lua.hpp"
#include "tile.h"
#include "world.h"

#define lua_userdata_cast(L, pos, T) reinterpret_cast<T*>(luaL_checkudata((L), (pos), #T))

class LuaInterpreter
{
	public:
		lua_State* L;
		World& world;
		TileLib& tile_lib;

		LuaInterpreter(World& world, TileLib& tile_lib);
		~LuaInterpreter();

		void RegisterEngine();

		// Shall not copy
		LuaInterpreter(const LuaInterpreter&);
		const LuaInterpreter& operator =(const LuaInterpreter&);

		void RunFile(const char* filename);

		// Tile Bindings
		int NewTile(lua_State *L);

		// World bindings
		int SetStartingDungeon(lua_State *L);

		// Dungeon bindings
		int MakeLevel(lua_State *L);
		int TownLevel(lua_State *L);
};

#endif // INCLUDE_LUABINDING_H
