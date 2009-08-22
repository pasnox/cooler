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
	static GSPadSettings* instance( const PadSettingsMap& pads );
	
	virtual void Init( const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );

protected:
	static GSPadSettings* mInstance;
	PadSettingsMap mPads;
	uint mCurrentPadIndex;
	bool mIsWaitingInput;
	QColor mActiveColor;
	QColor mActiveGoodColor;
	QColor mActiveBadColor;
	GSMenuItem::BrushColors mKeyBrushColors;
	
	TilesMap mTiles;
	QPixmap mBackground;
	int mBackgroundValue;
	int mBackgroundTimer;
	int mUpdateTimer;
	
	QGraphicsLinearLayout* mMainLayout;
	GSMenuItem* mTitle;
	QGraphicsLinearLayout* mMenuLayout;
	GSMenu* mLabelsMenu;
	GSMenu* mKeysMenu;
	GSMenu* mFormMenu;
	
	GSPadSettings( const PadSettingsMap& pads );
	
	void loadPadSettings( int index );
	bool canChangeStrokeKey( Globals::PadStroke stroke, int key );
	bool canChangeActionKey( Globals::PadAction action, int key );
	
	virtual void timerEvent( QTimerEvent* event );
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSPADSETTINGS_H
