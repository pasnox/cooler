#ifndef GSMULTIPLAYERCHOICE_H
#define GSMULTIPLAYERCHOICE_H

#include "AbstractGameState.h"
#include "GSMenu.h"

class GSMultiPlayerChoice : public AbstractGameState
{
public:
	static GSMultiPlayerChoice* instance();
	
	virtual void init( GameEngine* engine, const QSizeF& size );
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void handleEvents( GameEngine* game );
	virtual void update( GameEngine* game );
	virtual bool validateState( GameEngine* game ) const;

protected:
	static GSMultiPlayerChoice* mInstance;
	
	TilesMap mFacesTiles;
	
	int mBackgroundValue;
	QPixmap mBackground;
	
	QGraphicsLinearLayout* mMainLayout;
	GSMenuItem* mTitle;
	GSMenu* mFacesMenu;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSMULTIPLAYERCHOICE_H
