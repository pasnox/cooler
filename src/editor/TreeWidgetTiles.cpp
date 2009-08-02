#include "TreeWidgetTiles.h"
#include "TilesManager.h"

#include <QMouseEvent>
#include <QDebug>

TreeWidgetTiles::TreeWidgetTiles( QWidget* parent )
	: QTreeWidget( parent )
{
	mTiles = 0;
	mCurrentLayer = -1;
	
	setTiles( TilesManager::instance() );
}

TreeWidgetTiles::~TreeWidgetTiles()
{
}

TilesManager* TreeWidgetTiles::tiles() const
{
	return mTiles;
}

void TreeWidgetTiles::setTiles( TilesManager* tiles )
{
	if ( mTiles == tiles )
	{
		return;
	}
	
	if ( mTiles )
	{
		disconnect( mTiles, SIGNAL( datasLoaded( bool ) ), this, SLOT( datasLoaded( bool ) ) );
	}
	
	mTiles = tiles;
	
	if ( mTiles )
	{
		connect( mTiles, SIGNAL( datasLoaded( bool ) ), this, SLOT( datasLoaded( bool ) ) );
	}
}

void TreeWidgetTiles::setCurrentLayer( int layer )
{
	mCurrentLayer = layer;
}

void TreeWidgetTiles::mousePressEvent( QMouseEvent* event )
{
	QTreeWidget::mousePressEvent( event );
	QTreeWidgetItem* item = selectedItems().value( 0 );
	QTreeWidgetItem* mouseItem = itemAt( event->pos() );
	
	if ( mouseItem == item && item && item->type() != -1 && mCurrentLayer != -1 )
	{
		QMouseEvent* me = static_cast<QMouseEvent*>( event );
		
		if ( me->button() == Qt::LeftButton )
		{
			ObjectTile tile = item->data( 0, Qt::UserRole ).value<ObjectTile>();
			QDrag* drag = new QDrag( this );
			QMimeData* mimeData = new QMimeData;
			
			QByteArray datas;
			QDataStream stream( &datas, QIODevice::WriteOnly );
			stream << tile;

			mimeData->setData( "application/x-cooler-layer", QByteArray::number( mCurrentLayer ) );
			mimeData->setData( "application/x-cooler-objecttile", datas );

			drag->setMimeData( mimeData );
			drag->setPixmap( tile.Pixmap );
			drag->exec( Qt::CopyAction, Qt::CopyAction );
		}
	}
}

void TreeWidgetTiles::datasLoaded( bool success )
{
	if ( success )
	{
		clear();
		
		// Block
		QTreeWidgetItem* blocks = new QTreeWidgetItem( this, -1 );
		blocks->setText( 0, tr( "Blocks" ) );
		
		// Sky
		QTreeWidgetItem* sky = new QTreeWidgetItem( this, -1 );
		sky->setText( 0, tr( "Sky" ) );
		
		// Box
		QTreeWidgetItem* boxes = new QTreeWidgetItem( this, -1 );
		boxes->setText( 0, tr( "Boxes" ) );
		
		// Block
		QTreeWidgetItem* floors = new QTreeWidgetItem( this, -1 );
		floors->setText( 0, tr( "Floors" ) );
		
		// tiles
		TilesMap tiles = mTiles->objectTiles();
		foreach ( const QString& name, tiles.keys() )
		{
			const ObjectTile tile = tiles[ name ];
			QTreeWidgetItem* item = 0;
			
			switch ( tile.Type )
			{
				case Globals::BlockObject:
					item = new QTreeWidgetItem( blocks, tile.Type );
					break;
				case Globals::BoxObject:
					item = new QTreeWidgetItem( boxes, tile.Type );
					break;
				case Globals::FloorObject:
					item = new QTreeWidgetItem( floors, tile.Type );
					break;
				case Globals::SkyObject:
					item = new QTreeWidgetItem( sky, tile.Type );
					break;
				case Globals::InvalidObject:
					Q_ASSERT( 0 );
					break;
			}
			
			Q_ASSERT( item );
			item->setIcon( 0, QIcon( tile.Pixmap ) );
			item->setText( 0, QFileInfo( name ).fileName() );
			item->setToolTip( 0, name );
			item->setData( 0, Qt::UserRole, QVariant::fromValue( tile ) );
		}
	}
}
