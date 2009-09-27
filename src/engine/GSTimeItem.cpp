#include "GSTimeItem.h"

GSTimeItem::GSTimeItem( Globals::TimeState state, int pixelSize )
	: GSGenericStateItem( Globals::TimeState1, Globals::TimeStateInfinity )
{
	mPenColor = QColor( Qt::black );
	mBrushColors.first = QColor( Qt::yellow );
	mBrushColors.second = QColor( Qt::red );
	setPixelSize( pixelSize );
	setTimeState( state );
}

void GSTimeItem::setState( int state )
{
	GSGenericStateItem::setState( state );
	
	switch ( (Globals::TimeState)mState )
	{
		case Globals::TimeState1:
			mText = "1:00";
			break;
		case Globals::TimeState2:
			mText = "2:00";
			break;
		case Globals::TimeState3:
			mText = "3:00";
			break;
		case Globals::TimeState4:
			mText = "4:00";
			break;
		case Globals::TimeState5:
			mText = "5:00";
			break;
		case Globals::TimeStateInfinity:
			mText = QString( QChar( 8734 ) );
			break;
	}
	
	updateCachePixmap();
}

void GSTimeItem::setTimeState( Globals::TimeState state )
{
	setState( state );
}

Globals::TimeState GSTimeItem::timeState() const
{
	return (Globals::TimeState)mState;
}

