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
	
	const QRect collisionRect = mapRectToScene( QRect( p1, p2 ) ).toRect();
	QList<MapObjectItem*> objectsList = objectsIn( collisionRect );
	
	// temporary code
	foreach ( MapObjectItem* object, objectsList )
	{
		if ( !object->isWalkable() )
		{
			return;
		}
	}
	//end temporary code
	
	// ***********
	/*
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
	
	// return if can't walk
	if ( !walkToObject )
	{
		return;
	}
	
	const QRect bbr = player->mapRectToScene( player->boundingRect() ).toRect();
	const QRect ebr = player->mapRectToScene( player->explosiveBoundingRect() ).toRect();
	
	// calculate the step to move to
	if ( player->properties().mStrokes.testFlag( Globals::PadStrokeLeft ) || player->properties().mStrokes.testFlag( Globals::PadStrokeRight ) )
	{
		if ( bbr.y() != walkToObject->y() )
		{
			if ( bbr.y() < walkToObject->y() )
				pos.ry() += steps.y();
			else
				pos.ry() -= steps.y();
		}
		else
		{
			if( player->properties().mStrokes.testFlag( Globals::PadStrokeLeft ) )
				pos.rx() -= steps.x();
			else
				pos.rx() += steps.x();
		}
	}
	
	if ( player->properties().mStrokes.testFlag( Globals::PadStrokeUp ) || player->properties().mStrokes.testFlag( Globals::PadStrokeDown ) )
	{
		if ( bbr.x() != walkToObject->x() )
		{
			if ( bbr.x() < walkToObject->x() )
				pos.rx() += steps.x();
			else
				pos.rx() -= steps.x();
		}
		else
		{
			if ( player->properties().mStrokes.testFlag( Globals::PadStrokeUp ) )
				pos.ry() -= steps.y();
			else
				pos.ry() += steps.y();
		}
	}
	*/
	
	player->setPos( pos );
}

QPoint MapItem::canStrokeTo( PlayerItem* player, Globals::PadStroke stroke ) const
{
	const int stepBy = 1;
	const QRect bbr = player->mapRectToScene( player->boundingRect() ).toRect();
	const QRect ebr = player->mapRectToScene( player->explosiveBoundingRect() ).toRect();
	const int mw = ebr.width() /8;
	const int mh = ebr.height() /8;
	QPoint p;
	QRect sr;
	
	// check map bounding rect
	switch ( stroke )
	{
		case Globals::PadStrokeUp:
			if ( !( bbr.y() -stepBy >= y() ) )
				return p;
			sr = QRect( QPoint( ebr.left() +mw, ebr.top() -1 ), QSize( mw *6, 1 ) );
			break;
		case Globals::PadStrokeDown:
			if ( !( bbr.y() +stepBy +bbr.height() <= boundingRect().height() ) )
				return p;
			sr = QRect( QPoint( ebr.left() +mw, ebr.bottom() +1 ), QSize( mw *6, 1 ) );
			break;
		case Globals::PadStrokeLeft:
			if ( !( bbr.x() -stepBy >= x() ) )
				return p;
			sr = QRect( QPoint( ebr.left() -1, ebr.top() +mh ), QSize( 1, mh *6 ) );
			break;
		case Globals::PadStrokeRight:
			if ( !( bbr.x() +stepBy +bbr.width() <= boundingRect().width() ) )
				return p;
			sr = QRect( QPoint( ebr.right() +1, ebr.top() +mh ), QSize( 1, mh *6 ) );
			break;
		default:
			break;
	}
	
	QMap<QPoint, MapObjectItem*> objects;
	QSet<MapObjectItem*> walkableObjects;
	
	// minimized objects map
	foreach ( QGraphicsItem* item, scene()->items( sr ) )
	{
		MapObjectItem* object = qgraphicsitem_cast<MapObjectItem*>( item );
		
		if ( !object || !object->isValid() || object == player )
		{
			continue;
		}
		
		const QPoint pos = gridPos( object );
		
		if ( objects.contains( pos ) )
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
	MapObjectItem* walkToObject = nearestObject( sr.center(), stroke, walkableObjects );
	
	// return if can't walk
	if ( !walkToObject )
	{
		return p;
	}
	
	// calculate the step to move to
	switch ( stroke )
	{
		case Globals::PadStrokeUp:
		case Globals::PadStrokeDown:
			if ( bbr.x() != walkToObject->x() )
			{
				if ( bbr.x() < walkToObject->x() )
					p.rx() += stepBy;
				else
					p.rx() -= stepBy;
			}
			else
			{
				if ( stroke == Globals::PadStrokeUp )
					p.ry() -= stepBy;
				else
					p.ry() += stepBy;
			}
			break;
		case Globals::PadStrokeLeft:
		case Globals::PadStrokeRight:
			if ( bbr.y() != walkToObject->y() )
			{
				if ( bbr.y() < walkToObject->y() )
					p.ry() += stepBy;
				else
					p.ry() -= stepBy;
			}
			else
			{
				if( stroke == Globals::PadStrokeLeft )
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
