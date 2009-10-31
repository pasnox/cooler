#ifndef GSINTRODUCTION_H
#define GSINTRODUCTION_H

#include "AbstractGameState.h"

#include <QTime>

class GSMenuItem;

class GSIntroduction : public AbstractGameState
{
public:
	static GSIntroduction* instance();
	
	virtual void init( GameEngine* engine, const QSizeF& size );
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void handleEvents( GameEngine* game );
	virtual void update( GameEngine* game );
	virtual bool validateState( GameEngine* game ) const;

protected:
	static GSIntroduction* mInstance;
	QPixmap mGameScreen;
	QTime mElapsedTime;
	GSMenuItem* mPressAnyKey;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSINTRODUCTION_H
