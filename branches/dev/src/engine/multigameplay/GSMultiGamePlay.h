#ifndef GSMULTIGAMEPLAY_H
#define GSMULTIGAMEPLAY_H

#include "AbstractGameState.h"
#include "GSMenu.h"

class QGraphicsPixmapItem;

class GSMultiGamePlay : public AbstractGameState
{
public:
	static GSMultiGamePlay* instance();
	
	virtual void Init( GameEngine* engine, const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );
	virtual bool validateState( GameEngine* game ) const;

protected:
	static GSMultiGamePlay* mInstance;
	
	int mBackgroundValue;
	QPixmap mBackground;
	
	QGraphicsLinearLayout* mMainLayout;
	GSMenuItem* mTitle;
	QGraphicsLinearLayout* mMenuLayout;
	GSMenu* mOptionsMenu;
	GSMenu* mStatesMenu;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSMULTIGAMEPLAY_H
