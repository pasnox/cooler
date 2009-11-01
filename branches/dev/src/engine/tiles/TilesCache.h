#ifndef TILESCACHE_H
#define TILESCACHE_H

#include <QCache>
#include <QPixmap>

// defaulting to 10MB of pixmap cache should be enought
#define DEFAULT_KB_CACHE_SIZE 1024 *10

class TilesCache : public QCache<uint, QPixmap>
{
public:
	static TilesCache* instance();
	
	TilesCache( int cacheSizeinKb = DEFAULT_KB_CACHE_SIZE );
	virtual ~TilesCache();
	
	int cacheLimit() const;
	void setCacheLimit( int kb );
	
	bool find( const uint& key, QPixmap& pm ) const;
	bool find( const QString& key, QPixmap& pm ) const;
	bool insert( const uint& key, const QPixmap& pm );
	bool insert( const QString& key, const QPixmap& pm );
	
	QPixmap pixmap( const uint& key, const QString& filePath = QString::null );
	QPixmap pixmap( const QString& key, const QString& filePath = QString::null );
	
	static int pixmapCost( const QPixmap& pixmap );
};

#endif // TILESCACHE_H
