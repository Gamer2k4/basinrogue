//
// C++ Interface: dungeon
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DUNGEON_H
#define DUNGEON_H

#include <string>
#include <vector>

class Level;
class World;
class LevelGenerator;

/**
	@author
*/
class Dungeon
{
	private:
		World& world;
		std::string name, entry_message;
		int levelmax;

		Dungeon ( const Dungeon& level );
		const Dungeon& operator = ( const Dungeon& level );
	protected:
		virtual Level* VirtualGetLevel ( int depth ) = 0;
	public:
		Dungeon ( const std::string& name, const std::string& entry_message, World& world );
		virtual ~Dungeon();

		std::string getName() const;

		Level* GetLevel ( int depth );

		World& getWorld() const;

		std::string GetEntryMessage() const;

		void setLevelmax ( int theValue );
		int getLevelmax() const;

};

class TownLevel : public Dungeon
{
	private:
		Level& town_level;
	protected:
		virtual Level* VirtualGetLevel ( int depth );
	public:
		TownLevel ( const std::string& name, const std::string& entry_message, World& world, LevelGenerator& generator );
		virtual ~TownLevel();
};

struct ExternalConnection
{
	int depth;
	std::string dungeon_name;
};

class MultilevelDungeon : public Dungeon
{
	private:
		LevelGenerator& generator;
		std::vector<ExternalConnection> connection_list;
	protected:
		virtual Level* VirtualGetLevel ( int depth );
	public:
		MultilevelDungeon ( const std::string& name, const std::string& entry_message, World& world, LevelGenerator& generator );
		virtual ~MultilevelDungeon();

};

#endif
