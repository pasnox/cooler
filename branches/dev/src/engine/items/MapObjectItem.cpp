#include "MapObjectItem.h"
#include "MapItem.h"

MapObjectItem::MapObjectItem( AbstractTile* tile, QGraphicsItem* parent )
	: QGraphicsPixmapItem( parent )
{
	setTile( tile );
}

int MapObjectItem::type() const
{
	return Type;
}

QRectF MapObjectItem::boundingRect() const
{
	return QRectF( QPointF( 0, 0 ), mTile ? mTile->tile( 0 ).size() : QSize() );
}

void MapObjectItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QGraphicsPixmapItem::paint( painter, option, widget );
}

bool MapObjectItem::isValid() const
{
	return tileType() != Globals::InvalidTile;
}

bool MapObjectItem::isWalkable() const
{
	switch ( tileType() )
	{
		case Globals::FloorTile:
		case Globals::SkyTile:
		case Globals::PlayerTile:
		case Globals::BombExplosionTile:
		case Globals::BonusTile:
		case Globals::PlayerExplosionTile:
		case Globals::TextTile:
			return true;
			break;
		case Globals::BlockTile:
		case Globals::BoxTile:
		case Globals::BombTile:
		case Globals::GameScreenTile:
			break;
		case Globals::InvalidTile:
			Q_ASSERT( 0 );
			break;
	}
	
	return false;
}

bool MapObjectItem::isExplosive() const
{
	switch ( tileType() )
	{
		case Globals::PlayerTile:
		case Globals::BoxTile:
		case Globals::BombTile:
		case Globals::BonusTile:
			return true;
			break;
		case Globals::FloorTile:
		case Globals::SkyTile:
		case Globals::BlockTile:
		case Globals::BombExplosionTile:
		case Globals::PlayerExplosionTile:
		case Globals::TextTile:
		case Globals::GameScreenTile:
			break;
		case Globals::InvalidTile:
			Q_ASSERT( 0 );
			break;
	}
	
	return false;
}

QRectF MapObjectItem::explosiveBoundingRect() const
{
	return boundingRect();
}

MapItem* MapObjectItem::map() const
{
	return qgraphicsitem_cast<MapItem*>( parentItem() );
}

Globals::TypeTile MapObjectItem::tileType() const
{
	return mTile ? mTile->Type : Globals::InvalidTile;
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
