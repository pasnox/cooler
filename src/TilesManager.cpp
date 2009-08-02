#include "TilesManager.h"

#include <QDir>
#include <QDebug>

TilesManager::TilesManager( QObject* parent, const QString& datasPath )
	: QObject( parent )
{
	mTileSize = QSize( 48, 48 );
	setDatasPath( datasPath );
}

TilesManager::~TilesManager()
{
}

TilesManager* TilesManager::instance()
{
	static TilesManager* _instance = 0;
	
	if ( !_instance )
	{
		_instance = new TilesManager( qApp, qApp->applicationDirPath() +"/Graphics" );
	}
	
	return _instance;
}

QString TilesManager::datasPath() const
{
	return mDatasPath;
}

void TilesManager::setDatasPath( const QString& path )
{
	mDatasPath = path;
}

QSize TilesManager::tileSize() const
{
	return mTileSize;
}

void TilesManager::setTileSize( const QSize& size )
{
	mTileSize = size;
}

bool TilesManager::loadDatas()
{
	QDir datas( mDatasPath );
	
	if ( !datas.exists() )
	{
		emit datasLoaded( false );
		return false;
	}
	
	mObjectTiles.clear();
	
	const QStringList filters = QStringList( "blocks" ) << "bombs" << "boxes" << "explosions" << "floors" << "players" << "sky";
	const QFileInfoList folders = datas.entryInfoList( filters, QDir::Dirs );
	
	foreach ( const QFileInfo& folder, folders )
	{
		if ( folder.fileName().toLower() == "blocks" )
		{
			loadObjectTiles( Globals::BlockObject, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "boxes" )
		{
			loadObjectTiles( Globals::BoxObject, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "floors" )
		{
			loadObjectTiles( Globals::FloorObject, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "sky" )
		{
			loadObjectTiles( Globals::SkyObject, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "players" )
		{
			loadPlayerTiles( folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "bombs" )
		{
			loadBombTiles( folder.absoluteFilePath() );
		}
	}
	
	emit datasLoaded( true );
	return true;
}

ObjectTile TilesManager::objectTile( const QString& key ) const
{
	return mObjectTiles.value( key );
}

QString TilesManager::objectTileName( const ObjectTile& tile ) const
{
	return mObjectTiles.key( tile );
}

TilesMap TilesManager::objectTiles() const
{
	return mObjectTiles;
}

PlayerTile TilesManager::playerTile( const QString& key )
{
	return mPlayerTiles.value( key );
}

PlayerTilesMap TilesManager::playerTiles() const
{
	return mPlayerTiles;
}

BombTile TilesManager::bombTile( const QString& key ) const
{
	return mBombTiles.value( key );
}

BombTilesMap TilesManager::bombTiles() const
{
	return mBombTiles;
}

QString TilesManager::relativeFilePath( const QString& fn ) const
{
	QDir dir( mDatasPath );
	return dir.relativeFilePath( fn );
}

QString TilesManager::relativeFilePath( const QFileInfo& fn ) const
{
	return relativeFilePath( fn.absoluteFilePath() );
}

QFileInfoList TilesManager::getFiles( QDir& fromDir, const QStringList& filters ) const
{
	const QFileInfoList entries = fromDir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name );
	QFileInfoList files;
	
	foreach ( const QFileInfo& file, entries )
	{
		if ( file.isFile() && ( filters.isEmpty() || QDir::match( filters, file.fileName() ) ) )
		{
			files << file;
		}
		else if ( file.isDir() )
		{
			fromDir.cd( file.filePath() );
			files << getFiles( fromDir, filters );
			fromDir.cdUp();
		}
	}
	
	return files;
}

void TilesManager::loadObjectTiles( Globals::TypeObject type, const QString& path )
{
	QDir dir( path );
	
	foreach ( const QFileInfo& file, getFiles( dir ) )
	{
		const QString key = relativeFilePath( file );
		ObjectTile tile( type, file );
		mObjectTiles[ key ] = tile;
	}
}

void TilesManager::loadPlayerTiles( const QString& path )
{
	QDir dir( path );
	const QStringList filters = QStringList( "*.ini" );
	
	foreach ( const QFileInfo& file, getFiles( dir, filters ) )
	{
		PlayerTile tile( file );
		const QString key = tile.Name;
		mPlayerTiles[ key ] = tile;
	}
}

void TilesManager::loadBombTiles( const QString& path )
{
	QDir dir( path );
	const QStringList filters = QStringList( "*.ini" );
	
	foreach ( const QFileInfo& file, getFiles( dir, filters ) )
	{
		BombTile tile( file );
		const QString key = tile.Name;
		mBombTiles[ key ] = tile;
	}
}
