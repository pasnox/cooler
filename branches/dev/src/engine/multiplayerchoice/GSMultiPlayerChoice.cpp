#include "GSMultiPlayerChoice.h"
#include "GSMultiGamePlay.h"
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

void GSMultiPlayerChoice::Init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::Init( engine, size );
	
	mFacesTiles = TilesManager::instance()->tiles( Globals::PlayerTile );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mBackgroundValue = 0;
	
	mBackground = mTiles.value( "game screens/multiplayersetup_background.png" )->tile( 0 );
	
	// main layout
	mMainLayout = new QGraphicsLinearLayout( Qt::Vertical, this );
	mMainLayout->setContentsMargins( 10, 10, 10, 10 );
	mMainLayout->setSpacing( 10 );
	
	// title
	mTitle = new GSMenuItem( tr( "Select Player" ), Qt::AlignCenter, 38 );
	mTitle->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	mMainLayout->addItem( mTitle );
	
	// vertical spacer 1
	mMainLayout->insertStretch( 1, 100 );
	
	// faces menu
	mFacesMenu = new GSMenu( Qt::Horizontal );
	PlayerList players = engine->players();
	
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		GSFaceItem* item = new GSFaceItem( mFacesTiles, i );
		item->setWrapEnabled( true );
		item->setActive( players[ i ].state() == Globals::PlayerStateHuman );
		mFacesMenu->addItem( item );
	}
	
	mMainLayout->addItem( mFacesMenu );
	
	// vertical spacer 2
	mMainLayout->insertStretch( 3, 100 );
}

void GSMultiPlayerChoice::Cleanup()
{
	AbstractGameState::Cleanup();
	
	mBackgroundValue = 0;
	mBackground = QPixmap();
	setLayout( 0 );
	mMainLayout = 0;
	//Q_CLEANUP( mMainLayout );
	Q_CLEANUP( mTitle );
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
						game->ChangeState( GSMultiGamePlay::instance() );
						break;
					}
					case Qt::Key_Return:
					case Qt::Key_Enter:
					{
						/*
						if ( validateState( game ) )
							game->ChangeState( GSMultiGamePlay::instance() );
						*/
						break;
					}
				}
				
				const PadSettingsList& pads = game->padsSettings();
				
				for ( uint i = 0; i < Globals::MaxPlayers; i++ )
				{
					const PadSettings& pad = pads.at( i );
					GSFaceItem* item = static_cast<GSFaceItem*>( mFacesMenu->item( i ) );
					
					if ( !item->isActive() )
					{
						continue;
					}
					
					if ( pad.strokeKey( Globals::PadStrokeUp ) == ke->key )
					{
						item->previousState();
					}
					else if ( pad.strokeKey( Globals::PadStrokeDown ) == ke->key )
					{
						item->nextState();
					}
				}
				
				break;
			}
			default:
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

bool GSMultiPlayerChoice::validateState( GameEngine* game ) const
{
/*
	PlayerList players = game->players();
	int activeCount = 0;
	
	for ( int i = 0; i < mStatesMenu->count(); i++ )
	{
		GSStateItem* item = static_cast<GSStateItem*>( mStatesMenu->item( i ) );
		players[ i ].setState( item->playerState() );
		activeCount += item->playerState() != Globals::PlayerStateOff ? 1 : 0;
	}
	
	if ( activeCount > 1 )
	{
		game->setPlayers( players );
		return true;
	}
*/
	return false;
}

void GSMultiPlayerChoice::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( boundingRect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
