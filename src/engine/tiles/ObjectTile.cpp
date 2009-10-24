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

	switch ( Type )
	{
		case Globals::BlockTile:
		case Globals::BombTile:
		case Globals::BonusTile:
		case Globals::BoxTile:
		case Globals::FloorTile:
		case Globals::SkyTile:
			return Pixmap.scaled( tileSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
		case Globals::InvalidTile:
		case Globals::GameScreenTile:
		case Globals::BombExplosionTile:
		case Globals::PlayerTile:
		case Globals::PlayerExplosionTile:
		case Globals::TextTile:
			break;
	}
	
	return Pixmap;
}
