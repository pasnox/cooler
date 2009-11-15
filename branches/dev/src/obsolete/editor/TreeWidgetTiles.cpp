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
		disconnect( mTiles, SIGNAL( dataLoaded( bool ) ), this, SLOT( dataLoaded( bool ) ) );
	}
	
	mTiles = tiles;
	
	if ( mTiles )
	{
		connect( mTiles, SIGNAL( dataLoaded( bool ) ), this, SLOT( dataLoaded( bool ) ) );
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

void TreeWidgetTiles::dataLoaded( bool success )
{
	if ( success )
	{
		clear();
		mTypesItems.clear();
		
		const TypesTilesMap& tiles = mTiles->tiles();
		
		foreach ( const Globals::TypeTile& type, tiles.keys() )
		{
			QTreeWidgetItem* item = 0;
			
			switch ( type )
			{
				case Globals::BlockTile:
					item = new QTreeWidgetItem( this, QStringList( tr( "Blocks" ) ), -1 );
					break;
				case Globals::BombTile:
					//item = new QTreeWidgetItem( this, QStringList( tr( "Bombs" ) ), -1 );
					continue;
					break;
				case Globals::BombExplosionTile:
					//item = new QTreeWidgetItem( this, QStringList( tr( "Bombs Explosions" ) ), -1 );
					continue;
					break;
				case Globals::BonusTile:
					item = new QTreeWidgetItem( this, QStringList( tr( "Bonuses" ) ), -1 );
					break;
				case Globals::BoxTile:
					item = new QTreeWidgetItem( this, QStringList( tr( "Boxes" ) ), -1 );
					break;
				case Globals::FloorTile:
					item = new QTreeWidgetItem( this, QStringList( tr( "Floors" ) ), -1 );
					break;
				case Globals::PlayerTile:
					item = new QTreeWidgetItem( this, QStringList( tr( "Players" ) ), -1 );
					break;
				case Globals::PlayerExplosionTile:
					//item = new QTreeWidgetItem( this, QStringList( tr( "Players Explosions" ) ), -1 );
					continue;
					break;
				case Globals::SkyTile:
					item = new QTreeWidgetItem( this, QStringList( tr( "Skies" ) ), -1 );
					break;
				case Globals::TextTile:
					//item = new QTreeWidgetItem( this, QStringList( tr( "Texts" ) ), -1 );
					continue;
					break;
				case Globals::InvalidTile:
					Q_ASSERT( 0 );
					continue;
					break;
			}
			
			mTypesItems[ type ] = item;
		}
		
		// tiles
		foreach ( const Globals::TypeTile& type, tiles.keys() )
		{
			switch ( type )
			{
				case Globals::InvalidTile:
					Q_ASSERT( 0 );
				case Globals::BombTile:
				case Globals::BombExplosionTile:
				case Globals::PlayerExplosionTile:
				case Globals::TextTile:
					continue;
					break;
				case Globals::BlockTile:
				case Globals::BonusTile:
				case Globals::BoxTile:
				case Globals::FloorTile:
				case Globals::PlayerTile:
				case Globals::SkyTile:
					break;
			}
			
			const TilesMap& typeTiles = tiles[ type ];
			
			foreach ( const QString& name, typeTiles.keys() )
			{
				AbstractTile* tile = typeTiles[ name ];
				QTreeWidgetItem* item = new QTreeWidgetItem( mTypesItems[ type ], tile->Type );
				
				item->setIcon( 0, QIcon( tile->tile( 0 ) ) );
				item->setText( 0, tile->name() );
				item->setToolTip( 0, tile->absoluteFilePath() );
				item->setData( 0, Qt::UserRole, QVariant::fromValue( tile ) );
			}
		}
	}
}
