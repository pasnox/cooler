#include "PlayerTile.h"
#include "TilesManager.h"

PlayerTile::PlayerTile( const QFileInfo& fn )
	: AbstractTile( fn, Globals::PlayerTile )
{
}

int PlayerTile::steps() const
{
	return Pixmap.isNull() ? 0 : 6;
}

QPixmap PlayerTile::tile( int step ) const
{
	return tile( Globals::PadStrokeDown, step );
}

QPixmap PlayerTile::tile( Globals::PadStroke stroke, int step ) const
{
	const QSize tileSize = TilesManager::instance()->tileSize();
	const QPoint point = QPoint( step *Globals::TilesSize.width(), stroke *Globals::TilesSize.height() );
	return Pixmap.copy( QRect( point, Globals::TilesSize ) ).scaled( tileSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
}
