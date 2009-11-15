#ifndef BOMBTILE_H
#define BOMBTILE_H

#include "AbstractTile.h"

struct BombTile : AbstractTile
{
	BombTile( const QFileInfo& fn = QFileInfo() );
	
	virtual int steps() const;
	virtual QPixmap tile( int step ) const;
};

#endif // BOMBTILE_H
