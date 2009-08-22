#ifndef GSPADSETTINGS_H
#define GSPADSETTINGS_H

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include <AbstractGameState.h>
#include <TilesManager.h>

#include "GSMenu.h"
#include "GSMenuItem.h"

class QGraphicsPixmapItem;

class GSPadSettings : public AbstractGameState
{
public:
	static GSPadSettings* instance( PadSettingsMap* pads );
	
	virtual void Init( const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );
	virtual void Draw( GameEngine* game );

protected:
	static GSPadSettings* mInstance;
	PadSettingsMap* mPads;
	
	TilesMap mTiles;
	QPixmap mBackground;
	int mBackgroundValue;
	int mBackgroundTimer;
	int mUpdateTimer;
	uint mCurrentPadIndex;
	
	QGraphicsLinearLayout* mMainLayout;
	GSMenuItem* mTitle;
	QGraphicsLinearLayout* mMenuLayout;
	GSMenu* mLabelsMenu;
	GSMenu* mKeysMenu;
	GSMenu* mFormMenu;
	
	GSPadSettings( PadSettingsMap* pads );
	
	void loadPadSettings( int index );
	
	virtual void timerEvent( QTimerEvent* event );
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSPADSETTINGS_H
