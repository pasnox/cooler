#ifndef GSONOFFITEM_H
#define GSONOFFITEM_H

#include "GSGenericStateItem.h"
#include "Globals.h"

class GSOnOffItem : public GSGenericStateItem
{
public:
	GSOnOffItem( Globals::InterruptState state = Globals::InterruptStateOff, int pixelSize = 20 );
	
	void setInterruptState( Globals::InterruptState state );
	Globals::InterruptState interruptState() const;

protected:
	virtual void setState( int state );
};

#endif // GSONOFFITEM_H
