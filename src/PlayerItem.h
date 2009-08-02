#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

#include "Globals.h"
#include "TilesManager.h"
#include "BombInformations.h"

class QTimer;
class MapItem;

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

class PlayerItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	friend class GraphicsView;
	
public:
	enum { Type = Globals::PlayerItem };
	
	PlayerItem( const PlayerTile& playerTile, QObject* parent = 0 );
	virtual ~PlayerItem();
	
	virtual int type() const;
	virtual QRectF boundingRect() const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	MapItem* map() const;
	QRect bodyBoundingRect() const;
	
	const PadSettings& padSettings() const;
	void setPosAt( qreal step, const QPoint& pos );

protected:
	QString mName;
	BombInformations mBombInfos;
	PadSettings mPad;
	PlayerTile mPlayerTile;
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
