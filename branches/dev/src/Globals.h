#ifndef GLOBALS_H
#define GLOBALS_H

#include <QGraphicsItem>

namespace Globals
{
	enum TypeTile
	{
		InvalidTile,
		BlockTile,
		BoxTile,
		FloorTile,
		SkyTile,
		PlayerTile,
		BombTile
	};
	
	enum TypeItem
	{
		MapItem = QGraphicsItem::UserType +1,
		MapObjectItem
	};
	
	enum ObjectLayers
	{
		FirstLayer = 30000,
		LastLayer = 40000,
		PlayerLayer = 50000,
		BombLayer = PlayerLayer -1,
		SkyLayer = 60000
	};
	
	enum PlayerStroke
	{
		NoStroke = -1,
		DownStroke,
		RightStroke,
		LeftStroke,
		UpStroke
	};
};

#endif // GLOBALS_H
