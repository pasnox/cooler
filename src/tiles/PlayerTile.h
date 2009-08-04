#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include <AbstractTile.h>

struct PlayerTile : AbstractTile
{
	PlayerTile( const QFileInfo& fn = QFileInfo(), Globals::TypeTile type = Globals::InvalidTile );
	
	virtual int steps() const;
	virtual QPixmap tile( int step ) const;
	
	QPixmap tile( Globals::PlayerStroke stroke, int step ) const;
	
	QString Name;
	QSize Size;
	QPixmap DeadPixmap;
};

#endif // PLAYERTILE_H
