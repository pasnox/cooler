#include "ObjectTile.h"
#include "TilesManager.h"

ObjectTile::ObjectTile( const QFileInfo& fn, Globals::TypeTile type )
	: AbstractTile( fn, type )
{
}

int ObjectTile::steps() const
{
	return Pixmap.isNull() ? 0 : 1;
}

QPixmap ObjectTile::tile( int step ) const
{
	Q_UNUSED( step );
	const QSize tileSize = TilesManager::instance()->tileSize();
	return Pixmap; //.scaled( tileSize );
}
