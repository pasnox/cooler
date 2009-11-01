#include "BombTile.h"

#include <QSettings>

BombTile::BombTile( const QFileInfo& fn, Globals::TypeTile type )
	: AbstractTile( fn, type )
{
	QSettings settings( FileName.absoluteFilePath(), QSettings::IniFormat );
	settings.beginGroup( "Bomb" );
	Name = settings.value( "Name" ).toString();
	Size = settings.value( "Size" ).toSize();
	QString tilesFn = fn.absolutePath().append( "/" ).append( settings.value( "Tiles" ).toString() );
	settings.endGroup();
	//Pixmap = QPixmap( tilesFn );
}

int BombTile::steps() const
{
	return 0;
	//return Pixmap.width() /Size.width();
}

QPixmap BombTile::tile( int step ) const
{
	return QPixmap();
	/*
	const QPoint point = QPoint( step *Size.width(), 0 );
	return Pixmap.copy( QRect( point, Size ) );
	*/
}
