#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include <QObject>

#include "MapObjectItem.h"

class Player;

/*
class QTimer;
class MapItem;
class PlayerTile;
class PadSettings;
*/

class PlayerItem : public QObject, public MapObjectItem
{
	Q_OBJECT
	
public:
	PlayerItem( const Player* player, QGraphicsItem* parent );
	virtual ~PlayerItem();
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	virtual QRectF explosiveBoundingRect() const;
	virtual void setTile( AbstractTile* tile );
	
	Globals::PadStrokes padStrokes() const;
	void setPadStrokes( Globals::PadStrokes strokes );
	
	Globals::PadActions padActions() const;
	void setPadActions( Globals::PadActions actions );
	
	/*
	PadSettings* pad() const;
	void setPad( PadSettings* pad );
	
	void setPosAt( qreal step, const QPoint& pos );
	*/

protected:
	const Player* mPlayer;
	Globals::PadStrokes mStrokes;
	Globals::PadActions mActions;
	
	/*
	PlayerTile* mPlayerTile;
	PadSettings* mPad;
	QString mName;
//	BombInformations mBombInfos;
	Globals::PadStroke mStroke;
	qreal mStrokeStep;
	int mStrokeSpeed;
	QTimer* mStrokeTimer;
	
	void handleKeyboard( QKeyEvent* event );
	void handleAction( Globals::PadAction action );
	void dropBomb();

protected slots:
	void strokeTimer_timeout();
	*/
};

#endif // PLAYERITEM_H
