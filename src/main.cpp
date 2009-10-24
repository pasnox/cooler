#include <QApplication>

#include "TilesManager.h"
#include "GameEngine.h"
#include "GSIntroduction.h"

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
	engine.Init( "Cooler", QSize( 1024, 768 ) );

	// load the intro
	engine.ChangeState( GSIntroduction::instance() );
	
	// run engine
	engine.start();
	QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	int result = app.exec();

	// cleanup the engine
	engine.Cleanup();
	return result;
}
