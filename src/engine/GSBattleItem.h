#ifndef GSBATTLEITEM_H
#define GSBATTLEITEM_H

#include "GSGenericStateItem.h"

#include <Globals.h>

class GSBattleItem : public GSGenericStateItem
{
public:
	GSBattleItem( int state = 3, int pixelSize = 20 );
	
	virtual void setState( int state );
};

#endif // GSBATTLEITEM_H
