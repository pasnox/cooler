#include "PlayerTile.h"

#include <QSettings>

PlayerTile::PlayerTile( const QFileInfo& fn, Globals::TypeTile type )
	: AbstractTile( fn, type )
{
	QSettings settings( FileName.absoluteFilePath(), QSettings::IniFormat );
	settings.beginGroup( "Player" );
	Name = settings.value( "Name" ).toString();
	Size = settings.value( "Size" ).toSize();
	QString tilesFn = fn.absolutePath().append( "/" ).append( settings.value( "Tiles" ).toString() );
	QString deadTilesFn = fn.absolutePath().append( "/" ).append( settings.value( "DeadTiles" ).toString() );
	settings.endGroup();
	Pixmap = QPixmap( tilesFn );
	DeadPixmap = QPixmap( deadTilesFn );
}

int PlayerTile::steps() const
{
	return Pixmap.width() /Size.width();
}

QPixmap PlayerTile::tile( int step ) const
{
	return tile( Globals::PadStrokeDown, step );
}

QPixmap PlayerTile::tile( Globals::PadStroke stroke, int step ) const
{
	const QPoint point = QPoint( step *Size.width(), stroke *Size.height() );
	return Pixmap.copy( QRect( point, Size ) );
}
