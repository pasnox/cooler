#include "MapEditorItem.h"
#include "MapObjectItem.h"

#include <QFile>
#include <QPen>
#include <QSettings>
#include <QDebug>

#include <limits.h>

MapEditorItem::MapEditorItem( QObject* parentObject, QGraphicsItem* parent )
	: QObject( parentObject ), MapItem( parent )
{
	setPen( QPen( Qt::black ) );
}

MapEditorItem::~MapEditorItem()
{
}

void MapEditorItem::setName( const QString& name )
{
	mName = name;
}

void MapEditorItem::setSize( const QSize& size )
{
	mSize = size;
	updateMap();
}

void MapEditorItem::setLayers( const LayersMap& layers )
{
	clear();
	mLayers = layers;
	updateMap();
}

void MapEditorItem::setLayer( uint id, const LayerMap& layer )
{
	qDeleteAll( mLayers[ id ].values() );
	mLayers[ id ] = layer;
	updateMap();
}

void MapEditorItem::setLayerTile( uint id, const QPoint& pos, AbstractTile* tile )
{
	const QSize tileSize = mTiles->tileSize();
	MapObjectItem* object = mLayers.value( id ).value( pos );
	QPoint tilePos( pos.x() *tileSize.width(), pos.y() *tileSize.height() );
	
	if ( !tile )
	{
		if ( object )
		{
			delete object;
			mLayers[ id ].remove( pos );
		}
		
		return;
	}
	
	if ( object )
	{
		object->setTile( tile );
	}
	else
	{
		object = new MapObjectItem( tile, this );
		object->setPos( tilePos );
		object->setZValue( id );
		mLayers[ id ][ pos ] = object;
	}
}

void MapEditorItem::setCurrentLayer( int _layer, bool exclusive )
{
	foreach ( const int layer, mLayers.keys() )
	{
		foreach ( const QPoint& pos, mLayers[ layer ].keys() )
		{
			MapObjectItem* object = mLayers[ layer ][ pos ];
			const bool visible = exclusive ? ( _layer == -1 || _layer == layer ) : true;
			object->setVisible( visible );
		}
	}
}

void MapEditorItem::removeLayer( int layer )
{
	foreach ( const QPoint& pos, mLayers[ layer ].keys() )
	{
		delete mLayers[ layer ][ pos ];
	}
	mLayers.remove( layer );
}

void MapEditorItem::clearLayers()
{
	qDeleteAll( childItems() );
	mLayers.clear();
}

bool MapEditorItem::load( const QString& fileName )
{
	if ( !QFile::exists( fileName ) )
	{
		return false;
	}
	
	QSettings settings( fileName, QSettings::IniFormat );
	
	if ( settings.status() == QSettings::FormatError )
	{
		return false;
	}
	
	clear();
	mName = settings.value( "Map/Name" ).toString();
	mSize = settings.value( "Map/Size" ).toSize();
	
	// tiles
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
					case Globals::PlayerTile:
						break;
					case Globals::BombTile:
					case Globals::BombExplosionTile:
					case Globals::PlayerExplosionTile:
					case Globals::TextTile:
						continue;
						break;
				}
				
				MapObjectItem* object = new MapObjectItem( tile, this );
				object->setZValue( layer );
				mLayers[ layer ][ QPoint( x, y ) ] = object;
			}
		}
		settings.endGroup();
	}
	
	updateMap();
	
	return true;
}

bool MapEditorItem::save( const QString& fileName )
{
	QSettings settings( fileName, QSettings::IniFormat );
	settings.clear();
	
	
	settings.setValue( "Map/Name", mName );
	settings.setValue( "Map/Size", mSize );
	
	// layers
	QSet<QString> mappingValues = mMapping.values().toSet();
	foreach ( const int layer, mLayers.keys() )
	{
		settings.beginGroup( QString( "Layer_%1" ).arg( layer ) );
		
		for ( int y = 0; y < mSize.height(); y++ )
		{
			QStringList line;
			
			for ( int x = 0; x < mSize.width(); x++ )
			{
				MapObjectItem* object = mLayers.value( layer ).value( QPoint( x, y ) );
				
				if ( object )
				{
					AbstractTile* tile = object->tile();
					const QString name = mTiles->tileKey( tile );
					
					if ( name.isEmpty() )
					{
						line << "X";
					}
					else
					{
						if ( mappingValues.contains( name ) )
						{
							line << QString::number( mMapping.key( name ) );
						}
						else
						{
							const uint id = createMapping( name );
							mappingValues << name;
							line << QString::number( id );
						}
						
					}
				}
				else
				{
					line << "X";
				}
			}
			
			settings.setValue( QString::number( y ), line.join( MAP_SPLIT_CHAR ) );
		}
		
		settings.endGroup();
	}
	
	// tiles
	settings.beginGroup( "Tiles" );
	foreach ( uint id, mMapping.keys() )
	{
		settings.setValue( QString::number( id ), mMapping[ id ] );
	}
	settings.endGroup();
	
	settings.sync();
	
	return settings.status() == QSettings::NoError;
}

ObjectTile* MapEditorItem::objectTile( const QString& name ) const
{
	return mTiles ? (ObjectTile*)mTiles->tile( name ) : 0;
}

uint MapEditorItem::createMapping( const QString& name )
{
	for ( uint i = 0; i < INT_MAX; i++ )
	{
		if ( mMapping.contains( i ) )
		{
			continue;
		}
		
		mMapping[ i ] = name;
		return i;
	}
	
	Q_ASSERT( 0 );
	return 0;
}
