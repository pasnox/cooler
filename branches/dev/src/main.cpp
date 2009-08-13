#include <QApplication>
#include <QShortcut>

#include <TilesManager.h>
#include <pXmlSettings.h>

#include "UIMain.h"
#include "editor/UIMapEditor.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );
	app.setOrganizationDomain( "cooler.googlecode.com" );
	app.setOrganizationName( "SoDream" );
	app.setApplicationName( "Cooler" );

	QSettings set( "/home/pasnox/.Monkey Studio/Monkey Studio 1.8.4.0bsvn_release.ini", QSettings::IniFormat );
	pXmlSettings settings;
	
	const QStringList keys = set.allKeys();
	
	foreach ( const QString& key, keys )
	{
		settings.setValue( key, set.value( key ) );
	}
	
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
	return app.exec();
}
