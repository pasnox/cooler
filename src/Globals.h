#ifndef GLOBALS_H
#define GLOBALS_H

#include <QGraphicsItem>
#include <QDir>

namespace Globals
{
	const uint MaxPlayers = 5;
	const QSize TilesSize = QSize( 64, 64 );
	
	enum TypeTile
	{
		InvalidTile,
		BlockTile,
		BombTile,
		BombExplosionTile,
		BonusTile,
		BoxTile,
		FloorTile,
		GameScreenTile,
		PlayerTile,
		PlayerExplosionTile,
		SkyTile,
		TextTile
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
	
	enum PlayerState
	{
		PlayerStateNo = -1,
		PlayerStateOff,
		PlayerStateHuman,
		PlayerStateComputer
	};
	
	QString relativeFilePath( const QString& path, const QString& fn );
	QString relativeFilePath( const QFileInfo& path, const QFileInfo& fn );
	QFileInfoList getFiles( QDir& path, const QStringList& filters = QStringList() );
	QFileInfoList getFiles( const QString& path, const QStringList& filters = QStringList() );
	
	QString keyToString( int key );
};

#endif // GLOBALS_H
