#include "Player.h"
#include "AbstractTile.h"

Player::Player()
{
	mState = Globals::PlayerStateOff;
	mTile = 0;
}

Player::~Player()
{
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
Player::Player( const Player& other )
{
	operator=( other );
}

Player& Player::operator=( const Player& other )
{
	if ( *this != other )
	{
		mState = other.mState;
		mTile = other.mTile;
	}

	return *this;
}

bool Player::operator==( const Player& other ) const
{
	return mState == other.mState && mTile == other.mTile;
}

bool Player::operator!=( const Player& other ) const
{
	return !operator==( other );
}
/*** End of dynamic memory management ***/

void Player::setState( Globals::PlayerState state )
{
	mState = state;
}

const Globals::PlayerState& Player::state() const
{
	return mState;
}

void Player::setTile( AbstractTile* tile )
{
	mTile = tile;
}

AbstractTile* Player::tile() const
{
	return mTile;
}
