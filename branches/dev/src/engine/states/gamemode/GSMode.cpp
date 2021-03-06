#include "GSMode.h"
#include "GSIntroduction.h"
#include "GSMultiPlayerSetup.h"
#include "GSPadSettings.h"

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

void GSMode::init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::init( engine, size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mBackgroundValue = 0;
	
	mBackground = mTiles.value( "game screens/mode_background.png" )->tile( 0 );
	mTitle = new QGraphicsPixmapItem( mTiles.value( "game screens/mode_title.png" )->tile( 0 ), this );
	mFrame = new QGraphicsPixmapItem( mTiles.value( "game screens/mode_frame.png" )->tile( 0 ), this );
	mBombers = new QGraphicsPixmapItem( mTiles.value( "game screens/mode_bombers.png" )->tile( 0 ), this );
	const QPixmap cursor = mTiles.value( "game screens/cursor_head.png" )->tile( 0 );
	
	mTitle->setZValue( 2 );
	mFrame->setZValue( 1 );
	mBombers->setZValue( 3 );
	
	const QPointF center = boundingRect().center();
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
	mMenu->setZValue( 4 );
	mMenu->addTextItem( cursor, tr( "STORY MODE" ) )->setEnabled( false );
	mMenu->addTextItem( cursor, tr( "MULTIPLAYER MODE" ) );
	mMenu->addTextItem( cursor, tr( "SETTINGS" ) );
	
	foreach ( GSMenuItem* item, mMenu->items() )
	{
		item->setActiveColor( QColor( Qt::transparent ) );
		item->setActiveDisabledColor( QColor( Qt::transparent ) );
	}
	
	mMenu->setSelectedIndex( 0 );
	
	QRectF menuRect = mMenu->boundingRect();
	menuRect.moveCenter( center );
	menuRect.moveBottom( boundingRect().bottom() -20 );
	mMenu->setPos( menuRect.topLeft() );
}

void GSMode::cleanup()
{
	AbstractGameState::cleanup();
	
	mBackgroundValue = 0;
	mBackground = QPixmap();
	Q_CLEANUP( mTitle );
	Q_CLEANUP( mFrame );
	Q_CLEANUP( mBombers );
	Q_CLEANUP( mMenu );
}

void GSMode::pause()
{
}

void GSMode::resume()
{
}

void GSMode::handleEvents( GameEngine* game )
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
						game->changeState( GSIntroduction::instance() );
						break;
					case Qt::Key_Up:
						mMenu->selectPreviousItem();
						break;
					case Qt::Key_Down:
						mMenu->selectNextItem();
						break;
					case Qt::Key_Return:
					case Qt::Key_Enter:
					{
						if ( !validateState( game ) )
							return;
						
						switch ( mMenu->selectedIndex() )
						{
							case 0:
								break;
							case 1:
								game->changeState( GSMultiPlayerSetup::instance() );
								break;
							case 2:
								game->changeState( GSPadSettings::instance( game->padsSettings() ) );
								break;
						}
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
}

void GSMode::update( GameEngine* game )
{
	Q_UNUSED( game );
	
	mBackgroundValue++;
	
	if ( mBackgroundValue > mBackground.height() )
	{
		mBackgroundValue = 0;
	}
}

bool GSMode::validateState( GameEngine* game ) const
{
	Q_UNUSED( game );
	return true;
}

void GSMode::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( boundingRect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
