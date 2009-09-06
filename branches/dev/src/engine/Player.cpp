#include "Player.h"

Player::Player()
{
	mState = Globals::PlayerStateOff;
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
	}

	return *this;
}

bool Player::operator==( const Player& other ) const
{
	return mState == other.mState;
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
