#include "GSMenu.h"

#include <QGraphicsLinearLayout>

#include <QDebug>

GSMenu::GSMenu( Qt::Orientation orientation, QGraphicsItem* parent )
	: QGraphicsWidget( parent )
{
	mSelectedItem = 0;
	mLayout = new QGraphicsLinearLayout( orientation, this );
	mLayout->setContentsMargins( 0, 0, 0, 0 );
	mLayout->setSpacing( 10 );
}

GSMenu::~GSMenu()
{
	qDeleteAll( mItems );
	mItems.clear();
}

void GSMenu::setSpacing( int spacing )
{
	mLayout->setSpacing( spacing );
}

int GSMenu::spacing() const
{
	return mLayout->spacing();
}

void GSMenu::setSelectedItem( GSMenuItem* item )
{
	foreach ( GSMenuItem* it, mItems )
	{
		it->setActive( it == item );
	}
	
	mSelectedItem = mItems.contains( item ) ? item : 0;
}

GSMenuItem* GSMenu::selectedItem() const
{
	return mSelectedItem;
}

void GSMenu::setSelectedIndex( int index )
{
	setSelectedItem( item( index ) );
}

int GSMenu::selectedIndex() const
{
	return mItems.indexOf( selectedItem() );
}

void GSMenu::selectPreviousItem()
{
	if ( mItems.isEmpty() )
	{
		return;
	}
	
	if ( !mSelectedItem )
	{
		setSelectedItem( mItems.first() );
		return;
	}
	
	int index = mItems.indexOf( mSelectedItem );
	index--;
	
	if ( index < 0 )
	{
		index = mItems.count() -1;
	}
	
	setSelectedItem( mItems.at( index ) );
}

void GSMenu::selectNextItem()
{
	if ( mItems.isEmpty() )
	{
		return;
	}
	
	if ( !mSelectedItem )
	{
		setSelectedItem( mItems.first() );
		return;
	}
	
	int index = mItems.indexOf( mSelectedItem );
	index++;
	
	if ( index >= mItems.count() )
	{
		index = 0;
	}
	
	setSelectedItem( mItems.at( index ) );
}

int GSMenu::count() const
{
	return mItems.count();
}

const GSMenuItemList& GSMenu::items() const
{
	return mItems;
}

GSMenuItem* GSMenu::item( int index ) const
{
	return mItems.value( index );
}

void GSMenu::addItem( GSMenuItem* item )
{
	mLayout->addItem( item );
	mItems << item;
	updateGeometry();
}

GSMenuItem* GSMenu::addTextItem( const QPixmap& icon, const QString& text, Qt::Alignment align, int pixelSize )
{
	GSMenuItem* item = new GSMenuItem( icon, text, align, pixelSize );
	addItem( item );
	return item;
}

GSMenuItem* GSMenu::addTextItem( const QString& text, Qt::Alignment align, int pixelSize )
{
	return addTextItem( QPixmap(), text, align, pixelSize );
}

void GSMenu::removeItem( GSMenuItem* item )
{
	mLayout->removeItem( item );
	mItems.removeAll( item );
}
