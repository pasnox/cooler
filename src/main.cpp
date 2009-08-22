#include <QApplication>
#include <QShortcut>

#include <TilesManager.h>

#include "GameEngine.h"
#include "GSIntroduction.h"


#include "UIMain.h"
#include "editor/UIMapEditor.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	app.setOrganizationDomain( "cooler.googlecode.com" );
	app.setOrganizationName( "SoDream" );
	app.setApplicationName( "Cooler" );
	
	// initialize tiles
	TilesManager::instance()->loadDatas();
	
	// initialize game engine
	GameEngine engine;
	engine.Init( "Cooler" );

	// load the intro
	engine.ChangeState( GSIntroduction::instance() );
	
	// run engine
	engine.start();
	QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	int result = app.exec();

	// cleanup the engine
	engine.Cleanup();
	return result;
	
	/*
	UIMain m;
	m.setWindowTitle( "Cooler" );
	
	UIMapEditor me;
	me.setWindowTitle( "Cooler - Map Editor" );
	
	TilesManager::instance()->loadDatas();
	
	m.initialize();
	m.show();
	
	QShortcut s( &m );
	s.setKey( QKeySequence( "Ctrl+E" ) );
	s.setContext( Qt::ApplicationShortcut );
	QObject::connect( &s, SIGNAL( activated() ), &me, SLOT( show() ) );
	
	QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	bool result = app.exec();
	
	m.saveSettings();
	return result;
	*/
}
