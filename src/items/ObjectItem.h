#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <AbstractItem.h>
#include <TilesManager.h>

class ObjectItem : public AbstractItem
{
public:
	enum { Type = Globals::ObjectItem };
	
	ObjectItem( AbstractTile* tile = 0, QGraphicsItem* parent = 0 );
	virtual ~ObjectItem();
	
	virtual int type() const;
};

#endif // OBJECTITEM_H
