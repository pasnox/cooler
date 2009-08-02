#include "BombInformations.h"

BombInformations::BombInformations()
{
	mCount = 1;
	mPower = 1;
	mType = BombInformations::Normal;
}

BombInformations::~BombInformations()
{
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
/*
BombInformations::BombInformations( const BombInformations& other )
{
	// copy datas there
}

BombInformations& BombInformations::operator=( const BombInformations& other )
{
	// In order to support the operation "Obj = Obj;"
	if( *this != other )
	{
		// Copy datas there
	}

	return *this;
}

bool BombInformations::operator==( const BombInformations& other ) const
{
	// Comparaison of datas here
	return false;	// Change it
}

bool BombInformations::operator!=( const BombInformations& other ) const
{
	return !operator==( other );
}
*/
/*** End of dynamic memory management ***/
