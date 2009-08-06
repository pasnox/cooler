#include "UIMain.h"
#include "MapItem.h"
#include "PlayerItem.h"

#include <TilesManager.h>

#include <QFileDialog>
#include <QDebug>

UIMain::UIMain( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
	
	// temporary pad configuration
	PadSettings::StrokeKeys strokeKeys;
	PadSettings::ActionKeys actionKeys;
	
	strokeKeys[ Qt::Key_Up ] = Globals::PadStrokeUp;
	strokeKeys[ Qt::Key_Down ] = Globals::PadStrokeDown;
	strokeKeys[ Qt::Key_Left ] = Globals::PadStrokeLeft;
	strokeKeys[ Qt::Key_Right ] = Globals::PadStrokeRight;
	
	actionKeys[ Qt::Key_Q ] = Globals::PadAction1;
	actionKeys[ Qt::Key_S ] = Globals::PadAction2;
	actionKeys[ Qt::Key_A ] = Globals::PadAction3;
	actionKeys[ Qt::Key_Z ] = Globals::PadAction4;
	
	PadSettings pad;
	pad.setStrokeKeys( strokeKeys );
	pad.setActionKeys( actionKeys );
	
	mPads << pad;
	
	pcEditor->setPads( mPads, Globals::MaxPlayers );
}

UIMain::~UIMain()
{
}

void UIMain::initialize()
{
	const QString fn = QFileDialog::getOpenFileName( this, "Choose a map", "." );
	
	if ( fn.isNull() )
	{
		return;
	}
	
	if ( gvBoard->map()->load( fn ) )
	{
		PlayerTile* tile = static_cast<PlayerTile*>( TilesManager::instance()->tiles( Globals::PlayerTile ).values().first() );
		PlayerItem* player = new PlayerItem( tile, 0 );
		gvBoard->addPlayer( tile->Name, player );
	}
}

void UIMain::on_pbWelcome_clicked()
{
	swPages->setCurrentWidget( wGameMode );
}

void UIMain::on_pbMultiPlayerMode_clicked()
{
	swPages->setCurrentWidget( wPlayersSettings );
}

void UIMain::on_pbSettingsMode_clicked()
{
	swPages->setCurrentWidget( wGameSettings );
}
