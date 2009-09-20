#include "GSOnOffItem.h"

GSOnOffItem::GSOnOffItem( Globals::InterruptState state, int pixelSize )
	: GSGenericStateItem( Globals::InterruptStateOn, Globals::InterruptStateOff )
{
	setPixelSize( pixelSize );
	setInterruptState( state );
}

void GSOnOffItem::setState( int state )
{
	GSGenericStateItem::setState( state );
	
	switch ( (Globals::InterruptState)mState )
	{
		case Globals::InterruptStateOn:
			mText = tr( "ON" );
			mPenColor = QColor( Qt::black );
			mBrushColors.first = QColor( Qt::blue );
			mBrushColors.second = mBrushColors.first;
			break;
		case Globals::InterruptStateOff:
			mText = tr( "OFF" );
			mPenColor = QColor( Qt::black );
			mBrushColors.first = QColor( Qt::red );
			mBrushColors.second = mBrushColors.first;
			break;
	}
	
	updateCachePixmap();
}

void GSOnOffItem::setInterruptState( Globals::InterruptState state )
{
	setState( state );
}

Globals::InterruptState GSOnOffItem::interruptState() const
{
	return (Globals::InterruptState)mState;
}

