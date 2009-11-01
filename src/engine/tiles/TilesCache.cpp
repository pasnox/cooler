#include "TilesCache.h"

#include <QDebug>

TilesCache* TilesCache::instance()
{
	static TilesCache* _instance = 0;
	
	if ( !_instance )
	{
		_instance = new TilesCache();
	}
	
	return _instance;
}

TilesCache::TilesCache( int cacheSize )
{
	setCacheLimit( cacheSize );
}

TilesCache::~TilesCache()
{
}

int TilesCache::pixmapCost( const QPixmap& pixmap )
{
	return ( pixmap.width() *pixmap.height() *pixmap.depth() ) /8;
}

int TilesCache::cacheLimit() const
{
	return maxCost() /1024;
}

void TilesCache::setCacheLimit( int kb )
{
	setMaxCost( kb *1024 );
}

bool TilesCache::find( const uint& key, QPixmap& pm ) const
{
	QPixmap* pixmap = object( key );
	
	if ( pixmap )
	{
		pm = *pixmap;
	}
	
	return pixmap;
}

bool TilesCache::find( const QString& key, QPixmap& pm ) const
{
	return find( qHash( key ), pm );
}

bool TilesCache::insert( const uint& key, const QPixmap& pm )
{
	QPixmap* pixmap = new QPixmap( pm );
	
	if ( !QCache<uint, QPixmap>::insert( key, pixmap, pixmapCost( pm ) ) )
	{
		//delete pixmap; // QCache take ownership of it, cf the doc.
		pixmap = 0;
	}
	
	return pixmap;
}

bool TilesCache::insert( const QString& key, const QPixmap& pm )
{
	return insert( qHash( key ), pm );
}

QPixmap TilesCache::pixmap( const uint& key, const QString& filePath )
{
	if ( !filePath.isEmpty() && !object( key ) )
	{
		insert( key, QPixmap( filePath ) );
	}
	
	QPixmap pixmap;
	find( key, pixmap );
	return pixmap;
}

QPixmap TilesCache::pixmap( const QString& key, const QString& filePath )
{
	return pixmap( qHash( key ), filePath );
}
