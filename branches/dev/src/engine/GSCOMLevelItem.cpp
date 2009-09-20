#include "GSCOMLevelItem.h"

GSCOMLevelItem::GSCOMLevelItem( Globals::COMLevel level, int pixelSize )
	: GSGenericStateItem( Globals::EasyLevel, Globals::HardLevel )
{
	setPixelSize( pixelSize );
	setLevel( level );
}

void GSCOMLevelItem::setState( int state )
{
	GSGenericStateItem::setState( state );
	
	switch ( (Globals::COMLevel)mState )
	{
		case Globals::EasyLevel:
			mText = tr( "Easy" );
			mPenColor = QColor( Qt::black );
			mBrushColors.first = QColor( Qt::blue );
			mBrushColors.second = mBrushColors.first;
			break;
		case Globals::MediumLevel:
			mText = tr( "Medium" );
			mPenColor = QColor( Qt::black );
			mBrushColors.first = QColor( Qt::yellow );
			mBrushColors.second = mBrushColors.first;
			break;
		case Globals::HardLevel:
			mText = tr( "Hard" );
			mPenColor = QColor( Qt::black );
			mBrushColors.first = QColor( Qt::red );
			mBrushColors.second = mBrushColors.first;
			break;
	}
	
	updateCachePixmap();
}

void GSCOMLevelItem::setLevel( Globals::COMLevel level )
{
	setState( level );
}

Globals::COMLevel GSCOMLevelItem::level() const
{
	return (Globals::COMLevel)mState;
}

