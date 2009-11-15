#include "AbstractTile.h"
#include "TilesManager.h"
#include "TilesCache.h"

AbstractTile::AbstractTile( const QFileInfo& fn, Globals::TypeTile type )
{
	FileName = fn;
	Type = type;
}

AbstractTile::~AbstractTile()
{
}

QString AbstractTile::absoluteFilePath() const
{
	return FileName.absoluteFilePath();
}

QString AbstractTile::relativeFilePath() const
{
	return Globals::relativeFilePath( TilesManager::instance()->dataPath(), FileName.absoluteFilePath() );
}

QString AbstractTile::name() const
{
	return FileName.baseName();
}

QPixmap AbstractTile::pixmap() const
{
	return TilesCache::instance()->pixmap( qHash( absoluteFilePath() ), absoluteFilePath() );
}

QByteArray AbstractTile::toByteArray( const AbstractTile* tile )
{
	return QByteArray( (const char*)&tile, sizeof(tile) );
}

AbstractTile* AbstractTile::fromByteArray( const QByteArray& datas )
{
	return *(AbstractTile**)datas.constData();
}
