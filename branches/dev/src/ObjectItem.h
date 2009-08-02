#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QGraphicsPixmapItem>

#include "Globals.h"
#include "TilesManager.h"

class ObjectItem : public QGraphicsPixmapItem
{
public:
	enum { Type = Globals::ObjectItem };
	
	ObjectItem( const ObjectTile& tile, QGraphicsItem* parent = 0 );
	virtual ~ObjectItem();
	
	virtual int type() const;
	virtual QRectF boundingRect() const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	ObjectTile objectTile() const;
	void setObjectTile( const ObjectTile& tile );
	
	Qt::GlobalColor mDebugColor;

protected:
	ObjectTile mObjectTile;
};

#endif // OBJECTITEM_H
