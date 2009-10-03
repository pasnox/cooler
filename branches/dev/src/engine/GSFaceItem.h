#ifndef GSFACEITEM_H
#define GSFACEITEM_H

#include "GSGenericStateItem.h"
#include "AbstractTile.h"

class GSFaceItem : public GSGenericStateItem
{
public:
	GSFaceItem( const TilesMap& tiles, uint playerId );
	virtual ~GSFaceItem();

protected:
	const TilesMap* mTiles;
	uint mPlayerId;
	
	virtual void updateCachePixmap();
	
	virtual QSizeF sizeHint( Qt::SizeHint which, const QSizeF& constraint = QSizeF() ) const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSFACEITEM_H
