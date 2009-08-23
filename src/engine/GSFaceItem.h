#ifndef GSFACEITEM_H
#define GSFACEITEM_H

#include "GSMenuItem.h"

class AbstractTile;

class GSFaceItem : public GSMenuItem
{
public:
	GSFaceItem( AbstractTile* tile, uint id );
	virtual ~GSFaceItem();
	
	virtual void updateCachePixmap();

protected:
	AbstractTile* mTile;
	uint mId;
	
	virtual QSizeF sizeHint( Qt::SizeHint which, const QSizeF& constraint = QSizeF() ) const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSFACEITEM_H
