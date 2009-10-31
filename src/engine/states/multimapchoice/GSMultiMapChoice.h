#ifndef GSMULTIMAPCHOICE_H
#define GSMULTIMAPCHOICE_H

#include "AbstractGameState.h"
#include "GSMenuItem.h"

class QGraphicsLinearLayout;
class GSMapItem;

class GSMultiMapChoice : public AbstractGameState
{
public:
	static GSMultiMapChoice* instance();
	
	virtual void init( GameEngine* engine, const QSizeF& size );
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void handleEvents( GameEngine* game );
	virtual void update( GameEngine* game );
	virtual bool validateState( GameEngine* game ) const;

protected:
	static GSMultiMapChoice* mInstance;
	
	int mBackgroundValue;
	QPixmap mBackground;
	
	QGraphicsLinearLayout* mMainLayout;
	GSMenuItem* mTitle;
	GSMapItem* mMap;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSMULTIMAPCHOICE_H
