#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QGraphicsPixmapItem>

class AbstractItem : public QGraphicsPixmapItem
{
public:
	virtual int type() const = 0;
	virtual QRectF boundingRect() const = 0;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 ) = 0;
};

#endif // ABSTRACTITEM_H
