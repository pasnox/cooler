#ifndef TILESMANAGER_H
#define TILESMANAGER_H

#include <QApplication>

#include "ObjectTile.h"
#include "PlayerTile.h"
#include "BombTile.h"

class TilesManager : public QObject
{
	Q_OBJECT
	
public:
	static TilesManager* instance();
	
	QString datasPath() const;
	void setDatasPath( const QString& path );
	
	#warning renamme the member scaledTileSize() which is most appropriate name
	QSize tileSize() const;
	void setTileSize( const QSize& size );
	
	bool loadDatas();
	TypesTilesMap tiles() const;
	TilesMap tiles( Globals::TypeTile type ) const;
	AbstractTile* tile( const QString& key ) const;
	QString tileKey( AbstractTile* tile ) const;
	
protected:
	QString mDatasPath;
	QSize mTileSize;
	TypesTilesMap mTiles;
	
	TilesManager( QObject* parent = 0, const QString& datasPath = QApplication::applicationDirPath() +"/Graphics" );
	virtual ~TilesManager();
	
	void loadTiles( Globals::TypeTile type, const QString& path );

signals:
	void datasLoaded( bool success );
};

#endif // TILESMANAGER_H
