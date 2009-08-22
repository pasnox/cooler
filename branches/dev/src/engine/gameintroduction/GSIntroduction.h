#ifndef GSINTRODUCTION_H
#define GSINTRODUCTION_H

#include "AbstractGameState.h"

#include <QTime>

class GSMenuItem;

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

protected:
	static GSIntroduction* mInstance;
	QPixmap mGameScreen;
	QTime mElapsedTime;
	GSMenuItem* mPressAnyKey;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSINTRODUCTION_H
