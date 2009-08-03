#ifndef BOMBTILE_H
#define BOMBTILE_H

#include "AbstractTile.h"

struct BombTile : AbstractTile
{
	BombTile( const QFileInfo& fn = QFileInfo(), Globals::TypeTile type = Globals::InvalidTile );
	
	virtual int steps() const;
	virtual QPixmap tile( int step ) const;
	
	QString Name;
	QSize Size;
};

#endif // BOMBTILE_H
