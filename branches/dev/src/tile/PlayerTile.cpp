#include "PlayerTile.h"

#include <QSettings>

PlayerTile::PlayerTile()
{
}

PlayerTile::PlayerTile( const QFileInfo& fn )
{
	FileName = fn.absoluteFilePath();
	QSettings settings( FileName, QSettings::IniFormat );
	settings.beginGroup( "Player" );
	Name = settings.value( "Name" ).toString();
	Size = settings.value( "Size" ).toSize();
	QString tilesFn = fn.absolutePath().append( "/" ).append( settings.value( "Tiles" ).toString() );
	QString deadTilesFn = fn.absolutePath().append( "/" ).append( settings.value( "DeadTiles" ).toString() );
	settings.endGroup();
	Pixmap = QPixmap( tilesFn );
	DeadPixmap = QPixmap( deadTilesFn );
}

QPixmap PlayerTile::tile( Globals::PlayerStroke stroke, int step )
{
	const QPoint point = QPoint( step *Size.width(), stroke *Size.height() );
	return Pixmap.copy( QRect( point, Size ) );
}

int PlayerTile::steps() const
{
	return Pixmap.width() /Size.width();
}

bool PlayerTile::operator==( const PlayerTile& other ) const
{
	return FileName == other.FileName && Name == other.Name && Size == other.Size;
}

bool PlayerTile::operator!=( const PlayerTile& other ) const
{
	return !operator==( other );
}
