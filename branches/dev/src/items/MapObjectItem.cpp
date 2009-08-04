#include "MapObjectItem.h"

MapObjectItem::MapObjectItem( AbstractTile* tile, QGraphicsItem* parent )
	: QGraphicsPixmapItem( parent )
{
	setTile( tile );
}

int MapObjectItem::type() const
{
	return Globals::MapObjectItem;
}

QRectF MapObjectItem::boundingRect() const
{
	return QRectF( QPointF( 0, 0 ), mTile ? mTile->tile( 0 ).size() : QSize() );
}

void MapObjectItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QGraphicsPixmapItem::paint( painter, option, widget );
}

AbstractTile* MapObjectItem::tile() const
{
	return mTile;
}

void MapObjectItem::setTile( AbstractTile* tile )
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

Globals::TypeTile MapObjectItem::tileType() const
{
	return mTile ? Globals::InvalidTile : mTile->Type;
}
