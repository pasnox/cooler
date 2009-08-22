#ifndef GSMODE_H
#define GSMODE_H

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include <AbstractGameState.h>
#include <TilesManager.h>

#include "GSMenu.h"
#include "GSMenuItem.h"

class QGraphicsPixmapItem;

class GSMode : public AbstractGameState
{
public:
	static GSMode* instance();
	
	virtual void Init( const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );

protected:
	static GSMode* mInstance;
	int mBackgroundValue;
	QPixmap mBackground;
	QGraphicsPixmapItem* mTitle;
	QGraphicsPixmapItem* mFrame;
	QGraphicsPixmapItem* mBombers;
	GSMenu* mMenu;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSMODE_H
