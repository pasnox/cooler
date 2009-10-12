#ifndef BOMBINFORMATIONS_H
#define BOMBINFORMATIONS_H

#include <QtGlobal>

class BombInformations
{
public:
	enum Type
	{
		Normal = 0,
	};
	
	BombInformations();
	virtual ~BombInformations();

	/*** For dynamic memory management (can be removed if it isn't necessary) ***/
	/*
	BombInformations( const BombInformations& other );
	BombInformations& operator=( const BombInformations& other );
	bool operator==( const BombInformations& other ) const;
	bool operator!=( const BombInformations& other ) const;
	*/
	/*** End of dynamic memory management ***/

protected:
	uint mCount;
	uint mPower;
	BombInformations::Type mType;
};

#endif // BOMBINFORMATIONS_H
