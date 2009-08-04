#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QGraphicsPixmapItem>

#include <AbstractTile.h>

class AbstractItem : public QGraphicsPixmapItem // should be renamed MapObjectItem
{
public:
	AbstractItem( AbstractTile* tile, QGraphicsItem* parent );
	
	virtual int type() const;
	virtual QRectF boundingRect() const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	Globals::TypeTile tileType() const;
	AbstractTile* tile() const;
	void setTile( AbstractTile* tile );

protected:
	AbstractTile* mTile;
};

#endif // ABSTRACTITEM_H
