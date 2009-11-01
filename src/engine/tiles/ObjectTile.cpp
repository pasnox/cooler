#include "ObjectTile.h"
#include "TilesManager.h"
#include "TilesCache.h"

ObjectTile::ObjectTile( const QFileInfo& fn, Globals::TypeTile type )
	: AbstractTile( fn, type )
{
}

int ObjectTile::steps() const
{
	return pixmap().isNull() ? 0 : 1;
}

QPixmap ObjectTile::tile( int step ) const
{
	Q_UNUSED( step );
	const QSize tileScaledSize = TilesManager::instance()->tileScaledSize();

	switch ( Type )
	{
		case Globals::BlockTile:
		case Globals::BombTile:
		case Globals::BonusTile:
		case Globals::BoxTile:
		case Globals::FloorTile:
		case Globals::SkyTile:
		{
			const uint key = qHash( FileName.fileName().append( step ).append( tileScaledSize.width() ).append( tileScaledSize.height() ) );
			QPixmap pm;
			
			if ( !TilesCache::instance()->find( key, pm ) )
			{
				pm = pixmap().scaled( tileScaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
				TilesCache::instance()->insert( key, pm );
			}
			
			return pm;
		}
		case Globals::InvalidTile:
		case Globals::GameScreenTile:
		case Globals::BombExplosionTile:
		case Globals::PlayerTile:
		case Globals::PlayerExplosionTile:
		case Globals::TextTile:
			break;
	}
	
	return pixmap();
}
