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
		PadStrokeNo = 0x0,
		PadStrokeDown = 0x1,
		PadStrokeRight = 0x2,
		PadStrokeLeft = 0x4,
		PadStrokeUp = 0x8
	};
	
	Q_DECLARE_FLAGS( PadStrokes, PadStroke )
	
	enum PadAction
	{
		PadActionNo = 0x0,
		PadAction1 = 0x1,
		PadAction2 = 0x2,
		PadAction3 = 0x4,
		PadAction4 = 0x8
	};
	
	Q_DECLARE_FLAGS( PadActions, PadAction )
	
	enum PlayerState
	{
		PlayerStateNo = -1,
		PlayerStateOff,
		//PlayerStateComputer,
		PlayerStateHuman,
	};
	
	enum COMLevel
	{
		EasyLevel = 1,
		MediumLevel,
		HardLevel
	};
	
	enum InterruptState
	{
		InterruptStateOn,
		InterruptStateOff
	};
	
	enum TimeState
	{
		TimeState1,
		TimeState2,
		TimeState3,
		TimeState4,
		TimeState5,
		TimeStateInfinity
	};
	
	QString relativeFilePath( const QString& path, const QString& fn );
	QString relativeFilePath( const QFileInfo& path, const QFileInfo& fn );
	QFileInfoList getFiles( QDir& path, const QStringList& filters = QStringList() );
	QFileInfoList getFiles( const QString& path, const QStringList& filters = QStringList() );
	
	QString keyToString( int key );
	
	QImage toGrayscale( const QImage& srcImage, bool keepAlpha = true );
};

Q_DECLARE_OPERATORS_FOR_FLAGS( Globals::PadStrokes )
Q_DECLARE_OPERATORS_FOR_FLAGS( Globals::PadActions )

#endif // GLOBALS_H
