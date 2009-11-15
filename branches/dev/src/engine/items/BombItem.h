#ifndef BOMBITEM_H
#define BOMBITEM_H

#include <QObject>

#include "MapObjectItem.h"
#include "BombSettings.h"

class QTimer;

class BombItem : public QObject, public MapObjectItem
{
	Q_OBJECT
	
public:
	BombItem( AbstractTile* tile = 0, QGraphicsItem* parent = 0, const BombSettings& settings = BombSettings() );
	virtual ~BombItem();
	
	const BombSettings& settings() const;
	void setSettings( const BombSettings& settings );
	
	void start();

protected:
	BombSettings mSettings;
	
	int mAnimationStep;
	QTimer* mAnimationTimer;
	QTimer* mExplodeTimer;
	
	void setTileStep( int step );

protected slots:
	void animationTimer_timeout();
	void explodeTimer_timeout();
};

#endif // BOMBITEM_H
