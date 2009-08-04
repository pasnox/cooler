#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsRectItem>
#include <QMap>

#include "Globals.h"
#include "TilesManager.h"

class MapObjectItem;
class PlayerItem;

typedef QMap<uint, QString> TilesMapping;
typedef QMap<QPoint, MapObjectItem*> LayerMap;
typedef QMap<uint, LayerMap> LayersMap;

#define MAP_SPLIT_CHAR "|"

inline bool operator<( const QPoint& left, const QPoint& right )
{
	if ( left.x() == right.x() )
	{
		return left.y() < right.y();
	}
	else if ( left.x() < right.x() )
	{
		return true;
	}
	else if ( left.x() > right.x() )
	{
		return false;
	}
	
	return true;
}

class MapItem : public QGraphicsRectItem
{
public:
	enum { Type = Globals::MapItem };
	
	MapItem( QGraphicsItem* parent = 0 );
	virtual ~MapItem();
	
	virtual int type() const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	QString name() const;
	QSize size() const;
	LayersMap layers() const;
	LayerMap layer( uint id ) const;
	
	virtual void clear();
	bool load( const QString& fileName );
	
	QPoint canStrokeTo( PlayerItem* player, Globals::PlayerStroke stroke ) const;
	QPoint gridToPos( const QPoint& gridPos ) const;
	QPoint posToGrid( const QPoint& pos ) const;
	QPoint closestPos( const QPoint& pos ) const;
	
	mutable QRectF mDebugRect;
	mutable QLineF mDebugLine;

protected:
	TilesManager* mTiles;
	QString mName;
	QSize mSize;
	TilesMapping mMapping;
	LayersMap mLayers;
	
	AbstractTile* mappedTile( uint id ) const;
	QPoint objectPos( MapObjectItem* object ) const;
	MapObjectItem* nearestObject( const QPoint& strokePoint, Globals::PlayerStroke stroke, const QSet<MapObjectItem*>& objects ) const;
	void updateMap();
};

#endif // MAPITEM_H
