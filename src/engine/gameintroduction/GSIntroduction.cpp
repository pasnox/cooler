#include "GSIntroduction.h"
#include "GSMode.h"

#include <QGraphicsTextItem>
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

void GSIntroduction::Init( const QSizeF& size )
{
	AbstractGameState::Init( size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mGameScreen = mTiles.values().first()->tile( 0 );
	
	mTextItem = new QGraphicsTextItem( this );
	mTextItem->setDefaultTextColor( Qt::red );
	mTextItem->setHtml( tr( "<h1>P r e s s&nbsp;&nbsp;A n y&nbsp;&nbsp;K e y</h1>" ) );
	mTextItem->setPos( ( rect().width() -mTextItem->boundingRect().width() ) /2, rect().bottom() -mTextItem->boundingRect().height() );
	
	mPressStartTimer = startTimer( 700 );
}

void GSIntroduction::Cleanup()
{
	AbstractGameState::Cleanup();
	
	killTimer( mPressStartTimer );
	mPressStartTimer = -1;
	delete mTextItem;
	mTextItem = 0;
	mGameScreen = QPixmap();
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
}

void GSIntroduction::Draw( GameEngine* game )
{
	Q_UNUSED( game );
}

void GSIntroduction::timerEvent( QTimerEvent* event )
{
	if ( event->timerId() == mPressStartTimer )
	{
		mTextItem->setVisible( !mTextItem->isVisible() );
	}
}

void GSIntroduction::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->drawPixmap( rect().topLeft(), mGameScreen );
	AbstractGameState::paint( painter, option, widget );
}
