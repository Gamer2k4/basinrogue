//
// C++ Implementation: numericalstat
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#pragma warning(disable:4786)

#include "numericalstat.h"

const int maximum_maximum = 100000;

static int clamp_maximum ( int maximum )
{
	if ( maximum > maximum_maximum )
		return maximum_maximum;
	if ( maximum < 0 )
		return 0;
	return maximum;
}

NumericalStat::NumericalStat ( int maximum ) :
		value ( clamp_maximum ( maximum ) ),
		maximum ( clamp_maximum ( maximum ) )
{}

NumericalStat::NumericalStat ( int current, int maximum ) :
		value ( current ),
		maximum ( clamp_maximum ( maximum ) )
{
	if ( value > maximum )
		value = maximum;
}


NumericalStat::~NumericalStat()
{}


int NumericalStat::getValue() const
{
	return value;
}

void NumericalStat::setValue ( int theValue )
{
	value = theValue;
}

int NumericalStat::getMaximum() const
{
	return maximum;
}

void NumericalStat::setMaximum ( int theValue )
{
	if ( theValue > maximum_maximum )
		maximum = maximum_maximum;
	else
		maximum = theValue;
}

void NumericalStat::setToMax()
{
	value = maximum;
}

NumericalStat::operator int() const
{
	return value;
}

NumericalStat& NumericalStat::operator = ( int theValue )
{
	value = theValue;
	return *this;
}


/*!
    \fn NumericalStat::operator +=(int theValue)
 */
NumericalStat& NumericalStat::operator += ( int theValue )
{
	int newValue = value + theValue;
	if ( newValue > maximum )
		newValue = maximum;
	if ( newValue < 0 )
		newValue = 0;
	setValue ( newValue );

	return *this;
}


/*!
    \fn NumericalStat::operator -=(int theValue)
 */
NumericalStat& NumericalStat::operator -= ( int theValue )
{
	int newValue = value - theValue;
	if ( newValue > maximum )
		newValue = maximum;
	if ( newValue < 0 )
		newValue = 0;
	setValue ( newValue );

	return *this;
}
