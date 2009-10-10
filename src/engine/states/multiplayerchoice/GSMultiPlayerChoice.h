#ifndef GSMULTIPLAYERCHOICE_H
#define GSMULTIPLAYERCHOICE_H

#include "AbstractGameState.h"
#include "GSMenu.h"

class GSMultiPlayerChoice : public AbstractGameState
{
public:
	static GSMultiPlayerChoice* instance();
	
	virtual void Init( GameEngine* engine, const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );
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
