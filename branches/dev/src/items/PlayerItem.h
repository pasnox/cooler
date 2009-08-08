#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include <QObject>

#include <MapObjectItem.h>
#include <TilesManager.h>

#include "BombInformations.h"

class QTimer;
class MapItem;
class PlayerTile;
class PadSettings;

class PlayerItem : public QObject, public MapObjectItem
{
	Q_OBJECT
	friend class GraphicsView;
	
public:
	PlayerItem( AbstractTile* tile, QGraphicsItem* parent );
	virtual ~PlayerItem();
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	virtual void setTile( AbstractTile* tile );
	
	PadSettings* pad() const;
	void setPad( PadSettings* pad );
	
	void setPosAt( qreal step, const QPoint& pos );

protected:
	PlayerTile* mPlayerTile;
	PadSettings* mPad;
	QString mName;
	BombInformations mBombInfos;
	Globals::PadStroke mStroke;
	qreal mStrokeStep;
	int mStrokeSpeed;
	QTimer* mStrokeTimer;
	
	void handleKeyboard( QKeyEvent* event );
	void handleAction( Globals::PadAction action );
	void dropBomb();

protected slots:
	void strokeTimer_timeout();
};

#endif // PLAYERITEM_H
