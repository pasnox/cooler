#include "Map.h"
#include "pXmlSettings.h"

Map::Map( const QFileInfo& fileName  )
{
	mFileName = fileName;
	mTiles = TilesManager::instance();
	
	load();
}

Map::~Map()
{
}

Map::Map( const Map& other )
{
	operator=( other );
}

Map& Map::operator=( const Map& other )
{
	if( *this != other )
	{
		mFileName = other.mFileName;
		mTiles = other.mTiles;
		mName = other.mName;
		mSize = other.mSize;
		mPlayersPosition = other.mPlayersPosition;
		mMapping = other.mMapping;
		mLayers = other.mLayers;
	}

	return *this;
}

bool Map::operator==( const Map& other ) const
{
	return mFileName == other.mFileName &&
		mTiles == other.mTiles &&
		mName == other.mName &&
		mSize == other.mSize &&
		mPlayersPosition == other.mPlayersPosition &&
		mMapping == other.mMapping &&
		mLayers == other.mLayers;
}

bool Map::operator!=( const Map& other ) const
{
	return !operator==( other );
}

bool Map::isValid() const
{
	return !mLayers.isEmpty();
}

QString Map::name() const
{
	return mName;
}

QSize Map::size() const
{
	return mSize;
}

PlayersPosition Map::playersPosition() const
{
	return mPlayersPosition;
}

LayersMap Map::layers() const
{
	return mLayers;
}

LayerMap Map::layer( uint id ) const
{
	return mLayers.value( id );
}

AbstractTile* Map::mappedTile( uint id ) const
{
	const QString name = mMapping.value( id );
	return mTiles ? mTiles->tile( name ) : 0;
}

void Map::clear()
{
	mName.clear();
	mSize = QSize();
	mPlayersPosition.clear();
	mMapping.clear();
	mLayers.clear();
}

void Map::load()
{
	if ( !mFileName.exists() )
	{
		return;
	}
	
	pXmlSettings settings( mFileName.absoluteFilePath() );
	
	if ( settings.status() == QSettings::FormatError )
	{
		return;
	}
	
	clear();
	
	mName = settings.value( "Map/Name" ).toString();
	mSize = settings.value( "Map/Size" ).toSize();
	
	// players position
	settings.beginGroup( "PlayersPosition" );
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		const QPoint position = settings.value( QString::number( i ), QPoint( -1, -1 ) ).toPoint();
		mPlayersPosition << position;
	}
	settings.endGroup();
	
	// tiles mapping
	settings.beginGroup( "Tiles" );
	const QStringList tiles = settings.childKeys();
	foreach ( const QString& tile, tiles )
	{
		uint t = tile.toUInt();
		mMapping[ t ] = settings.value( tile ).toString();
	}
	settings.endGroup();
	
	// layers
	const QStringList groups = settings.childGroups();
	foreach ( const QString& group, groups )
	{
		if ( !group.toLower().startsWith( "layer" ) )
		{
			continue;
		}
		
		const uint layer = group.section( '_', 1, 1 ).toUInt();
		
		if ( layer < Globals::FirstLayer || layer > Globals::LastLayer )
		{
			if ( layer != Globals::SkyLayer )
			{
				Q_ASSERT( 0 );
			}
		}
		
		settings.beginGroup( group );
		const QStringList keys = settings.childKeys();
		foreach ( const QString& key, keys )
		{
			const int y = key.toInt();
			const QString line = settings.value( key ).toString();
			const QStringList parts = line.split( MAP_SPLIT_CHAR, QString::SkipEmptyParts );
			
			for ( int x = 0; x < parts.count(); x++ )
			{
				if ( parts.at( x ).toLower() == "x" )
				{
					continue;
				}
				
				const uint id = parts.at( x ).toUInt();
				AbstractTile* tile = mappedTile( id );
				
				switch ( tile->Type )
				{
					case Globals::InvalidTile:
						Q_ASSERT( 0 );
						continue;
						break;
					case Globals::BlockTile:
					case Globals::BoxTile:
					case Globals::FloorTile:
					case Globals::SkyTile:
					case Globals::BonusTile:
						break;
					case Globals::PlayerTile:
					case Globals::BombTile:
					case Globals::BombExplosionTile:
					case Globals::GameScreenTile:
					case Globals::PlayerExplosionTile:
					case Globals::TextTile:
						continue;
						break;
				}
				
				mLayers[ layer ][ QPoint( x, y ) ] = tile;
			}
		}
		settings.endGroup();
	}
}
