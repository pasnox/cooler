#include "GameEngine.h"
#include "AbstractGameState.h"

#include <pXmlSettings.h>

#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>

GameEngine::GameEngine()
{
	// settings
	mSettings = new pXmlSettings( this );
	
	// pad structures
	mSettings->beginReadArray( "Pads" );
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		mSettings->setArrayIndex( i );
		
		PadSettings& pad = mPads[ i ];
		pad.loadMapping( mSettings );
		
		if ( !pad.isValid() )
		{
			pad = PadSettings::defaultConfiguration( i );
		}
	}
	mSettings->endArray();
}

void GameEngine::Init( const QString& title, const QSize& size, int bpp, bool fullscreen )
{
	QRect geometry = QRect( QPoint( 0, 0 ), size );
	geometry.moveCenter( QApplication::desktop()->availableGeometry().center() );
	
	mScene = new QGraphicsScene( this );
	mScene->setSceneRect( QRectF( QPointF( 0, 0 ), QSizeF( size ) ) );
	mIDLETimer = -1;
	mRunning = true;
	mFullScreen = fullscreen;
	mSize = size;
	
	setWindowTitle( title );
	setFrameStyle( QFrame::NoFrame | QFrame::Plain );
	setAlignment( Qt::AlignCenter );
	setGeometry( geometry );
	setScene( mScene );
	
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
		mStates.last()->Cleanup();
		mStates.erase( mStates.end() -1 );
	}
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
	mStates.last()->Init( mSize );
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
	mStates.last()->Init( mSize );
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
	if ( mStates.isEmpty() )
	{
		return;
	}
	
	mStates.last()->Draw( this );
}

void GameEngine::setPadSettings( const PadSettingsMap& pads )
{
	mPads = pads;
}

PadSettingsMap GameEngine::padsSettings() const
{
	return mPads;
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
