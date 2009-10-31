#ifndef GSMODE_H
#define GSMODE_H

#include "AbstractGameState.h"
#include "GSMenu.h"

class QGraphicsPixmapItem;

class GSMode : public AbstractGameState
{
public:
	static GSMode* instance();
	
	virtual void init( GameEngine* engine, const QSizeF& size );
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void handleEvents( GameEngine* game );
	virtual void update( GameEngine* game );
	virtual bool validateState( GameEngine* game ) const;

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
