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
	/*
	const PlayersPositionMap& playersPosition() const;
	int maxPlayers() const;
	*/
	QPoint canStrokeTo( PlayerItem* player, Globals::PadStroke stroke ) const;
	QPoint gridToPos( const QPoint& gridPos ) const;
	QPoint posToGrid( const QPoint& pos ) const;
	QPoint gridPos( MapObjectItem* object ) const;
	QPoint closestPos( const QPoint& pos ) const;
	
protected:
	Map* mMap;
	LayersMapObjects mObjects;
	
	MapObjectItem* nearestObject( const QPoint& strokePoint, Globals::PadStroke stroke, const QSet<MapObjectItem*>& objects ) const;
};

#endif // MAPITEM_H
