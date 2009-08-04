#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include <QObject>

#include <MapObjectItem.h>
#include <TilesManager.h>

#include "BombInformations.h"

class QTimer;
class MapItem;
class PlayerTile;

struct PadSettings
{
	enum Action
	{
		Action1,
		Action2,
		Action3,
		Action4
	};
	
	QMap<int, Globals::PlayerStroke> StrokeMapping;
	QMap<int, PadSettings::Action> ActionMapping;
};

class PlayerItem : public QObject, public MapObjectItem
{
	Q_OBJECT
	friend class GraphicsView;
	
public:
	PlayerItem( AbstractTile* tile, QGraphicsItem* parent );
	virtual ~PlayerItem();
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	virtual void setTile( AbstractTile* tile );
	
	QRect bodyBoundingRect() const;
	
	const PadSettings& padSettings() const;
	void setPosAt( qreal step, const QPoint& pos );

protected:
	PlayerTile* mPlayerTile;
	QString mName;
	BombInformations mBombInfos;
	PadSettings mPad;
	Globals::PlayerStroke mStroke;
	qreal mStrokeStep;
	int mStrokeSpeed;
	QTimer* mStrokeTimer;
	
	void handleKeyboard( QKeyEvent* event );
	void handleAction( PadSettings::Action action );
	void dropBomb();

protected slots:
	void strokeTimer_timeout();
};

#endif // PLAYERITEM_H
