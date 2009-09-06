#ifndef GSSTATEITEM_H
#define GSSTATEITEM_H

#include "GSMenuItem.h"

#include <Globals.h>

class GSStateItem : public GSMenuItem
{
public:
	GSStateItem( Globals::PlayerState state = Globals::PlayerStateOff, int pixelSize = 200 );
	
	virtual void setPixelSize( int pixelSize = 20 );
	
	void setState( Globals::PlayerState state );
	const Globals::PlayerState& state() const;
	
	void previousState();
	void nextState();

protected:
	Globals::PlayerState mState;
};

#endif // GSSTATEITEM_H
