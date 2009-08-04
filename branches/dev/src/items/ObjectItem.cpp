#include "ObjectItem.h"

#include <QPainter>
#include <QDebug>

ObjectItem::ObjectItem( AbstractTile* tile, QGraphicsItem* parent )
	: AbstractItem( tile, parent )
{
}

ObjectItem::~ObjectItem()
{
}

int ObjectItem::type() const
{
	return Type;
}
