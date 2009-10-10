#ifndef GSMAPITEM_H
#define GSMAPITEM_H

#include "GSGenericStateItem.h"

#include <QFileInfo>

class GSMapItem : public GSGenericStateItem
{
public:
	GSMapItem( const QFileInfoList& maps );
	virtual ~GSMapItem();
	
	void setMap( const QFileInfo& map );
	const QFileInfo& map() const;

protected:
	const QFileInfoList* mMaps;
	
	virtual void updateCachePixmap();
	
	virtual QSizeF sizeHint( Qt::SizeHint which, const QSizeF& constraint = QSizeF() ) const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSMAPITEM_H
