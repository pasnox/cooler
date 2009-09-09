#include "GSIntroduction.h"
#include "GSMode.h"
#include "GSMenuItem.h"

#include <QPainter>

GSIntroduction* GSIntroduction::mInstance = 0;

GSIntroduction* GSIntroduction::instance()
{
	if ( !mInstance )
	{
		mInstance = new GSIntroduction();
	}
	
	return mInstance;
}

void GSIntroduction::Init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::Init( engine, size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mGameScreen = mTiles.value( "game screens/bomberman.png" )->tile( 0 );
	
	mElapsedTime.restart();
	
	mPressAnyKey = new GSMenuItem( tr( "Press Any Key" ), this, Qt::AlignCenter, 28 );
	mPressAnyKey->setPos( ( rect().width() -mPressAnyKey->boundingRect().width() ) /2, rect().bottom() -mPressAnyKey->boundingRect().height() );
}

void GSIntroduction::Cleanup()
{
	AbstractGameState::Cleanup();
	
	mGameScreen = QPixmap();
	Q_CLEANUP( mPressAnyKey );
}

void GSIntroduction::Pause()
{
}

void GSIntroduction::Resume()
{
}

void GSIntroduction::HandleEvents( GameEngine* game )
{
	Q_UNUSED( game );
	
	foreach ( Event* event, mEvents )
	{
		switch ( event->type )
		{
			case Event::KeyPress:
			{
				KeyEvent* ke = static_cast<KeyEvent*>( event );
				
				switch ( ke->key )
				{
					case Qt::Key_Escape:
						game->Quit();
						break;
					default:
						game->ChangeState( GSMode::instance() );
						break;
				}
				
				break;
			}
			case Event::KeyRelease:
				break;
			case Event::MouseDoubleClick:
				break;
			case Event::MousePress:
				break;
			case Event::MouseRelease:
				break;
			case Event::MouseMove:
				break;
			case Event::Invalid:
				break;
		}
	}
	
	qDeleteAll( mEvents );
	mEvents.clear();
}

void GSIntroduction::Update( GameEngine* game )
{
	Q_UNUSED( game );
	
	int elapsed = mElapsedTime.elapsed() /1000.0;
	mPressAnyKey->setVisible( elapsed %2 == 0 );
}

void GSIntroduction::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawPixmap( boundingRect().topLeft(), mGameScreen );
}
