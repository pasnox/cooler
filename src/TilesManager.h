#ifndef TILESMANAGER_H
#define TILESMANAGER_H

#include <QFileInfo>
#include <QSettings>
#include <QPixmap>
#include <QMetaType>
#include <QApplication>
#include <QObject>
#include <QMap>

#include "tile/ObjectTile.h"
#include "tile/PlayerTile.h"
#include "tile/BombTile.h"

typedef QMap<QString, ObjectTile> TilesMap;
typedef QMap<QString, PlayerTile> PlayerTilesMap;
typedef QMap<QString, BombTile> BombTilesMap;

// TilesManager

class TilesManager : public QObject
{
	Q_OBJECT
	
public:
	static TilesManager* instance();
	
	QString datasPath() const;
	void setDatasPath( const QString& path );
	
	QSize tileSize() const;
	void setTileSize( const QSize& size );
	
	bool loadDatas();
	
	ObjectTile objectTile( const QString& key ) const;
	QString objectTileName( const ObjectTile& tile ) const;
	TilesMap objectTiles() const;
	
	PlayerTile playerTile( const QString& key );
	PlayerTilesMap playerTiles() const;
	
	BombTile bombTile( const QString& key ) const;
	BombTilesMap bombTiles() const;

protected:
	QString mDatasPath;
	QSize mTileSize;
	TilesMap mObjectTiles;
	PlayerTilesMap mPlayerTiles;
	BombTilesMap mBombTiles;
	
	TilesManager( QObject* parent = 0, const QString& datasPath = QApplication::applicationDirPath() +"/Graphics" );
	virtual ~TilesManager();
	
	QString relativeFilePath( const QString& fn ) const;
	QString relativeFilePath( const QFileInfo& fn ) const;
	QFileInfoList getFiles( QDir& path, const QStringList& filters = QStringList() ) const;
	void loadObjectTiles( Globals::TypeObject type, const QString& path );
	void loadPlayerTiles( const QString& path );
	void loadBombTiles( const QString& path );

signals:
	void datasLoaded( bool success );
};

#endif // TILESMANAGER_H
