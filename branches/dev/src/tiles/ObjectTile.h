#ifndef OBJECTTILE_H
#define OBJECTTILE_H

#include "AbstractTile.h"

struct ObjectTile : AbstractTile
{
	ObjectTile( const QFileInfo& fn = QFileInfo(), Globals::TypeTile type = Globals::InvalidTile );
	
	virtual int steps() const;
	virtual QPixmap tile( int step ) const;
};

#endif // OBJECTTILE_H
