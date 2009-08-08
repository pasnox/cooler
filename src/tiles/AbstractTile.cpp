#include "AbstractTile.h"

AbstractTile::AbstractTile( const QFileInfo& fn, Globals::TypeTile type )
{
	FileName = fn;
	Type = type;
	Pixmap = QPixmap( absoluteFilePath() );
}

AbstractTile::~AbstractTile()
{
}

QString AbstractTile::absoluteFilePath() const
{
	return FileName.absoluteFilePath();
}

QString AbstractTile::name() const
{
	return FileName.baseName();
}

QByteArray AbstractTile::toByteArray( const AbstractTile* tile )
{
	return QByteArray( (const char*)&tile, sizeof(tile) );
}

AbstractTile* AbstractTile::fromByteArray( const QByteArray& datas )
{
	return *(AbstractTile**)datas.constData();
}
