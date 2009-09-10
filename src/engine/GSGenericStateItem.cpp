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
}

void GSGenericStateItem::setState( int state )
{
	if ( mState == state )
		return;
	
	mState = state;
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
		i++;
	}
	
	setState( i );
}

void GSGenericStateItem::nextState()
{
	int i = mState +1;
	
	if ( i > mMax )
	{
		i--;
	}
	
	setState( i );
}
