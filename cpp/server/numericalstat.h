//
// C++ Interface: numericalstat
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NUMERICALSTAT_H
#define NUMERICALSTAT_H

/**
	@author
*/
class NumericalStat
{
	private:
		int value;
		int maximum;
	public:
		NumericalStat(int maximum);
		NumericalStat(int current, int maximum);

		~NumericalStat();

		void setMaximum ( int theValue );
		int getMaximum() const;

		void setValue ( int theValue );
		int getValue() const;
		operator int() const;
		void setToMax();
		NumericalStat& operator =(int theValue);
		NumericalStat& operator +=(int theValue);
		NumericalStat& operator -=(int theValue);


};

#endif
