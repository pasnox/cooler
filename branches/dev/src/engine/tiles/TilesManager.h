#ifndef TILESMANAGER_H
#define TILESMANAGER_H

#include <QApplication>

#include "AbstractTile.h"

class TilesManager : public QObject
{
	Q_OBJECT
	
public:
	static TilesManager* instance();
	
	QString dataPath() const;
	void setDataPath( const QString& path );
	
	QSize tileScaledSize() const;
	void setTileScaledSize( const QSize& size );
	
	bool loadDatas();
	TypesTilesMap tiles() const;
	TilesMap tiles( Globals::TypeTile type ) const;
	AbstractTile* tile( const QString& key ) const;
	QString tileKey( AbstractTile* tile ) const;
	
protected:
	QString mDataPath;
	QSize mTileScaledSize;
	TypesTilesMap mTiles;
	
	TilesManager( QObject* parent = 0, const QString& dataPath = QApplication::applicationDirPath() +"/Graphics" );
	virtual ~TilesManager();
	
	void loadTiles( Globals::TypeTile type, const QString& path );

signals:
	void dataLoaded( bool success );
};

#endif // TILESMANAGER_H
