#include "TilesManager.h"
#include "Globals.h"

#include <QDebug>

TilesManager::TilesManager( QObject* parent, const QString& datasPath )
	: QObject( parent )
{
	mTileScaledSize = Globals::TilesSize;
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
		_instance = new TilesManager( qApp, qApp->applicationDirPath() +"/../../../tiles" );
#else
		_instance = new TilesManager( qApp, qApp->applicationDirPath() +"/tiles" );
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

QSize TilesManager::tileScaledSize() const
{
	return mTileScaledSize;
}

void TilesManager::setTileScaledSize( const QSize& size )
{
	mTileScaledSize = size;
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
	
	const QFileInfoList folders = datas.entryInfoList( QStringList( "*" ), QDir::Dirs );
	
	foreach ( const QFileInfo& folder, folders )
	{
		if ( folder.fileName().toLower() == "blocks" )
		{
			loadTiles( Globals::BlockTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "bombs" )
		{
			loadTiles( Globals::BombTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "bombs explosions" )
		{
			loadTiles( Globals::BombExplosionTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "bonus" )
		{
			loadTiles( Globals::BonusTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "boxes" )
		{
			loadTiles( Globals::BoxTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "floors" )
		{
			loadTiles( Globals::FloorTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "game screens" )
		{
			loadTiles( Globals::GameScreenTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "players" )
		{
			loadTiles( Globals::PlayerTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "players explosions" )
		{
			loadTiles( Globals::PlayerExplosionTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "sky" )
		{
			loadTiles( Globals::SkyTile, folder.absoluteFilePath() );
		}
		else if ( folder.fileName().toLower() == "texts" )
		{
			loadTiles( Globals::TextTile, folder.absoluteFilePath() );
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

QString TilesManager::tileKey( AbstractTile* tile ) const
{
	foreach ( const Globals::TypeTile& type, mTiles.keys() )
	{
		const TilesMap& map = mTiles[ type ];
		const QString key = map.key( tile );
		
		if ( !key.isNull() )
		{
			return key;
		}
	}
	
	return QString::null;
}

void TilesManager::loadTiles( Globals::TypeTile type, const QString& path )
{
	const QFileInfoList files = Globals::getFiles( path );
	
	foreach ( const QFileInfo& file, files )
	{
		const QString key = Globals::relativeFilePath( mDatasPath, file.absoluteFilePath() );
		AbstractTile* tile = 0;
		
		switch ( type )
		{
			case Globals::PlayerTile:
				tile = new PlayerTile( file );
				break;
			case Globals::BlockTile:
			case Globals::BombTile:
			case Globals::BombExplosionTile:
			case Globals::BonusTile:
			case Globals::BoxTile:
			case Globals::FloorTile:
			case Globals::GameScreenTile:
			case Globals::PlayerExplosionTile:
			case Globals::SkyTile:
			case Globals::TextTile:
				tile = new ObjectTile( file, type );
				break;
			case Globals::InvalidTile:
				Q_ASSERT( 0 );
				break;
		}
		
		mTiles[ type ][ key ] = tile;
	}
}
