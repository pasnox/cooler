#include "PlayerTile.h"

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
	const QSize tileSize = Globals::TilesSize;
	const QPoint point = QPoint( step *tileSize.width(), stroke *tileSize.height() );
	return Pixmap.copy( QRect( point, tileSize ) );
}
