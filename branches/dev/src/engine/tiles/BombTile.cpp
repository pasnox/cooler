#include "BombTile.h"
#include "TilesManager.h"
#include "TilesCache.h"

BombTile::BombTile( const QFileInfo& fn )
	: AbstractTile( fn, Globals::BombTile )
{
}

int BombTile::steps() const
{
	return pixmap().isNull() ? 0 : 3;
}

QPixmap BombTile::tile( int step ) const
{
	const QSize tileScaledSize = TilesManager::instance()->tileScaledSize();
	const uint key = qHash( relativeFilePath().append( step ).append( tileScaledSize.width() ).append( tileScaledSize.height() ) );
	QPixmap pm;
	
	if ( !TilesCache::instance()->find( key, pm ) )
	{
		const QPoint point = QPoint( step *Globals::TilesSize.width(), 0 );
		pm = pixmap().copy( QRect( point, Globals::TilesSize ) ).scaled( tileScaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
		TilesCache::instance()->insert( key, pm );
	}
	
	return pm;
}
