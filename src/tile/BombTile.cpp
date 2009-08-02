#include "BombTile.h"

#include <QSettings>

BombTile::BombTile()
{
}

BombTile::BombTile( const QFileInfo& fn )
{
	FileName = fn.absoluteFilePath();
	QSettings settings( FileName, QSettings::IniFormat );
	settings.beginGroup( "Bomb" );
	Name = settings.value( "Name" ).toString();
	Size = settings.value( "Size" ).toSize();
	QString tilesFn = fn.absolutePath().append( "/" ).append( settings.value( "Tiles" ).toString() );
	settings.endGroup();
	Pixmap = QPixmap( tilesFn );
}

QPixmap BombTile::tile( int step )
{
	const QPoint point = QPoint( step *Size.width(), 0 );
	return Pixmap.copy( QRect( point, Size ) );
}

int BombTile::steps() const
{
	return Pixmap.width() /Size.width();
}

bool BombTile::operator==( const BombTile& other ) const
{
	return FileName == other.FileName && Name == other.Name && Size == other.Size;
}

bool BombTile::operator!=( const BombTile& other ) const
{
	return !operator==( other );
}
