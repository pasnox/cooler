#ifndef GSSTATEITEM_H
#define GSSTATEITEM_H

#include "GSGenericStateItem.h"
#include "Globals.h"

class GSStateItem : public GSGenericStateItem
{
public:
	GSStateItem( Globals::PlayerState state = Globals::PlayerStateOff, int pixelSize = 20 );
	
	void setPlayerState( Globals::PlayerState state );
	Globals::PlayerState playerState() const;

protected:
	virtual void setState( int state );
};

#endif // GSSTATEITEM_H
