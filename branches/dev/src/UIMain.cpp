#include "UIMain.h"
#include "MapItem.h"
#include "PlayerItem.h"

#include <TilesManager.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

UIMain::UIMain( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
	swPages->setCurrentIndex( 0 );
}

UIMain::~UIMain()
{
}

void UIMain::initialize()
{
	// maps
	const QFileInfoList maps = Globals::getFiles( qApp->applicationDirPath().append( "/maps" ), QStringList( "*.ini" ) );
	foreach ( const QFileInfo& map, maps )
	{
		QListWidgetItem* item = new QListWidgetItem( map.baseName(), lwMaps );
		item->setData( Qt::UserRole, map.absoluteFilePath() );
	}
	
	// players
	const TilesMap tiles = TilesManager::instance()->tiles( Globals::PlayerTile );
	const bool playersLocked = cbPlayers->blockSignals( true );
	
	foreach ( const QString& name, tiles.keys() )
	{
		AbstractTile* tile = tiles[ name ];
		cbPlayers->addItem( tile->tile( 0 ), QFileInfo( name ).baseName(), QVariant::fromValue( tile ) );
	}
	
	cbPlayers->blockSignals( playersLocked );
	
	sbPlayerNumber->setMinimum( 1 );
	sbPlayerNumber->setMaximum( Globals::MaxPlayers );
	
	pcEditor->setPads( mPads, Globals::MaxPlayers );
}

void UIMain::on_pbWelcome_clicked()
{
	swPages->setCurrentWidget( wGameMode );
}

void UIMain::on_pbMultiPlayerMode_clicked()
{
	swPages->setCurrentWidget( wMultiplayerGame );
}

void UIMain::on_sbPlayerNumber_valueChanged( int value )
{
	if ( lwPlayers->count() < value )
	{
		while ( lwPlayers->count() < value )
		{
			QListWidgetItem* item = new QListWidgetItem( tr( "Undefined player" ) );
			lwPlayers->addItem( item );
		}
	}
	else
	{
		while ( lwPlayers->count() > value )
		{
			delete lwPlayers->takeItem( lwPlayers->count() -1 );
		}
	}
}

void UIMain::on_cbPlayers_currentIndexChanged( int index )
{
	AbstractTile* tile = cbPlayers->itemData( index, Qt::UserRole ).value<AbstractTile*>();
	QListWidgetItem* item = lwPlayers->selectedItems().value( 0 );
	
	item->setIcon( tile->tile( 0 ) );
	item->setText( cbPlayers->itemText( index ) );
	item->setData( Qt::UserRole, QVariant::fromValue( tile ) );
}

void UIMain::on_lwPlayers_itemSelectionChanged()
{
	const QListWidgetItem* item = lwPlayers->selectedItems().value( 0 );
	const bool locked = cbPlayers->blockSignals( true );
	const int index = cbPlayers->findData( item ? item->data( Qt::UserRole ) : QVariant() );
	cbPlayers->setCurrentIndex( index );
	cbPlayers->blockSignals( locked );
	cbPlayers->setDisabled( lwPlayers->selectedItems().isEmpty() );
}

void UIMain::on_pbLetsGo_clicked()
{
	const QListWidgetItem* mapItem = lwMaps->selectedItems().value( 0 );
	
	if ( !mapItem )
	{
		QMessageBox::information( this, "", tr( "Please select a map." ) );
		return;
	}
	
	if ( sbPlayerNumber->value() == 1 )
	{
		QMessageBox::information( this, "", tr( "Please add at least one more player." ) );
		return;
	}
	
	for ( int i = 0; i < lwPlayers->count(); i++ )
	{
		QListWidgetItem* playerItem = lwPlayers->item( i );
		
		if ( !playerItem->data( Qt::UserRole ).value<AbstractTile*>() )
		{
			lwPlayers->setCurrentItem( playerItem );
			QMessageBox::information( this, "", tr( "Please defined each players." ) );
			return;
		}
	}
	
	for ( int i = 0; i < sbPlayerNumber->value(); i++ )
	{
		if ( !mPads.at( i )->isValid() )
		{
			pcEditor->setCurrentPadIndex( i );
			QMessageBox::information( this, "", tr( "Please set pad settings for player #%1." ).arg( i +1 ) );
			return;
		}
	}
	
	const QString mapFileName = mapItem->data( Qt::UserRole ).toString();
	
	if ( gvBoard->map()->load( mapFileName ) )
	{
		for ( int i = 0; i < lwPlayers->count(); i++ )
		{
			QListWidgetItem* playerItem = lwPlayers->item( i );
			AbstractTile* tile = playerItem->data( Qt::UserRole ).value<AbstractTile*>();
			PlayerItem* player = new PlayerItem( tile, 0 );
			player->setPad( mPads.at( i ) );
			gvBoard->addPlayer( playerItem->text().append( QString( " #%1" ).arg( i ) ), player );
		}
		
		swPages->setCurrentWidget( gvBoard );
	}
	else
	{
		QMessageBox::information( this, "", tr( "Can't load the map." ) );
	}
}
