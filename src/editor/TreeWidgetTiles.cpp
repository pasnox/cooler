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
			AbstractTile* tile = item->data( 0, Qt::UserRole ).value<AbstractTile*>();
			QDrag* drag = new QDrag( this );
			QMimeData* mimeData = new QMimeData;

			mimeData->setData( "application/x-cooler-layer", QByteArray::number( mCurrentLayer ) );
			mimeData->setData( "application/x-cooler-tile", AbstractTile::toByteArray( tile ) );

			drag->setMimeData( mimeData );
			drag->setPixmap( tile->tile( 0 ) );
			drag->exec( Qt::CopyAction, Qt::CopyAction );
		}
	}
}

void TreeWidgetTiles::datasLoaded( bool success )
{
	if ( success )
	{
		clear();
		mTypesItems.clear();
		
		const TypesTilesMap tiles = mTiles->tiles();
		QTreeWidgetItem* item;
		
		foreach ( const Globals::TypeTile& type, tiles.keys() )
		{
			item = new QTreeWidgetItem( this, -1 );
			
			switch ( type )
			{
				case Globals::BlockTile:
					item->setText( 0, tr( "Blocks" ) );
					break;
				case Globals::BoxTile:
					item->setText( 0, tr( "Boxes" ) );
					break;
				case Globals::FloorTile:
					item->setText( 0, tr( "Floors" ) );
					break;
				case Globals::SkyTile:
					item->setText( 0, tr( "Sky" ) );
					break;
				case Globals::PlayerTile:
					item->setText( 0, tr( "Players" ) );
					break;
				case Globals::BombTile:
					item->setText( 0, tr( "Bombs" ) );
					break;
				case Globals::InvalidTile:
					Q_ASSERT( 0 );
					break;
			}
			
			mTypesItems[ type ] = item;
		}
		
		// tiles
		foreach ( const Globals::TypeTile& type, tiles.keys() )
		{
			const TilesMap typeTiles = tiles[ type ];
			
			foreach ( const QString& name, typeTiles.keys() )
			{
				AbstractTile* tile = typeTiles[ name ];
				item = new QTreeWidgetItem( mTypesItems[ type ], tile->Type );
				
				item->setIcon( 0, QIcon( tile->tile( 0 ) ) );
				item->setText( 0, tile->name() );
				item->setToolTip( 0, tile->absoluteFilePath() );
				item->setData( 0, Qt::UserRole, QVariant::fromValue( tile ) );
			}
		}
	}
}
