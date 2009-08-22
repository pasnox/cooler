#include "GSMode.h"
#include "GSIntroduction.h"
#include "GSPadSettings.h"

#include <QGraphicsTextItem>
#include <QPainter>
#include <QGraphicsPixmapItem>

GSMode* GSMode::mInstance = 0;

GSMode* GSMode::instance()
{
	if ( !mInstance )
	{
		mInstance = new GSMode();
	}
	
	return mInstance;
}

void GSMode::Init( const QSizeF& size )
{
	AbstractGameState::Init( size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mBackgroundValue = 0;
	
	mBackground = mTiles.value( "game screens/mode_background.png" )->tile( 0 );
	mTitle = new QGraphicsPixmapItem( mTiles.value( "game screens/mode_title.png" )->tile( 0 ), this );
	mFrame = new QGraphicsPixmapItem( mTiles.value( "game screens/mode_frame.png" )->tile( 0 ), this );
	mBombers = new QGraphicsPixmapItem( mTiles.value( "game screens/mode_bombers.png" )->tile( 0 ), this );
	
	mBombers->setZValue( 1 );
	
	const QPointF center = rect().center();
	QRectF titleRect = QRectF( QPointF(), mTitle->boundingRect().size() );
	QRectF frameRect = QRectF( QPointF(), mFrame->boundingRect().size() );
	QRectF bombersRect = QRectF( QPointF(), mBombers->boundingRect().size() );
	
	titleRect.moveCenter( center );
	titleRect.moveTop( 10 );
	frameRect.moveCenter( center );
	bombersRect.moveCenter( center -( QPointF( 2.5, 7 ) *2 ) );
	
	mTitle->setPos( titleRect.topLeft() );
	mFrame->setPos( frameRect.topLeft() );
	mBombers->setPos( bombersRect.topLeft() );
	
	mMenu = new GSMenu( Qt::Vertical, this );
	mMenu->addItem( tr( "STORY MODE" ) )->setEnabled( false );
	mMenu->addItem( tr( "MULTIPLAYER MODE" ) );
	mMenu->addItem( tr( "SETTINGS" ) );
	mMenu->setSelectedIndex( 0 );
	
	QRectF menuRect = mMenu->boundingRect();
	menuRect.moveCenter( center );
	menuRect.moveBottom( size.height() -( menuRect.height() /2 ) );
	mMenu->setPos( menuRect.topLeft() );
	mMenu->update();
}

void GSMode::Cleanup()
{
	AbstractGameState::Cleanup();
	
	mBackgroundValue = 0;
	mBackground = QPixmap();
	Q_CLEANUP( mTitle );
	Q_CLEANUP( mFrame );
	Q_CLEANUP( mBombers );
	Q_CLEANUP( mMenu );
}

void GSMode::Pause()
{
}

void GSMode::Resume()
{
}

void GSMode::HandleEvents( GameEngine* game )
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
						game->ChangeState( GSIntroduction::instance() );
						break;
					case Qt::Key_Up:
						mMenu->selectPreviousItem();
						break;
					case Qt::Key_Down:
						mMenu->selectNextItem();
						break;
					case Qt::Key_Return:
					case Qt::Key_Enter:
						if ( mMenu->selectedIndex() == 2 )
						{
							game->ChangeState( GSPadSettings::instance( game->padsSettings() ) );
						}
					default:
						//game->ChangeState( 0 );
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

void GSMode::Update( GameEngine* game )
{
	Q_UNUSED( game );
	
	mBackgroundValue++;
	
	if ( mBackgroundValue > mBackground.height() )
	{
		mBackgroundValue = 0;
	}
}

void GSMode::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( rect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
