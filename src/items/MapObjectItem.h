#ifndef MAPOBJECTITEM_H
#define MAPOBJECTITEM_H

#include <QGraphicsPixmapItem>

#include <AbstractTile.h>

class MapObjectItem : public QGraphicsPixmapItem
{
public:
	MapObjectItem( AbstractTile* tile, QGraphicsItem* parent );
	
	virtual int type() const;
	virtual QRectF boundingRect() const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	Globals::TypeTile tileType() const;
	AbstractTile* tile() const;
	void setTile( AbstractTile* tile );

protected:
	AbstractTile* mTile;
};

#endif // MAPOBJECTITEM_H
