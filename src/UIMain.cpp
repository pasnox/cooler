#include "UIMain.h"
#include "MapItem.h"
#include "PlayerItem.h"

#include <QFileDialog>
#include <QDebug>

UIMain::UIMain( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
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
	
	if ( gvView->map()->load( fn ) )
	{
		PlayerTile* tile = static_cast<PlayerTile*>( TilesManager::instance()->tiles( Globals::PlayerTile ).values().first() );
		PlayerItem* player = new PlayerItem( tile, 0 );
		gvView->addPlayer( tile->Name, player );
	}
}
