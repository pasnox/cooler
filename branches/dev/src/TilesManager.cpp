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
#ifdef Q_OS_MAC
		_instance = new TilesManager( qApp, qApp->applicationDirPath() +"/../../../Graphics" );
#else
		_instance = new TilesManager( qApp, qApp->applicationDirPath() +"/Graphics" );
#endif
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
	
	foreach ( const Globals::TypeTile& type, mTiles.keys() )
	{
		qDeleteAll( mTiles[ type ] );
	}
	mTiles.clear();
	
	const QStringList filters = QStringList( "blocks" ) << "bombs" << "boxes" << "explosions" << "floors" << "players" << "sky";
	const QFileInfoList folders = datas.entryInfoList( filters, QDir::Dirs );
	
	foreach ( const QFileInfo& folder, folders )
	{
		if ( folder.fileName().toLower() == "blocks" )
		{
			loadTiles( Globals::BlockTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "boxes" )
		{
			loadTiles( Globals::BoxTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "floors" )
		{
			loadTiles( Globals::FloorTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "sky" )
		{
			loadTiles( Globals::SkyTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "players" )
		{
			loadTiles( Globals::PlayerTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "bombs" )
		{
			loadTiles( Globals::BombTile, folder.absoluteFilePath() );
		}
	}
	
	emit datasLoaded( true );
	return true;
}

TypesTilesMap TilesManager::tiles() const
{
	return mTiles;
}

TilesMap TilesManager::tiles( Globals::TypeTile type ) const
{
	return mTiles.value( type );
}

AbstractTile* TilesManager::tile( const QString& key ) const
{
	foreach ( const Globals::TypeTile& type, mTiles.keys() )
	{
		if ( mTiles[ type ].contains( key ) )
		{
			return mTiles[ type ][ key ];
		}
	}
	
	return 0;
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

void TilesManager::loadTiles( Globals::TypeTile type, const QString& path )
{
	QDir dir( path );
	QFileInfoList files;
	
	switch ( type )
	{
		case Globals::BlockTile:
		case Globals::BoxTile:
		case Globals::FloorTile:
		case Globals::SkyTile:
			files = getFiles( dir );
			break;
		case Globals::PlayerTile:
		case Globals::BombTile:
			files = getFiles( dir, QStringList( "*.ini" ) );
			break;
		case Globals::InvalidTile:
			Q_ASSERT( 0 );
			break;
	}
	
	foreach ( const QFileInfo& file, files )
	{
		const QString key = relativeFilePath( file );
		AbstractTile* tile = 0;
		
		switch ( type )
		{
			case Globals::BlockTile:
			case Globals::BoxTile:
			case Globals::FloorTile:
			case Globals::SkyTile:
				tile = new ObjectTile( file, type );
				break;
			case Globals::PlayerTile:
				tile = new PlayerTile( file, type );
				break;
			case Globals::BombTile:
				tile = new BombTile( file, type );
				break;
			case Globals::InvalidTile:
				Q_ASSERT( 0 );
				break;
		}
		
		mTiles[ type ][ key ] = tile;
	}
}
