#include "UIMain.h"
#include "MapItem.h"
#include "PlayerItem.h"
#include "TilesManager.h"
#include "pXmlSettings.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

UIMain::UIMain( QWidget* parent )
	: QMainWindow( parent )
{
	mSettings = new pXmlSettings( this );
	setupUi( this );
	swPages->setCurrentIndex( 0 );
}

UIMain::~UIMain()
{
}

void UIMain::initialize()
{
	// maps
	QFileInfoList maps;
#ifdef Q_OS_MAC
		maps = Globals::getFiles( qApp->applicationDirPath().append( "/../../../maps" ), QStringList( "*.xml" ) );
#else
		maps = Globals::getFiles( qApp->applicationDirPath().append( "/maps" ), QStringList( "*.xml" ) );
#endif
	foreach ( const QFileInfo& map, maps )
	{
		QListWidgetItem* item = new QListWidgetItem( map.baseName(), lwMaps );
		item->setData( Qt::UserRole, map.absoluteFilePath() );
	}
	
	// players
	const TilesMap tiles = TilesManager::instance()->tiles( Globals::PlayerTile );
	const QStringList playersKeys = tiles.keys();
	const bool playersLocked = cbPlayers->blockSignals( true );
	
	for ( int i = 0; i < playersKeys.count(); i++ )
	{
		const QString& name = playersKeys.at( i );
		AbstractTile* tile = tiles[ name ];
		cbPlayers->addItem( tile->tile( 0 ), tile->name(), QVariant::fromValue( tile ) );
		mPlayersNameMapping[ tile->name() ] = i;
	}
	
	cbPlayers->blockSignals( playersLocked );
	
	sbPlayerNumber->setMinimum( 1 );
	sbPlayerNumber->setMaximum( Globals::MaxPlayers );
	
	pcEditor->setPads( mPads, Globals::MaxPlayers );
	loadSettings();
}

void UIMain::loadSettings()
{
	// selected map
	const QString selectedMap = mSettings->value( "LastSelectedMap" ).toString();
	if ( !selectedMap.isEmpty() )
	{
		for ( int i = 0; i < lwMaps->count(); i++ )
		{
			QListWidgetItem* mapItem = lwMaps->item( i );
			
			if ( mapItem->text() == selectedMap )
			{
				lwMaps->setCurrentItem( mapItem );
				break;
			}
		}
	}
	// player number
	sbPlayerNumber->setValue( mSettings->value( "PlayerNumber", "2" ).toInt() );
	// players definition
	mSettings->beginReadArray( "Players" );
	for ( int i = 0; i < lwPlayers->count(); i++ )
	{
		mSettings->setArrayIndex( i );
		QListWidgetItem* playerItem = lwPlayers->item( i );
		const QString name = mSettings->value( "Tile" ).toString();
		const int playerIndex = mPlayersNameMapping.value( name );
		AbstractTile* tile = cbPlayers->itemData( playerIndex, Qt::UserRole ).value<AbstractTile*>();
		
		playerItem->setIcon( tile->tile( 0 ) );
		playerItem->setText( cbPlayers->itemText( playerIndex ) );
		playerItem->setData( Qt::UserRole, QVariant::fromValue( tile ) );
	}
	mSettings->endArray();
	// pad settings
	mSettings->beginReadArray( "Pads" );
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		mSettings->setArrayIndex( i );
		PadSettings& pad = mPads[ i ];
		pad.loadMapping( mSettings );
		
		if ( !pad.isValid() )
		{
		qWarning() << "yo";
			pad = PadSettings::defaultConfiguration( i );
		}
	}
	mSettings->endArray();
	pcEditor->updateGui();
}

void UIMain::saveSettings()
{
	// selected map
	QListWidgetItem* mapItem = lwMaps->selectedItems().value( 0 );
	mSettings->setValue( "LastSelectedMap", mapItem ? mapItem->text() : QString::null );
	// player number
	mSettings->setValue( "PlayerNumber", sbPlayerNumber->value() );
	// players definition
	mSettings->beginWriteArray( "Players", lwPlayers->count() );
	for ( int i = 0; i < lwPlayers->count(); i++ )
	{
		mSettings->setArrayIndex( i );
		QListWidgetItem* playerItem = lwPlayers->item( i );
		AbstractTile* tile = playerItem->data( Qt::UserRole ).value<AbstractTile*>();
		mSettings->setValue( "Tile", tile ? tile->name() : QString::null );
	}
	mSettings->endArray();
	// pad settings
	mSettings->beginWriteArray( "Pads", mPads.count() );
	for ( int i = 0; i < mPads.count(); i++ )
	{
		PadSettings& pad = mPads[ i ];
		mSettings->setArrayIndex( i );
		pad.saveMapping( mSettings );
	}
	mSettings->endArray();
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
			QMessageBox::information( this, "", tr( "Please define each players." ) );
			return;
		}
	}
	
	for ( int i = 0; i < sbPlayerNumber->value(); i++ )
	{
		if ( !mPads.at( i ).isValid() )
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
			player->setPad( &mPads[ i ] );
			gvBoard->addPlayer( playerItem->text().append( QString( " #%1" ).arg( i ) ), player );
		}
		
		swPages->setCurrentWidget( gvBoard );
		showFullScreen();
	}
	else
	{
		QMessageBox::information( this, "", tr( "Can't load the map." ) );
	}
}
