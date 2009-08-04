#ifndef BOMBITEM_H
#define BOMBITEM_H

#include <QObject>

#include <AbstractItem.h>

class QTimer;

class BombItem : public QObject, public AbstractItem
{
	Q_OBJECT
	
public:
	enum { Type = Globals::BombItem };
	
	BombItem( AbstractTile* tile = 0, QGraphicsItem* parent = 0 );
	virtual ~BombItem();
	
	virtual int type() const;

protected:
	int mAnimationStep;
	QTimer* mAnimationTimer;
	QTimer* mExplodeTimer;
	
	void setTileStep( int step );

protected slots:
	void animationTimer_timeout();
	void explodeTimer_timeout();
};

#endif // BOMBITEM_H
