#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include <QMetaType>

#include <Globals.h>

class AbstractTile;

class Player
{
public:
	Player();
	virtual ~Player();

	/*** For dynamic memory management (can be removed if it isn't necessary) ***/
	Player( const Player& other );
	Player& operator=( const Player& other );
	bool operator==( const Player& other ) const;
	bool operator!=( const Player& other ) const;
	/*** End of dynamic memory management ***/
	
	void setState( Globals::PlayerState state );
	const Globals::PlayerState& state() const;
	
	void setTile( AbstractTile* tile );
	AbstractTile* tile() const;

protected:
	Globals::PlayerState mState;
	AbstractTile* mTile;
};

typedef QList<Player> PlayerList;

Q_DECLARE_METATYPE( Player* );
Q_DECLARE_METATYPE( Player );

#endif // PLAYER_H
