#ifndef GSMENU_H
#define GSMENU_H

#include <QGraphicsWidget>

#include "GSMenuItem.h"

class QGraphicsLinearLayout;

class GSMenu : public QGraphicsWidget
{
public:
	GSMenu( Qt::Orientation orientation = Qt::Vertical, QGraphicsItem* parent = 0 );
	virtual ~GSMenu();
	
	void setSpacing( int spacing );
	int spacing() const;
	
	void setSelectedItem( GSMenuItem* item );
	GSMenuItem* selectedItem() const;
	
	void setSelectedIndex( int index );
	int selectedIndex() const;
	
	void selectPreviousItem();
	void selectNextItem();
	
	int count() const;
	
	GSMenuItemList items() const;
	GSMenuItem* item( int index ) const;
	void addItem( GSMenuItem* item );
	GSMenuItem* addItem( const QString& text, Qt::Alignment align = Qt::AlignCenter, int pixelSize = 20 );
	void removeItem( GSMenuItem* item );

protected:
	QGraphicsLinearLayout* mLayout;
	GSMenuItemList mItems;
	GSMenuItem* mSelectedItem;
};

#endif // GSMENU_H
