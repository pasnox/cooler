#include "MapItem.h"
#include "ObjectItem.h"
#include "PlayerItem.h"

#include <QFile>
#include <QSettings>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

MapItem::MapItem( QGraphicsItem* parent )
	: QGraphicsRectItem( parent )
{
	setPen( QPen( Qt::NoPen ) );
	mTiles = TilesManager::instance();
}

MapItem::~MapItem()
{
}

int MapItem::type() const
{
	return Type;
}

void MapItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QGraphicsRectItem::paint( painter, option, widget );
}

QString MapItem::name() const
{
	return mName;
}

QSize MapItem::size() const
{
	return mSize;
}

LayersMap MapItem::layers() const
{
	return mLayers;
}

LayerMap MapItem::layer( uint id ) const
{
	return mLayers.value( id );
}

void MapItem::clear()
{
	qDeleteAll( childItems() );
	mName.clear();
	mSize = QSize();
	mMapping.clear();
	mLayers.clear();
}

bool MapItem::load( const QString& fileName )
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
				const ObjectTile tile = mappedObjectTile( id );
				ObjectItem* object = new ObjectItem( tile, this );
				object->setZValue( layer );
				mLayers[ layer ][ QPoint( x, y ) ] = object;
			}
		}
		settings.endGroup();
	}
	
	updateMap();
	
	return true;
}

QPoint MapItem::canStrokeTo( PlayerItem* player, Globals::PlayerStroke stroke ) const
{
	const int stepBy = 1;
	const QRect bbr = player->mapRectToScene( player->bodyBoundingRect() ).toRect();
	const int mw = bbr.width() /8;
	const int mh = bbr.height() /8;
	QPoint p;
	QRect sr;
	
	// check map bounding rect
	switch ( stroke )
	{
		case Globals::UpStroke:
			if ( !( bbr.y() -stepBy >= y() ) )
				return p;
			sr = QRect( QPoint( bbr.left() +mw, bbr.top() -1 ), QSize( mw *6, 1 ) );
			break;
		case Globals::DownStroke:
			if ( !( bbr.y() +stepBy +bbr.height() <= boundingRect().height() ) )
				return p;
			sr = QRect( QPoint( bbr.left() +mw, bbr.bottom() +1 ), QSize( mw *6, 1 ) );
			break;
		case Globals::LeftStroke:
			if ( !( bbr.x() -stepBy >= x() ) )
				return p;
			sr = QRect( QPoint( bbr.left() -1, bbr.top() +mh ), QSize( 1, mh *6 ) );
			break;
		case Globals::RightStroke:
			if ( !( bbr.x() +stepBy +bbr.width() <= boundingRect().width() ) )
				return p;
			sr = QRect( QPoint( bbr.right() +1, bbr.top() +mh ), QSize( 1, mh *6 ) );
			break;
		default:
			break;
	}
	
	mDebugRect = sr;
	QMap<QPoint, ObjectItem*> objects;
	QSet<ObjectItem*> walkableObjects;
	
	// minimized objects map
	foreach ( QGraphicsItem* item, scene()->items( sr ) )
	{
		ObjectItem* object = qgraphicsitem_cast<ObjectItem*>( item );
		
		if ( !object )
		{
			continue;
		}
		
		const Globals::TypeObject type = object->objectTile().Type;
		const QPoint pos = objectPos( object );
		
		switch ( type )
		{
			case Globals::FloorObject:
			case Globals::BlockObject:
			case Globals::BoxObject:
				break;
			case Globals::SkyObject:
			case Globals::InvalidObject:
			default:
				continue;
				break;
		}
		
		if ( objects.contains( pos ) )
		{
			ObjectItem* curObject = objects[ pos ];
			const Globals::TypeObject curType = curObject->objectTile().Type;
			
			if ( curType == Globals::FloorObject )
			{
				walkableObjects.remove( curObject );
				objects[ pos ] = object;
				
				if ( type == Globals::FloorObject )
				{
					walkableObjects << object;
				}
			}
		}
		else
		{
			objects[ pos ] = object;
			
			if ( type == Globals::FloorObject )
			{
				walkableObjects << object;
			}
		}
	}
	
	// return if can't walk
	if ( walkableObjects.isEmpty() )
	{
		return p;
	}
	
	// determine the item to walk to
	ObjectItem* walkToObject = walkableObjects.count() == 1 ? *walkableObjects.begin() : 0;
	
	// determine nearest item if needed
	if ( !walkToObject )
	{
		walkToObject = nearestObject( sr.center(), stroke, walkableObjects );
	}
	
	// calculate the step to move to
	switch ( stroke )
	{
		case Globals::UpStroke:
		case Globals::DownStroke:
			if ( bbr.x() != walkToObject->x() )
			{
				if ( bbr.x() < walkToObject->x() )
					p.rx() += stepBy;
				else
					p.rx() -= stepBy;
			}
			else
			{
				if ( stroke == Globals::UpStroke )
					p.ry() -= stepBy;
				else
					p.ry() += stepBy;
			}
			break;
		case Globals::LeftStroke:
		case Globals::RightStroke:
			if ( bbr.y() != walkToObject->y() )
			{
				if ( bbr.y() < walkToObject->y() )
					p.ry() += stepBy;
				else
					p.ry() -= stepBy;
			}
			else
			{
				if( stroke == Globals::LeftStroke )
					p.rx() -= stepBy;
				else
					p.rx() += stepBy;
			}
			break;
		default:
			break;
	}
	
	// return possible walk
	return p;
}

QPoint MapItem::gridToPos( const QPoint& gridPos ) const
{
	const QSize tileSize = mTiles->tileSize();
	return QPoint( gridPos.x() *tileSize.width(), gridPos.y() *tileSize.height() );
}

QPoint MapItem::posToGrid( const QPoint& pos ) const
{
	const QSize tileSize = mTiles->tileSize();
	return QPoint( pos.x() /tileSize.width(), pos.y() /tileSize.height() );
}

QPoint MapItem::closestPos( const QPoint& pos ) const
{
	const QSize tileSize = mTiles->tileSize();
	const QPoint point( pos.x() /tileSize.width(), pos.y() /tileSize.height() );
	return QPoint( point.x() *tileSize.width(), point.y() *tileSize.height() );
}

ObjectTile MapItem::mappedObjectTile( uint id ) const
{
	const QString name = mMapping.value( id );
	return mTiles ? mTiles->objectTile( name ) : ObjectTile();
}

QPoint MapItem::objectPos( ObjectItem* object ) const
{
	const QPoint invalidPos( -1, -1 );
	const int layer = object->zValue();
	QPoint p = mLayers.value( layer ).key( object, invalidPos );
	
	if ( p == invalidPos )
	{
		const QSize tileSize = mTiles->tileSize();
		p = QPoint( object->pos().x() /tileSize.width(), object->pos().y() /tileSize.height() );
	}
	
	return p;
}

ObjectItem* MapItem::nearestObject( const QPoint& strokePoint, Globals::PlayerStroke stroke, const QSet<ObjectItem*>& objects ) const
{
	QMap<int, ObjectItem*> items;
	
	foreach ( ObjectItem* object, objects )
	{
		const QPoint center = object->mapToScene( object->boundingRect().center() ).toPoint();
		int diff, min, max;
		
		switch ( stroke )
		{
			case Globals::UpStroke:
			case Globals::DownStroke:
				min = qMin( strokePoint.x(), center.x() );
				max = qMax( strokePoint.x(), center.x() );
				break;
			case Globals::LeftStroke:
			case Globals::RightStroke:
				min = qMin( strokePoint.y(), center.y() );
				max = qMax( strokePoint.y(), center.y() );
				break;
			default:
				Q_ASSERT( 0 );
				break;
		}
		
		diff = max -min;
		items[ diff ] = object;
	}
	
	return items.values().first();
}

void MapItem::updateMap()
{
	const QSize tileSize = mTiles->tileSize();
	
	foreach ( const int layer, mLayers.keys() )
	{
		foreach ( const QPoint& pos, mLayers[ layer ].keys() )
		{
			ObjectItem* object = mLayers[ layer ][ pos ];
			
			if ( pos.x() >= mSize.width() || pos.y() >= mSize.height() )
			{
				delete object;
				mLayers[ layer ].remove( pos );
			}
			else
			{
				const QPoint itemPos( pos.x() *tileSize.width(), pos.y() *tileSize.height() );
				object->setPos( itemPos );
				
				if ( pos.y() %2 == 0 )
				{
					object->mDebugColor = pos.x() %2 == 0 ? Qt::green : Qt::yellow;
				}
				else
				{
					object->mDebugColor = pos.x() %2 == 0 ? Qt::yellow : Qt::green;
				}
			}
		}
	}
	
	setRect( 0, 0, mSize.width() *tileSize.width(), mSize.height() *tileSize.height() );
}
