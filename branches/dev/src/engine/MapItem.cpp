#include "MapItem.h"
#include "Map.h"
#include "PlayerItem.h"
#include "MapObjectItem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

MapItem::MapItem( QGraphicsItem* parent )
	: QGraphicsWidget( parent )
{
	mMap = 0;
	setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );
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
	QGraphicsWidget::paint( painter, option, widget );
}

bool MapItem::loadMap( Map* map )
{
	if ( !map || !map->isValid() )
	{
		return false;
	}
	
	mObjects.clear();
	mMap = map;
	const QSize tileScaledSize = mMap->mTiles->tileScaledSize();
	const QSize size = mMap->mSize;
	
	foreach ( const uint layer, mMap->mLayers.keys() )
	{
		foreach ( const QPoint& point, mMap->mLayers[ layer ].keys() )
		{
			AbstractTile* tile = mMap->mLayers[ layer ][ point ];
			MapObjectItem* object = new MapObjectItem( tile, this );
			object->setZValue( layer );
			
			mObjects[ layer ][ point ] = object;
			
			if ( point.x() >= size.width() || point.y() >= size.height() )
			{
				delete object;
				mObjects[ layer ].remove( point );
			}
			else
			{
				const QPoint itemPos( point.x() *tileScaledSize.width(), point.y() *tileScaledSize.height() );
				object->setPos( itemPos );
			}
		}
	}
	
	setMinimumSize( size.width() *tileScaledSize.width(), size.height() *tileScaledSize.height() );
	
	return true;
}

QPoint MapItem::gridToPos( const QPoint& gridPos ) const
{
	const QSize tileScaledSize = mMap->mTiles->tileScaledSize();
	return QPoint( gridPos.x() *tileScaledSize.width(), gridPos.y() *tileScaledSize.height() );
}

QPoint MapItem::posToGrid( const QPoint& pos ) const
{
	const QSize tileScaledSize = mMap->mTiles->tileScaledSize();
	return QPoint( pos.x() /tileScaledSize.width(), pos.y() /tileScaledSize.height() );
}

QPoint MapItem::gridPos( MapObjectItem* object ) const
{
	const QPoint invalidPos( -1, -1 );
	const int layer = object->zValue();
	QPoint p = mObjects.value( layer ).key( object, invalidPos );
	
	if ( p == invalidPos )
	{
		const QSize tileScaledSize = mMap->mTiles->tileScaledSize();
		p = QPoint( object->pos().x() /tileScaledSize.width(), object->pos().y() /tileScaledSize.height() );
	}
	
	return p;
}

QList<MapObjectItem*> MapItem::graphicsItemListToMapObjectItemList( const QList<QGraphicsItem*>& items ) const
{
	QList<MapObjectItem*> objects;
	
	foreach ( QGraphicsItem* item, items )
	{
		MapObjectItem* object = qgraphicsitem_cast<MapObjectItem*>( item );
		
		if ( object )
		{
			objects << object;
		}
	}
	
	return objects;
}

QList<MapObjectItem*> MapItem::objectsAt( const QPoint& pos ) const
{
	QList<QGraphicsItem*> items = scene()->items( pos );
	return graphicsItemListToMapObjectItemList( items );
}

QList<MapObjectItem*> MapItem::objectsIn( const QRect& rect ) const
{
	QList<QGraphicsItem*> items = scene()->items( rect );
	return graphicsItemListToMapObjectItemList( items );
}

void MapItem::moveObjectToGridPosition( MapObjectItem* object, const QPoint& position )
{
	const QPoint pos = gridToPos( position );
	object->setPos( pos );
}

void MapItem::movePlayerBySteps( PlayerItem* player, const QPoint& steps )
{
	QPoint pos = player->pos().toPoint() +steps;
	
	// bound to map space
	pos.rx() = qBound( 0, pos.x(), int( size().width() -player->boundingRect().width() -1 ) );
	pos.ry() = qBound( 0, pos.y(), int( size().height() -player->boundingRect().height() -1 ) );
	
	// check for walkable objects
	QPoint p1 = pos;
	QPoint p2 = pos;
	
	// calculate bounding rect for getting objects
	if ( player->properties().mStrokes.testFlag( Globals::PadStrokeDown ) )
	{
		p1.ry() += player->boundingRect().height() -1;
		p2.ry() = p1.ry();
		p2.rx() += player->boundingRect().width() -1;
	}
	
	if ( player->properties().mStrokes.testFlag( Globals::PadStrokeRight ) )
	{
		p1.rx() += player->boundingRect().width() -1;
		p2.rx() = p1.rx();
		p2.ry() += player->boundingRect().height() -1;
	}
	
	if ( player->properties().mStrokes.testFlag( Globals::PadStrokeLeft ) )
	{
		p2.ry() += player->boundingRect().height() -1;
	}
	
	if ( player->properties().mStrokes.testFlag( Globals::PadStrokeUp ) )
	{
		p2.rx() += player->boundingRect().width() -1;
	}
	
	// get items
	const QRect collisionRect = mapRectToScene( QRect( p1, p2 ) ).toRect();
	QList<MapObjectItem*> objectsList = objectsIn( collisionRect );
	QMap<QPoint, MapObjectItem*> objects;
	QSet<MapObjectItem*> walkableObjects;
	
	// minimized objects map
	foreach ( MapObjectItem* object, objectsList )
	{
		if ( !object->isValid() || object == player )
		{
			continue;
		}
		
		const QPoint p = gridPos( object );
		
		if ( objects.contains( p ) )
		{
			MapObjectItem* curObject = objects[ pos ];
			
			if ( curObject->isWalkable() )
			{
				walkableObjects.remove( curObject );
				objects[ pos ] = object;
				
				if ( object->isWalkable() )
				{
					walkableObjects << object;
				}
			}
		}
		else
		{
			objects[ pos ] = object;
			
			if ( object->isWalkable() )
			{
				walkableObjects << object;
			}
		}
	}
	
	// determine the item to walk to
	MapObjectItem* walkToObject = nearestObject( collisionRect.center(), player->properties().mStrokes, walkableObjects );
	
	//qWarning() << ( walkToObject ? "have way" : "no way" ) << ( walkToObject ? walkToObject->isWalkable() : false );
	
	// return if can't walk
	if ( !walkToObject || ( gridPos( walkToObject ) == gridPos( player ) && walkToObject->pos() == player->pos() ) )
	{
		return;
	}
	
	/*
	foreach ( MapObjectItem* object, walkableObjects )
	{
		qWarning() << object->isWalkable() << object->tile()->name();
	}
	*/
	
	const QRect bbr = mapRectFromScene( player->mapRectToScene( player->boundingRect() ) ).toRect();
	
	//qWarning() << bbr << player->pos() << walkToObject->pos();
	//qWarning() << gridPos( player ) << gridPos( walkToObject );
	
	// calculate auto stroke
	if ( player->properties().mStrokes.testFlag( Globals::PadStrokeLeft ) || player->properties().mStrokes.testFlag( Globals::PadStrokeRight ) )
	{
		const bool noUpDown = !player->properties().mStrokes.testFlag( Globals::PadStrokeUp ) && !player->properties().mStrokes.testFlag( Globals::PadStrokeDown );
		
		if ( bbr.y() < walkToObject->y() )
		{
			//qWarning() << "#1";
			pos.rx() -= steps.x();
			
			if ( noUpDown )
			{
				pos.ry() += abs( steps.x() );
			}
		}
		
		if ( bbr.y() > walkToObject->y() )
		{
			//qWarning() << "#2";
			pos.rx() -= steps.x();
			
			if ( noUpDown )
			{
				pos.ry() -= abs( steps.x() );
			}
		}
	}
	
	if ( player->properties().mStrokes.testFlag( Globals::PadStrokeUp ) || player->properties().mStrokes.testFlag( Globals::PadStrokeDown ) )
	{
		const bool noLeftRight = !player->properties().mStrokes.testFlag( Globals::PadStrokeLeft ) && !player->properties().mStrokes.testFlag( Globals::PadStrokeRight );
		
		if ( bbr.x() < walkToObject->x() )
		{
			//qWarning() << "#3";
			if ( noLeftRight )
			{
				pos.rx() += abs( steps.y() );
			}
			
			pos.ry() -= steps.y();
		}
		
		if ( bbr.x() > walkToObject->x() )
		{
			//qWarning() << "#4";
			if ( noLeftRight )
			{
				pos.rx() -= abs( steps.y() );
			}
			
			pos.ry() -= steps.y();
		}
	}
	
	player->setPos( pos );
}

QPoint MapItem::closestPos( const QPoint& pos ) const
{
	const QSize tileScaledSize = mMap->mTiles->tileScaledSize();
	const QPoint point( pos.x() /tileScaledSize.width(), pos.y() /tileScaledSize.height() );
	return QPoint( point.x() *tileScaledSize.width(), point.y() *tileScaledSize.height() );
}


MapObjectItem* MapItem::nearestObject( const QPoint& strokePoint, Globals::PadStrokes strokes, const QSet<MapObjectItem*>& objects ) const
{
	if ( objects.isEmpty() )
	{
		return 0;
	}
	else if ( objects.count() == 1 )
	{
		return *objects.begin();
	}
	
	QMap<int, MapObjectItem*> items;
	
	foreach ( MapObjectItem* object, objects )
	{
		const QPoint center = object->mapToScene( object->boundingRect().center() ).toPoint();
		int diff, min = 0, max = 0;
		
		if ( strokes.testFlag( Globals::PadStrokeLeft ) || strokes.testFlag( Globals::PadStrokeRight ) )
		{
			min = qMin( strokePoint.y(), center.y() );
			max = qMax( strokePoint.y(), center.y() );
			
			diff = max -min;
			items[ diff ] = object;
		}
		
		if ( strokes.testFlag( Globals::PadStrokeUp ) || strokes.testFlag( Globals::PadStrokeDown ) )
		{
			min = qMin( strokePoint.x(), center.x() );
			max = qMax( strokePoint.x(), center.x() );
			
			diff = max -min;
			items[ diff ] = object;
		}
	}
	
	return items.values().first();
}
