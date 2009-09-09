#include "GSStateItem.h"

GSStateItem::GSStateItem( Globals::PlayerState state, int pixelSize )
	: GSMenuItem()
{
	mActiveColor = QColor( Qt::transparent );
	mActiveDisabledColor = mActiveColor;
	mState = Globals::PlayerStateNo;
	
	setPixelSize( pixelSize );
	setState( state );
}

void GSStateItem::setState( Globals::PlayerState state )
{
	if ( mState == state )
		return;
	
	mState = state;
	
	switch ( mState )
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
		case Globals::PlayerStateLast:
			Q_ASSERT( 0 );
			break;
	}
	
	updateCachePixmap();
}

const Globals::PlayerState& GSStateItem::state() const
{
	return mState;
}

void GSStateItem::previousState()
{
	int i = mState -1;
	
	if ( i == Globals::PlayerStateNo )
	{
		i++;
	}
	
	setState( (Globals::PlayerState)i );
}

void GSStateItem::nextState()
{
	int i = mState +1;
	
	if ( i == Globals::PlayerStateLast )
	{
		i--;
	}
	
	setState( (Globals::PlayerState)i );
}
