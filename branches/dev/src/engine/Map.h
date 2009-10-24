#ifndef MAP_H
#define MAP_H

#include <QFileInfo>

#include "Globals.h"
#include "TilesManager.h"

#define MAP_SPLIT_CHAR "|"

typedef QMap<uint, QString> TilesMapping;
typedef QMap<QPoint, AbstractTile*> LayerMap;
typedef QMap<uint, LayerMap> LayersMap;
typedef QList<QPoint> PlayersPosition;

class Map
{
	friend class MapItem;
	
public:
	Map( const QFileInfo& fileName );
	virtual ~Map();

	Map( const Map& other );
	Map& operator=( const Map& other );
	bool operator==( const Map& other ) const;
	bool operator!=( const Map& other ) const;
	
	bool isValid() const;
	
	QString name() const;
	QSize size() const;
	PlayersPosition playersPosition() const;
	LayersMap layers() const;
	LayerMap layer( uint id ) const;

protected:
	QFileInfo mFileName;
	TilesManager* mTiles;
	QString mName;
	QSize mSize;
	PlayersPosition mPlayersPosition;
	TilesMapping mMapping;
	LayersMap mLayers;
	
	AbstractTile* mappedTile( uint id ) const;
	void clear();
	void load();
};

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

#endif // MAP_H
