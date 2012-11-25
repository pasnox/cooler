#include "GameEngine.h"
#include "AbstractGameState.h"
#include "pXmlSettings.h"
#include "Map.h"

#include <QGLWidget>
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>

void GameEngine::init( const QString& title, const QSize& size, int bpp, bool fullscreen )
{
	Q_UNUSED( bpp );
	
	// settings
	mSettings = new pXmlSettings( this );
	
	// geometry
	QRect geometry = QRect( QPoint( 0, 0 ), size );
	geometry.moveCenter( QApplication::desktop()->availableGeometry().center() );
	
	// scene
	mScene = new QGraphicsScene( this );
	mScene->setItemIndexMethod( QGraphicsScene::NoIndex );
	mScene->setSceneRect( QRectF( QPointF( 0, 0 ), QSizeF( size ) ) );
	
	// variables
	mIDLETimer = -1;
	mFPSTimer = -1;
	mRunning = true;
	mFullScreen = fullscreen;
	mSize = size;
	
	mMap = 0;
	
	// view properties
	//setViewport( new QGLWidget );
	setViewportUpdateMode( /*QGraphicsView::FullViewportUpdate |*/ QGraphicsView::NoViewportUpdate );
	setWindowTitle( title );
	setFrameStyle( QFrame::NoFrame | QFrame::Plain );
	setAlignment( Qt::AlignCenter );
	setGeometry( geometry );
	setScene( mScene );
	
	// load player/pad structures
	mSettings->beginReadArray( "Pads" );
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		mSettings->setArrayIndex( i );
		
		PadSettings pad;
		pad.loadMapping( mSettings );
		
		if ( !pad.isValid() )
		{
			pad = PadSettings::defaultConfiguration( i );
		}
		
		mPads << pad;
		mPlayers << Player();
	}
	mSettings->endArray();
	
	// get maps
#ifdef Q_OS_MAC
	mMaps = Globals::getFiles( qApp->applicationDirPath().append( "/../../../maps" ), QStringList( "*.xml" ) );
#else
	mMaps = Globals::getFiles( qApp->applicationDirPath().append( "/maps" ), QStringList( "*.xml" ) );
#endif
	
	// show
	if ( fullscreen )
	{
		showFullScreen();
	}
	else
	{
		setFixedSize( size );
		show();
	}
}

void GameEngine::cleanup()
{
	while ( !mStates.isEmpty() )
	{
		mStates.last()->ungrabKeyboard();
		mScene->removeItem( mStates.last() );
		mStates.last()->cleanup();
		mStates.erase( mStates.end() -1 );
	}
	
	// delete map object
	delete mMap;
	
	// save pad structures
	mSettings->beginWriteArray( "Pads", Globals::MaxPlayers );
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		mSettings->setArrayIndex( i );
		
		PadSettings& pad = mPads[ i ];
		pad.saveMapping( mSettings );
	}
	mSettings->endArray();
}

void GameEngine::changeState( AbstractGameState* state ) 
{
	if ( !mStates.isEmpty() )
	{
		mStates.last()->ungrabKeyboard();
		mScene->removeItem( mStates.last() );
		mStates.last()->cleanup();
		mStates.erase( mStates.end() -1 );
	}
	
	mStates.append( state );
	mStates.last()->init( this, mSize );
	mScene->addItem( state );
	mScene->setFocusItem( state );
	state->grabKeyboard();
}

void GameEngine::pushState( AbstractGameState* state )
{
	if ( !mStates.isEmpty() )
	{
		mStates.last()->pause();
	}
	
	mStates.append( state );
	mStates.last()->init( this, mSize );
	mScene->addItem( state );
	mScene->setFocusItem( state );
	state->grabKeyboard();
}

void GameEngine::popState()
{
	if ( !mStates.isEmpty() )
	{
		mStates.last()->ungrabKeyboard();
		mScene->removeItem( mStates.last() );
		mStates.last()->cleanup();
		mStates.erase( mStates.end() -1 );
	}
	
	if ( !mStates.isEmpty() )
	{
		mStates.last()->resume();
	}
}


void GameEngine::handleEvents() 
{
	if ( mStates.isEmpty() )
	{
		return;
	}
	
	mStates.last()->handleEvents( this );
	mStates.last()->flushEvents();
}

void GameEngine::update() 
{
	if ( mStates.isEmpty() )
	{
		return;
	}
	
	mStates.last()->update( this );
}

void GameEngine::draw() 
{
	mScene->advance();
	mScene->update();
}

void GameEngine::setPadSettings( const PadSettingsList& pads )
{
	mPads = pads;
}

const PadSettingsList& GameEngine::padsSettings() const
{
	return mPads;
}

void GameEngine::setPlayers( const PlayerList& players )
{
	mPlayers = players;
}

const PlayerList& GameEngine::players() const
{
	return mPlayers;
}

const QFileInfoList& GameEngine::maps() const
{
	return mMaps;
}

void GameEngine::setMap( Map* map )
{
	delete mMap;
	mMap = map;
}

Map* GameEngine::map() const
{
	return mMap;
}

void GameEngine::closeEvent( QCloseEvent* event )
{
	stop();
	QGraphicsView::closeEvent( event );
}

void GameEngine::timerEvent( QTimerEvent* event )
{
	static int fps = 0;
	
	if ( event->timerId() == mIDLETimer )
	{
		if ( !mRunning )
		{
			close();
		}
		
		handleEvents();
		update();
		draw();
		
		fps++;
	}
	else if ( event->timerId() == mFPSTimer )
	{
		setWindowTitle( QString( "FPS: %1" ).arg( fps ) );
		fps = 0;
	}
}

void GameEngine::start()
{
	if ( mIDLETimer == -1 )
	{
		const int fps = 30;
		mIDLETimer = startTimer( 1000 /fps );
		mFPSTimer = startTimer( 1000 );
		mRunning = true;
	}
}

void GameEngine::stop()
{
	if ( mIDLETimer != -1 )
	{
		killTimer( mIDLETimer );
		killTimer( mFPSTimer );
		mIDLETimer = -1;
		mFPSTimer = -1;
		mRunning = false;
	}
}
