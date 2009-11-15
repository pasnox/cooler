#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include <QObject>

#include "MapObjectItem.h"
#include "BombSettings.h"

class Player;
class PlayerTile;

struct PlayerProperties
{
	PlayerProperties()
	{
		mPlayer = 0;
		mPlayerTile = 0;
		mStrokes = Globals::PadStrokeNo;
		mActions = Globals::PadActionNo;
		mFrameStep = 0;
		mSpeed = 2;
	}
	
	const Player* mPlayer;
	PlayerTile* mPlayerTile;
	Globals::PadStrokes mStrokes;
	Globals::PadActions mActions;
	qreal mFrameStep;
	qreal mSpeed;
	BombSettings mBombSettings;
};

class PlayerItem : public QObject, public MapObjectItem
{
	Q_OBJECT
	
public:
	PlayerItem( const Player* player, QGraphicsItem* parent );
	virtual ~PlayerItem();
	
	virtual void advance( int phase );
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	virtual QRectF explosiveBoundingRect() const;
	virtual QPointF explosiveBoundingRectCenter() const;
	virtual void setTile( AbstractTile* tile );
	
	const PlayerProperties& properties() const;
	
	Globals::PadStrokes padStrokes() const;
	void setPadStrokes( Globals::PadStrokes strokes );
	
	Globals::PadActions padActions() const;
	void setPadActions( Globals::PadActions actions );
	
	void triggerAction( Globals::PadAction action );

protected:
	PlayerProperties mProperties;
	
	void dropBomb();
};

#endif // PLAYERITEM_H
