#ifndef MAPOBJECTITEM_H
#define MAPOBJECTITEM_H

#include <QGraphicsPixmapItem>

#include <AbstractTile.h>

class MapItem;

class MapObjectItem : public QGraphicsPixmapItem
{
public:
	enum { Type = Globals::MapObjectItem };
	
	MapObjectItem( AbstractTile* tile, QGraphicsItem* parent );
	
	virtual int type() const;
	virtual QRectF boundingRect() const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	virtual bool isValid() const;
	virtual bool isWalkable() const;
	virtual bool isExplosive() const;
	MapItem* map() const;
	Globals::TypeTile tileType() const;
	AbstractTile* tile() const;
	virtual void setTile( AbstractTile* tile );

protected:
	AbstractTile* mTile;
};

#endif // MAPOBJECTITEM_H
