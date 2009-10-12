#include "TreeWidgetLayers.h"
#include "Globals.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

TreeWidgetLayers::TreeWidgetLayers( QWidget* parent )
	: QTreeWidget( parent )
{
	connect( this, SIGNAL( itemSelectionChanged() ), this, SLOT( _q_itemSelectionChanged() ) );
}

TreeWidgetLayers::~TreeWidgetLayers()
{
}

QList<uint> TreeWidgetLayers::layers() const
{
	QList<uint> list;
	
	for ( int i = 0; i < topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* item = topLevelItem( i );
		const int layer = item->data( 0, Qt::UserRole ).toInt();
		
		if ( layer < 0 )
		{
			continue;
		}
		
		list << layer;
	}
	
	return list;
}

void TreeWidgetLayers::setLayers( const QList<uint> layers )
{
	clear();
	
	foreach ( const uint layer, layers )
	{
		QTreeWidgetItem* item = new TreeWidgetLayersItem( this );
		item->setText( 0, QString::number( layer ) );
		item->setData( 0, Qt::UserRole, layer );
	}
	
	updateItems();
}

void TreeWidgetLayers::addLayer()
{
	QList<uint> allLayers = layers();
	
	if ( !allLayers.isEmpty() && allLayers.last() == Globals::SkyLayer )
	{
		allLayers.removeLast();
	}
	
	bool ok;
	const int lastLayer = allLayers.isEmpty() ? (int)Globals::FirstLayer : allLayers.last();
	const int layer = QInputDialog::getInt( this, tr( "New layer..." ), tr( "Enter layer id" ), lastLayer, Globals::FirstLayer, Globals::LastLayer, 10, &ok );
	
	if ( ok )
	{
		if ( layers().contains( layer ) )
		{
			QMessageBox::warning( this, "", tr( "A layer with this id already exists." ) );
			return;
		}
		
		QTreeWidgetItem* item = new TreeWidgetLayersItem( this );
		item->setText( 0, QString::number( layer ) );
		item->setData( 0, Qt::UserRole, layer );
		
		updateItems();
		setCurrentLayer( layer );
	}
}

void TreeWidgetLayers::addSkyLayer()
{
	if ( layers().contains( Globals::SkyLayer ) )
	{
		QMessageBox::warning( this, "", tr( "The sky layer already exists." ) );
		setCurrentLayer( Globals::SkyLayer );
		return;
	}
	
	QTreeWidgetItem* item = new TreeWidgetLayersItem( this );
	item->setText( 0, QString::number( Globals::SkyLayer ) );
	item->setData( 0, Qt::UserRole, Globals::SkyLayer );
	
	updateItems();
	setCurrentLayer( Globals::SkyLayer );
}

void TreeWidgetLayers::removeLayer()
{
	const int layer = currentLayer();
	
	if ( layer == -1 )
	{
		return;
	}
	
	delete selectedItems().value( 0 );
	emit layerRemoved( layer );
}

void TreeWidgetLayers::clearLayers()
{
	clear();
	emit layersCleared();
}

int TreeWidgetLayers::currentLayer() const
{
	QTreeWidgetItem* item = selectedItems().value( 0 );
	return item ? item->data( 0, Qt::UserRole ).toInt() : -1;
}

void TreeWidgetLayers::setCurrentLayer( int layer )
{
	for ( int i = 0; i < topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* item = topLevelItem( i );
		const int _layer = item->data( 0, Qt::UserRole ).toInt();
		
		if ( _layer == layer )
		{
			setCurrentItem( item );
			return;
		}
	}
}

void TreeWidgetLayers::updateItems()
{
	bool haveAll = false;
	
	for ( int i = 0; i < topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* item = topLevelItem( i );
		const int layer = item->data( 0, Qt::UserRole ).toInt();
		
		if ( layer == -1 )
		{
			haveAll = true;
			item->setText( 0, tr( "All" ) );
		}
		else if ( layer == Globals::SkyLayer )
		{
			item->setText( 0, tr( "Sky" ) );
		}
	}
	
	if ( topLevelItemCount() > 0 && !haveAll )
	{
		QTreeWidgetItem* item = new TreeWidgetLayersItem( this );
		item->setText( 0, tr( "All" ) );
		item->setData( 0, Qt::UserRole, -1 );
	}
	
	sortItems( 0, Qt::AscendingOrder );
}

void TreeWidgetLayers::_q_itemSelectionChanged()
{
	QTreeWidgetItem* item = selectedItems().value( 0 );
	int layer = -1;
	
	if ( item )
	{
		layer = item->data( 0, Qt::UserRole ).toInt();
	}
	
	emit currentLayerChanged( layer );
}
