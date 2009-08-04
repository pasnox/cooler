#include "ObjectTile.h"

ObjectTile::ObjectTile( const QFileInfo& fn, Globals::TypeTile type )
	: AbstractTile( fn, type )
{
	Pixmap = QPixmap( FileName.absoluteFilePath() );
}

int ObjectTile::steps() const
{
	return Pixmap.isNull() ? 0 : 1;
}

QPixmap ObjectTile::tile( int step ) const
{
	Q_UNUSED( step );
	return Pixmap;
}