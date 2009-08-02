#include "ObjectItem.h"

#include <QPainter>
#include <QDebug>

ObjectItem::ObjectItem( const ObjectTile& tile, QGraphicsItem* parent )
	: QGraphicsPixmapItem( parent )
{
	setObjectTile( tile );
	mDebugColor = Qt::transparent;
}

ObjectItem::~ObjectItem()
{
}

int ObjectItem::type() const
{
	return Type;
}

QRectF ObjectItem::boundingRect() const
{
	return QRectF( QPointF( 0, 0 ), pixmap().size() );
}

void ObjectItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QGraphicsPixmapItem::paint( painter, option, widget );
}

ObjectTile ObjectItem::objectTile() const
{
	return mObjectTile;
}

void ObjectItem::setObjectTile( const ObjectTile& tile )
{
	mObjectTile = tile;
	setPixmap( mObjectTile.Pixmap );
}
