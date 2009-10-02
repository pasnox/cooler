#ifndef GSPADSETTINGS_H
#define GSPADSETTINGS_H

#include <AbstractGameState.h>

#include "GSMenu.h"

class QGraphicsPixmapItem;

class GSPadSettings : public AbstractGameState
{
public:
	static GSPadSettings* instance( const PadSettingsList& pads );
	
	virtual void Init( GameEngine* engine, const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );
	virtual bool validateState( GameEngine* game ) const;

protected:
	static GSPadSettings* mInstance;
	PadSettingsList mPads;
	uint mCurrentPadIndex;
	bool mIsWaitingInput;
	QColor mActiveColor;
	QColor mActiveGoodColor;
	QColor mActiveBadColor;
	GSMenuItem::BrushColors mKeyBrushColors;
	
	int mBackgroundValue;
	QPixmap mBackground;
	
	QGraphicsLinearLayout* mMainLayout;
	GSMenuItem* mTitle;
	QGraphicsLinearLayout* mMenuLayout;
	GSMenu* mLabelsMenu;
	GSMenu* mKeysMenu;
	GSMenu* mFormMenu;
	
	GSPadSettings( const PadSettingsList& pads );
	
	void loadPadSettings( int index );
	bool canChangeStrokeKey( Globals::PadStroke stroke, int key );
	bool canChangeActionKey( Globals::PadAction action, int key );
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSPADSETTINGS_H
