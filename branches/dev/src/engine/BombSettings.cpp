#include "BombSettings.h"

BombSettings::BombSettings()
{
	TimeOut = 3000;
	Power = 1;
}

BombSettings::BombSettings( const BombSettings& other )
{
	operator=( other );
}

BombSettings& BombSettings::operator=( const BombSettings& other )
{
	if( *this != other )
	{
		TimeOut = other.TimeOut;
		Power = other.Power;
	}

	return *this;
}

bool BombSettings::operator==( const BombSettings& other ) const
{
	return TimeOut == other.TimeOut &&
		Power == other.Power;
}

bool BombSettings::operator!=( const BombSettings& other ) const
{
	return !operator==( other );
}
