#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include <AbstractTile.h>

struct PlayerTile : AbstractTile
{
	PlayerTile( const QFileInfo& fn = QFileInfo(), Globals::TypeTile type = Globals::InvalidTile );
	
	virtual int steps() const;
	virtual QPixmap tile( int step ) const;
	
	QPixmap tile( Globals::PadStroke stroke, int step ) const;
};

#endif // PLAYERTILE_H
