#ifndef BOMBITEM_H
#define BOMBITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

#include "Globals.h"
#include "BombTile.h"

class QTimer;

class BombItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	
public:
	enum { Type = Globals::BombItem };
	
	BombItem( const BombTile& bombTile, QObject* parent = 0 );
	virtual ~BombItem();
	
	virtual int type() const;
	virtual QRectF boundingRect() const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );

protected:
	BombTile mBombTile;
	int mAnimationStep;
	QTimer* mAnimationTimer;
	QTimer* mExplodeTimer;
	
	void setTileStep( int step );

protected slots:
	void animationTimer_timeout();
	void explodeTimer_timeout();
};

#endif // BOMBITEM_H
