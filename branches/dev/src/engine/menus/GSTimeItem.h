#ifndef GSTIMEITEM_H
#define GSTIMEITEM_H

#include "GSGenericStateItem.h"
#include "Globals.h"

class GSTimeItem : public GSGenericStateItem
{
public:
	GSTimeItem( Globals::TimeState state = Globals::TimeState3, int pixelSize = 20 );
	
	void setTimeState( Globals::TimeState state );
	Globals::TimeState timeState() const;

protected:
	virtual void setState( int state );
};

#endif // GSTIMEITEM_H
