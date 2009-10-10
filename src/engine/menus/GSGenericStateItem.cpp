#include "GSGenericStateItem.h"

GSGenericStateItem::GSGenericStateItem( int min, int max, int state, int pixelSize )
	: GSMenuItem()
{
	init();
	
	mMin = min;
	mMax = max;
	
	setPixelSize( pixelSize );
	setState( state );
}

GSGenericStateItem::GSGenericStateItem( int min, int max )
	: GSMenuItem()
{
	init();
	
	mMin = min;
	mMax = max;
}

void GSGenericStateItem::init()
{
	mActiveColor = QColor( Qt::transparent );
	mActiveDisabledColor = mActiveColor;
	mMin = 0;
	mMax = 0;
	mState = 0;
	mWrapEnabled = false;
}

void GSGenericStateItem::setWrapEnabled( bool enabled )
{
	mWrapEnabled = enabled;
}

const bool& GSGenericStateItem::wrapEnabled() const
{
	return mWrapEnabled;
}

void GSGenericStateItem::setState( int state )
{
	if ( mState == state )
		return;
	
	mState = state;
	mText = QString::number( mState );
	Q_ASSERT( mState >= mMin && mState <= mMax );
	
	updateCachePixmap();
}

const int& GSGenericStateItem::state() const
{
	return mState;
}

void GSGenericStateItem::previousState()
{
	int i = mState -1;
	
	if ( i < mMin )
	{
		if ( mWrapEnabled )
			i = mMax;
		else
			i++;
	}
	
	setState( i );
}

void GSGenericStateItem::nextState()
{
	int i = mState +1;
	
	if ( i > mMax )
	{
		if ( mWrapEnabled )
			i = mMin;
		else
			i--;
	}
	
	setState( i );
}
