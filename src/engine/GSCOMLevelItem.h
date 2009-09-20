#ifndef GSCOMLEVELITEM_H
#define GSCOMLEVELITEM_H

#include "GSGenericStateItem.h"

#include <Globals.h>

class GSCOMLevelItem : public GSGenericStateItem
{
public:
	GSCOMLevelItem( Globals::COMLevel level = Globals::MediumLevel, int pixelSize = 20 );
	
	void setLevel( Globals::COMLevel level );
	Globals::COMLevel level() const;

protected:
	virtual void setState( int state );
};

#endif // GSCOMLEVELITEM_H
