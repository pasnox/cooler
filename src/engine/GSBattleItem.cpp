#include "GSBattleItem.h"

GSBattleItem::GSBattleItem( int state, int pixelSize )
	: GSGenericStateItem( 1, 5 )
{
	mPenColor = QColor( Qt::black );
	mBrushColors.first = QColor( Qt::yellow );
	mBrushColors.second = mBrushColors.first;
	
	setPixelSize( pixelSize );
	setState( state );
}

void GSBattleItem::setState( int state )
{
	GSGenericStateItem::setState( state );
	mText.fill( 'X', state );
	updateCachePixmap();
}
