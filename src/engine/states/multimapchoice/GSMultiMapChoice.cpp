#include "GSMultiMapChoice.h"
#include "GSMultiGamePlay.h"
#include "GSMultiPlayGround.h"
#include "GSMapItem.h"
#include "Map.h"

#include <QGraphicsLinearLayout>
#include <QPainter>

GSMultiMapChoice* GSMultiMapChoice::mInstance = 0;

GSMultiMapChoice* GSMultiMapChoice::instance()
{
	if ( !mInstance )
	{
		mInstance = new GSMultiMapChoice();
	}
	
	return mInstance;
}

void GSMultiMapChoice::init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::init( engine, size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mBackgroundValue = 0;
	
	mBackground = mTiles.value( "game screens/multiplayerchoice_background.png" )->tile( 0 );
	
	// main layout
	mMainLayout = new QGraphicsLinearLayout( Qt::Vertical, this );
	mMainLayout->setContentsMargins( 10, 10, 10, 10 );
	mMainLayout->setSpacing( 10 );
	
	// title
	mTitle = new GSMenuItem( tr( "Select Map" ), Qt::AlignCenter, 38 );
	mTitle->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	mMainLayout->addItem( mTitle );
	
	// vertical spacer 1
	mMainLayout->insertStretch( 1, 100 );
	
	// maps menu
	mMap = new GSMapItem( engine->maps() );	
	mMainLayout->addItem( mMap );
	
	// vertical spacer 2
	mMainLayout->insertStretch( 3, 100 );
}

void GSMultiMapChoice::cleanup()
{
	AbstractGameState::cleanup();
	
	mBackgroundValue = 0;
	mBackground = QPixmap();
	setLayout( 0 );
	mMainLayout = 0;
	//Q_CLEANUP( mMainLayout );
	Q_CLEANUP( mTitle );
	Q_CLEANUP( mMap );
}

void GSMultiMapChoice::pause()
{
}

void GSMultiMapChoice::resume()
{
}

void GSMultiMapChoice::handleEvents( GameEngine* game )
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
						game->changeState( GSMultiGamePlay::instance() );
						break;
					}
					case Qt::Key_Return:
					case Qt::Key_Enter:
					{
						if ( validateState( game ) )
							game->changeState( GSMultiPlayGround::instance() );
						break;
					}
					case Qt::Key_Left:
					{
						mMap->previousState();
						break;
					}
					case Qt::Key_Right:
					{
						mMap->nextState();
						break;
					}
				}
				
				break;
			}
			default:
				break;
		}
	}
}

void GSMultiMapChoice::update( GameEngine* game )
{
	Q_UNUSED( game );
	
	mBackgroundValue++;
	
	if ( mBackgroundValue > mBackground.height() )
	{
		mBackgroundValue = 0;
	}
}

bool GSMultiMapChoice::validateState( GameEngine* game ) const
{
	const PlayerList& players = game->players();
	
	foreach ( const Player& player, players )
	{
		if ( player.state() == Globals::PlayerStateOff )
		{
			continue;
		}
		
		if ( !player.tile() )
		{
			return false;
		}
	}
	
	game->setMap( new Map( mMap->map() ) );
	
	return game->map()->isValid();
}

void GSMultiMapChoice::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( boundingRect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
