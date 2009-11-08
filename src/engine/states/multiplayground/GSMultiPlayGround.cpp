#include "GSMultiPlayGround.h"
#include "Map.h"
#include "MapItem.h"
#include "PlayerItem.h"

#include <QGraphicsLinearLayout>
#include <QPainter>

GSMultiPlayGround* GSMultiPlayGround::mInstance = 0;

GSMultiPlayGround* GSMultiPlayGround::instance()
{
	if ( !mInstance )
	{
		mInstance = new GSMultiPlayGround();
	}
	
	return mInstance;
}

void GSMultiPlayGround::init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::init( engine, size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	
	// main layout
	mMainLayout = new QGraphicsLinearLayout( Qt::Vertical, this );
	mMainLayout->setContentsMargins( 0, 0, 0, 0 );
	mMainLayout->setSpacing( 10 );
	
	// title
	mTitle = new GSMenuItem( tr( "** PlayGround **" ), Qt::AlignCenter, 38 );
	mTitle->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	mMainLayout->addItem( mTitle );
	
	// vertical spacer 1
	mMainLayout->insertStretch( 1, 100 );
	
	// map item
	mMap = new MapItem;
	mMap->loadMap( engine->map() );
	mMainLayout->addItem( mMap );
	mMainLayout->setAlignment( mMap, Qt::AlignCenter );
	
	// players
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		const Player* player = &engine->players().at( i );
		
		if ( player->state() == Globals::PlayerStateOff )
		{
			continue;
		}
		
		PlayerItem* playerItem = new PlayerItem( player, mMap );
		mPlayers[ i ] = playerItem;
		mMap->moveObjectToGridPosition( playerItem, engine->map()->playersPosition().at( i ) );
	}
}

void GSMultiPlayGround::cleanup()
{
	AbstractGameState::cleanup();
	
	setLayout( 0 );
	mMainLayout = 0;
	//Q_CLEANUP( mMainLayout );
	Q_CLEANUP( mTitle );
	Q_CLEANUP( mMap );
	mPlayers.clear();
}

void GSMultiPlayGround::pause()
{
}

void GSMultiPlayGround::resume()
{
}

void GSMultiPlayGround::handleEvents( GameEngine* engine )
{
	Q_UNUSED( engine );
	
	foreach ( Event* event, mEvents )
	{
		switch ( event->type )
		{
			case Event::KeyPress:
			{
				KeyEvent* ke = static_cast<KeyEvent*>( event );
				
				if ( handlePlayersEvent( ke, engine ) )
				{
					continue;
				}
				
				switch ( ke->key )
				{
					case Qt::Key_Escape:
					{
						//engine->ChangeState( GSMultiGamePlay::instance() );
						break;
					}
					case Qt::Key_Return:
					case Qt::Key_Enter:
					{
						/*
						if ( validateState( engine ) )
						{
							//engine->ChangeState( GSMultiGamePlay::instance() );
							qWarning() << "start game...";
						}
						*/
						break;
					}
				}
				
				break;
			}
			case Event::KeyRelease:
			{
				KeyEvent* ke = static_cast<KeyEvent*>( event );
				
				if ( handlePlayersEvent( ke, engine ) )
				{
					continue;
				}
				
				break;
			}
			default:
				break;
		}
	}
}

void GSMultiPlayGround::update( GameEngine* engine )
{
}

bool GSMultiPlayGround::validateState( GameEngine* engine ) const
{
	Q_UNUSED( engine );
	return false;
}

void GSMultiPlayGround::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
}

bool GSMultiPlayGround::handlePlayersEvent( KeyEvent* event, GameEngine* engine )
{
	const PadSettingsList& pads = engine->padsSettings();
	
	foreach ( const uint& i, mPlayers.keys() )
	{
		const PadSettings& pad = pads.at( i );
		PlayerItem* player = mPlayers[ i ];
		
		switch ( event->type )
		{
			case Event::KeyPress:
			{
				if ( pad.isStrokeKeyUsed( event->key ) )
				{
					const Globals::PadStroke stroke = pad.keyStroke( event->key );
					const Globals::PadStrokes strokes = player->padStrokes() | stroke;
					
					player->setPadStrokes( strokes );
					return true;
				}
				
				break;
			}
			case Event::KeyRelease:
			{
				if ( event->autoRepeat )
				{
					break;
				}
				
				if ( pad.isStrokeKeyUsed( event->key ) )
				{
					const Globals::PadStroke stroke = pad.keyStroke( event->key );
					const Globals::PadStrokes strokes = player->padStrokes() &~ stroke;
					
					player->setPadStrokes( strokes );
					return true;
				}
				else if ( pad.isActionKeyUsed( event->key ) )
				{
					/*
					Globals::PadAction action = mPad->keyAction( event->key() );
					handleAction( action );
					*/
				}
				
				break;
			}
			default:
			{
				Q_ASSERT( 0 );
				break;
			}
		}
	}
	
	return false;
}
