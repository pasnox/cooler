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
	int offset = 0;
	
	switch ( stroke )
	{
		case Globals::PadStrokeRight:
			offset = 1;
			break;
		case Globals::PadStrokeLeft:
			offset = 2;
			break;
		case Globals::PadStrokeUp:
			offset = 3;
			break;
		case Globals::PadStrokeNo:
		case Globals::PadStrokeDown:
		default:
			offset = 0;
			break;
	}
	
	const QSize tileScaledSize = TilesManager::instance()->tileScaledSize();
	const QPoint point = QPoint( step *Globals::TilesSize.width(), offset *Globals::TilesSize.height() );
	return Pixmap.copy( QRect( point, Globals::TilesSize ) ).scaled( tileScaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
}
