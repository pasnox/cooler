#include "AbstractItem.h"

AbstractItem::AbstractItem( AbstractTile* tile, QGraphicsItem* parent )
	: QGraphicsPixmapItem( parent )
{
	setTile( tile );
}

int AbstractItem::type() const
{
	return Globals::MapObjectItem;
}

QRectF AbstractItem::boundingRect() const
{
	return QRectF( QPointF( 0, 0 ), mTile ? mTile->tile( 0 ).size() : QSize() );
}

void AbstractItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QGraphicsPixmapItem::paint( painter, option, widget );
}

AbstractTile* AbstractItem::tile() const
{
	return mTile;
}

void AbstractItem::setTile( AbstractTile* tile )
{
	mTile = tile;
	
	if ( mTile )
	{
		setPixmap( mTile->tile( 0 ) );
	}
	else
	{
		setPixmap( QPixmap() );
	}
}

Globals::TypeTile AbstractItem::tileType() const
{
	return mTile ? Globals::InvalidTile : mTile->Type;
}
