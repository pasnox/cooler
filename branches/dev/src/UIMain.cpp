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
		const PlayerTile playerTile = TilesManager::instance()->playerTiles().values().first();
		PlayerItem* player = new PlayerItem( playerTile, gvView );
		gvView->addPlayer( playerTile.Name, player );
	}
}
