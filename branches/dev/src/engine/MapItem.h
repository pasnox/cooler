#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsWidget>

#include "Globals.h"

class Map;
class PlayerItem;
class MapObjectItem;

typedef QMap<QPoint, MapObjectItem*> LayerMapObjects;
typedef QMap<uint, LayerMapObjects> LayersMapObjects;

class MapItem : public QGraphicsWidget
{
public:
	enum { Type = Globals::MapItem };
	
	MapItem( QGraphicsItem* parent = 0 );
	virtual ~MapItem();
	
	virtual int type() const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	virtual bool loadMap( Map* map );
	
	QPoint gridToPos( const QPoint& gridPos ) const;
	QPoint posToGrid( const QPoint& pos ) const;
	QPoint gridPos( MapObjectItem* object ) const;
	
	QList<MapObjectItem*> graphicsItemListToMapObjectItemList( const QList<QGraphicsItem*>& items ) const;
	QList<MapObjectItem*> objectsAt( const QPoint& pos ) const;
	QList<MapObjectItem*> objectsIn( const QRect& rect ) const;
	
	void moveObjectToGridPosition( MapObjectItem* object, const QPoint& position );
	void movePlayerBySteps( PlayerItem* player, const QPoint& steps );
	
protected:
	Map* mMap;
	LayersMapObjects mObjects;
	
	QPoint closestPos( const QPoint& pos ) const;
	MapObjectItem* nearestObject( const QPoint& strokePoint, Globals::PadStrokes strokes, const QSet<MapObjectItem*>& objects ) const;
};

#endif // MAPITEM_H
