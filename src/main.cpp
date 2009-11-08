#include <QApplication>

#include "TilesManager.h"
#include "GameEngine.h"
#include "GSIntroduction.h"

#include "Map.h"
#include "GSMultiPlayGround.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	app.setOrganizationDomain( "cooler.googlecode.com" );
	app.setOrganizationName( "SoDream" );
	app.setApplicationName( "Cooler" );
	
	// initialize tiles
	TilesManager::instance()->loadDatas();
	TilesManager::instance()->setTileScaledSize( QSize( 60, 60 ) );
	
	// initialize game engine
	GameEngine engine;
	engine.init( "Cooler", QSize( 1024, 768 ) );

	// load the intro
	engine.changeState( GSIntroduction::instance() );
	
	// ****************** QUICK START HACK *****************
	const TilesMap faces = TilesManager::instance()->tiles( Globals::PlayerTile );
	PlayerList players = engine.players();
	
	// p1
	players[ 0 ].setState( Globals::PlayerStateHuman );
	players[ 0 ].setTile( faces.value( "players/Windows.png" ) );
	
	// p2
	players[ 1 ].setState( Globals::PlayerStateHuman );
	players[ 1 ].setTile( faces.value( "players/Apple.png" ) );
	
	// set players
	engine.setPlayers( players );
	
	// set map
	engine.setMap( new Map( engine.maps().first() ) );
	
	//set gameplay state
	engine.changeState( GSMultiPlayGround::instance() );
	// ****************** QUICK END HACK *****************
	
	// run engine
	engine.start();
	QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	int result = app.exec();

	// cleanup the engine
	engine.cleanup();
	return result;
}
