#include "GameEngine.h"
#include "AbstractGameState.h"

#include <pXmlSettings.h>

#include <QGLWidget>
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>

void GameEngine::Init( const QString& title, const QSize& size, int bpp, bool fullscreen )
{
	Q_UNUSED( bpp );
	
	// settings
	mSettings = new pXmlSettings( this );
	
	// geometry
	QRect geometry = QRect( QPoint( 0, 0 ), size );
	geometry.moveCenter( QApplication::desktop()->availableGeometry().center() );
	
	// scene
	mScene = new QGraphicsScene( this );
	mScene->setSceneRect( QRectF( QPointF( 0, 0 ), QSizeF( size ) ) );
	
	// variables
	mIDLETimer = -1;
	mRunning = true;
	mFullScreen = fullscreen;
	mSize = size;
	
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

void GameEngine::Cleanup()
{
	while ( !mStates.isEmpty() )
	{
		mScene->removeItem( mStates.last() );
		mStates.last()->Cleanup();
		mStates.erase( mStates.end() -1 );
	}
	
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

void GameEngine::ChangeState( AbstractGameState* state ) 
{
	if ( !mStates.isEmpty() )
	{
		mScene->removeItem( mStates.last() );
		mStates.last()->Cleanup();
		mStates.erase( mStates.end() -1 );
	}
	
	mStates.append( state );
	mStates.last()->Init( this, mSize );
	mScene->addItem( state );
	mScene->setFocusItem( state );
}

void GameEngine::PushState( AbstractGameState* state )
{
	if ( !mStates.isEmpty() )
	{
		mStates.last()->Pause();
	}
	
	mStates.append( state );
	mStates.last()->Init( this, mSize );
	mScene->addItem( state );
	mScene->setFocusItem( state );
}

void GameEngine::PopState()
{
	if ( !mStates.isEmpty() )
	{
		mScene->removeItem( mStates.last() );
		mStates.last()->Cleanup();
		mStates.erase( mStates.end() -1 );
	}
	
	if ( !mStates.isEmpty() )
	{
		mStates.last()->Resume();
	}
}


void GameEngine::HandleEvents() 
{
	if ( mStates.isEmpty() )
	{
		return;
	}
	
	mStates.last()->HandleEvents( this );
}

void GameEngine::Update() 
{
	if ( mStates.isEmpty() )
	{
		return;
	}
	
	mStates.last()->Update( this );
}

void GameEngine::Draw() 
{
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

void GameEngine::closeEvent( QCloseEvent* event )
{
	stop();
	QGraphicsView::closeEvent( event );
}

void GameEngine::timerEvent( QTimerEvent* event )
{
	if ( event->timerId() == mIDLETimer )
	{
		if ( !mRunning )
		{
			close();
		}
		
		HandleEvents();
		Update();
		Draw();
	}
}

void GameEngine::start()
{
	if ( mIDLETimer == -1 )
	{
		const int fps = 30;
		mIDLETimer = startTimer( 1000 /fps );
		mRunning = true;
	}
}

void GameEngine::stop()
{
	if ( mIDLETimer != -1 )
	{
		killTimer( mIDLETimer );
		mIDLETimer = -1;
		mRunning = false;
	}
}
