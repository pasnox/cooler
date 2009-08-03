#ifndef ABSTRACTOBJECTITEM_H
#define ABSTRACTOBJECTITEM_H

#include <QGraphicsPixmapItem>

class AbstractObjectItem : public QGraphicsPixmapItem
{
public:
	virtual int type() const = 0;
	virtual QRectF boundingRect() const = 0;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 ) = 0;
};

#endif // ABSTRACTOBJECTITEM_H
