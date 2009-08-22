#ifndef GSINTRODUCTION_H
#define GSINTRODUCTION_H

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "AbstractGameState.h"

class QGraphicsTextItem;

class GSIntroduction : public AbstractGameState
{
public:
	static GSIntroduction* instance();
	
	virtual void Init( const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );
	virtual void Draw( GameEngine* game );

protected:
	static GSIntroduction* mInstance;
	QPixmap mGameScreen;
	int mPressStartTimer;
	QGraphicsTextItem* mTextItem;
	
	virtual void timerEvent( QTimerEvent* event );
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSINTRODUCTION_H
