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
class Monster;

class Mobile
{
	private:
		Tile* appearance;
	public:
		int posx;  // Note : readonly for outside
		int posy;

		bool dirty;

		Mobile();
		virtual ~Mobile();

		virtual int IsMonster(){return 0;}
		virtual int IsPlayer(){return 0;}

		void SetLevel ( Level& newlevel, int newposx, int newposy );
		void Translate ( int decx, int decy );
		Level* GetLevel();
		void SetAppearance ( Tile* tile );
		Tile* GetAppearance();

		bool TryMove ( int decx, int decy );
		virtual void BumpInto ( Mobile* mob ) = 0;

		virtual float GetMovementSpeed() = 0;
		virtual void GainEnergy ( double energy_gain ) = 0;
		virtual void LoseEnergy ( double energy_loss ) = 0;
		virtual int HasEnergy() = 0;

	protected:
		Level* level;
};

class Monster : public Mobile
{
	public:
		Monster();

		NumericalStat Hp;

		virtual int IsMonster(){return 1;}
		virtual int IsPlayer(){return 0;}

		void BumpInto ( Mobile* mob );

		virtual void GainEnergy ( double energy_gain );
		virtual void LoseEnergy ( double energy_loss );
		virtual int HasEnergy();
		virtual void MonsterTurn();

	private:
		virtual float GetMovementSpeed();
		double energy;

	protected:
		virtual int ComputeDamage ( Mobile& target );
		virtual void TakeDamage ( int damage );
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

		void GainEnergy ( double energy_gain ){}; // only real
		void LoseEnergy ( double energy_loss ){}; // monsters
		int HasEnergy(){return 0;}                // have energy

		int IsMonster(){return 0;}
		int IsPlayer(){return 1;}

	private:
		float GetMovementSpeed();

	protected:
		virtual int ComputeDamage ( Mobile& target );
		virtual void CheckHp();
};

#endif
