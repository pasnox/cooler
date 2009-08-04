#include "MapEditorItem.h"
#include "ObjectItem.h"

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
	AbstractItem* object = mLayers[ id ][ pos ];
	QPoint tilePos( pos.x() *tileSize.width(), pos.y() *tileSize.height() );
	
	if ( object )
	{
		object->setTile( tile );
	}
	else
	{
		object = new ObjectItem( tile, this );
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
			AbstractItem* object = mLayers[ layer ][ pos ];
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

bool MapEditorItem::save( const QString& fileName )
{
return false;
/*
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
				ObjectItem* object = mLayers.value( layer ).value( QPoint( x, y ) );
				
				if ( object )
				{
					const ObjectTile tile = object->objectTile();
					const QString name = mTiles->objectTileName( tile );
					
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
*/
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
