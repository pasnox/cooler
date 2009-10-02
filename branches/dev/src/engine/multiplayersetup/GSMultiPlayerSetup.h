#ifndef GSMULTIPLAYERSETUP_H
#define GSMULTIPLAYERSETUP_H

#include <AbstractGameState.h>

#include "GSMenu.h"

class QGraphicsPixmapItem;

class GSMultiPlayerSetup : public AbstractGameState
{
public:
	static GSMultiPlayerSetup* instance();
	
	virtual void Init( GameEngine* engine, const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );
	virtual bool validateState( GameEngine* game ) const;

protected:
	static GSMultiPlayerSetup* mInstance;
	
	int mBackgroundValue;
	QPixmap mBackground;
	
	QGraphicsLinearLayout* mMainLayout;
	GSMenuItem* mTitle;
	QGraphicsLinearLayout* mMenuLayout;
	GSMenu* mPlayersMenu;
	GSMenu* mStatesMenu;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSMULTIPLAYERSETUP_H
