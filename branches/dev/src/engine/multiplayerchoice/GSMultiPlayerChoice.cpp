#include "GSMultiPlayerChoice.h"
#include "GSMode.h"
#include "GSFaceItem.h"

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QGraphicsPixmapItem>

GSMultiPlayerChoice* GSMultiPlayerChoice::mInstance = 0;

GSMultiPlayerChoice* GSMultiPlayerChoice::instance()
{
	if ( !mInstance )
	{
		mInstance = new GSMultiPlayerChoice();
	}
	
	return mInstance;
}

void GSMultiPlayerChoice::Init( const QSizeF& size )
{
	AbstractGameState::Init( size );
	
	mTiles = TilesManager::instance()->tiles( Globals::PlayerTile );
	mBackgroundValue = 0;
	
	//mBackground = mTiles.value( "game screens/padsettings_background.png" )->tile( 0 );
	
	// right menu
	mFacesMenu = new GSMenu( Qt::Horizontal, this );
	mFacesMenu->setSpacing( 2 );
	
	foreach ( const QString& key, mTiles.keys() )
	{
		GSFaceItem* item = new GSFaceItem( mTiles.value( key ) );
		mFacesMenu->addItem( item );
	}
	
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
	}
}

void GSMultiPlayerChoice::Cleanup()
{
	AbstractGameState::Cleanup();
	
	mBackgroundValue = 0;
	mBackground = QPixmap();
	Q_CLEANUP( mFacesMenu );
}

void GSMultiPlayerChoice::Pause()
{
}

void GSMultiPlayerChoice::Resume()
{
}

void GSMultiPlayerChoice::HandleEvents( GameEngine* game )
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
					{
						game->ChangeState( GSMode::instance() );
						break;
					}
					case Qt::Key_Return:
					case Qt::Key_Enter:
					{
						break;
					}
					case Qt::Key_Up:
					{
						break;
					}
					case Qt::Key_Down:
					{
						break;
					}
					case Qt::Key_Left:
					{
						
						break;
					}
					case Qt::Key_Right:
					{
						break;
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

void GSMultiPlayerChoice::Update( GameEngine* game )
{
	Q_UNUSED( game );
	
	mBackgroundValue++;
	
	if ( mBackgroundValue > mBackground.height() )
	{
		mBackgroundValue = 0;
	}
}

void GSMultiPlayerChoice::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( boundingRect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
