#include "GSStateItem.h"

GSStateItem::GSStateItem( Globals::PlayerState state, int pixelSize )
	: GSGenericStateItem( Globals::PlayerStateOff, Globals::PlayerStateHuman )
{
	setPixelSize( pixelSize );
	setPlayerState( state );
}

void GSStateItem::setState( int state )
{
	GSGenericStateItem::setState( state );
	
	switch ( (Globals::PlayerState)mState )
	{
		case Globals::PlayerStateOff:
			mText = tr( "OFF" );
			mPenColor = QColor( Qt::black );
			mBrushColors.first = QColor( Qt::green );
			mBrushColors.second = mBrushColors.first;
			break;
		case Globals::PlayerStateHuman:
			mText = tr( "MAN" );
			mPenColor = QColor( Qt::black );
			mBrushColors.first = QColor( Qt::blue );
			mBrushColors.second = mBrushColors.first;
			break;
		/*
		case Globals::PlayerStateComputer:
			mText = tr( "COM" );
			mPenColor = QColor( Qt::black );
			mBrushColors.first = QColor( Qt::red );
			mBrushColors.second = mBrushColors.first;
			break;
		*/
		case Globals::PlayerStateNo:
			Q_ASSERT( 0 );
			break;
	}
	
	updateCachePixmap();
}

void GSStateItem::setPlayerState( Globals::PlayerState state )
{
	setState( state );
}

Globals::PlayerState GSStateItem::playerState() const
{
	return (Globals::PlayerState)mState;
}

