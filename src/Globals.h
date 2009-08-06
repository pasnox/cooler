#ifndef GLOBALS_H
#define GLOBALS_H

#include <QGraphicsItem>

namespace Globals
{
	const uint MaxPlayers = 4;
	
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
		MapItem = QGraphicsItem::UserType,
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
	
	enum PadStroke
	{
		PadStrokeNo = -1,
		PadStrokeDown,
		PadStrokeRight,
		PadStrokeLeft,
		PadStrokeUp
	};
	
	enum PadAction
	{
		PadActionNo = -1,
		PadAction1,
		PadAction2,
		PadAction3,
		PadAction4
	};
};

#endif // GLOBALS_H
