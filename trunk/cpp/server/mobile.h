//
// C++ Interface: mobile
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MOBILE_H
#define MOBILE_H

#include <vector>
#include "tile.h"
#include "numericalstat.h"

/**
    @author Cavalaria Christophe <chris.cavalaria@free.fr>
*/

class Level;

class Mobile
{
	private:
		Level* level;
		Tile* appearance;
	public:
		int posx;  // Note : readonly for outside
		int posy;

		bool dirty;

		Mobile();
		virtual ~Mobile();

		void SetLevel ( Level& newlevel, int newposx, int newposy );
		void Translate ( int decx, int decy );
		Level* GetLevel();
		void SetAppearance ( Tile* tile );
		Tile* GetAppearance();

		void TryMove ( int decx, int decy );
};

class Monster : public Mobile
{
	public:
		Monster();
		NumericalStat Hp;

	protected:
		virtual int ComputeDamage ( Monster& target );
		virtual void CheckHp();
};

class PlayerMonster : public Monster // Temporary class to handle Mobile elements with Player Stats
{
	public:
		PlayerMonster();
		NumericalStat Strength;
		NumericalStat Dexterity;
		NumericalStat Magic;
		NumericalStat Intelligence;
	protected:
		virtual int ComputeDamage ( Monster& target );
};

#endif
