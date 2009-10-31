#include "GSMultiPlayerSetup.h"
#include "GSMode.h"
#include "GSMultiPlayerChoice.h"
#include "GSStateItem.h"

#include <QGraphicsLinearLayout>
#include <QPainter>

GSMultiPlayerSetup* GSMultiPlayerSetup::mInstance = 0;

GSMultiPlayerSetup* GSMultiPlayerSetup::instance()
{
	if ( !mInstance )
	{
		mInstance = new GSMultiPlayerSetup();
	}
	
	return mInstance;
}

void GSMultiPlayerSetup::init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::init( engine, size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mBackgroundValue = 0;
	
	mBackground = mTiles.value( "game screens/multiplayersetup_background.png" )->tile( 0 );
	const QPixmap cursor = mTiles.value( "game screens/cursor_head.png" )->tile( 0 );
	
	// main layout
	mMainLayout = new QGraphicsLinearLayout( Qt::Vertical, this );
	mMainLayout->setContentsMargins( 10, 10, 10, 10 );
	mMainLayout->setSpacing( 10 );
	
	// title
	mTitle = new GSMenuItem( tr( "Setup Player" ), Qt::AlignCenter, 38 );
	mTitle->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	mMainLayout->addItem( mTitle );
	
	// vertical spacer 1
	mMainLayout->insertStretch( 1, 100 );
	
	// menus layout
	mMenuLayout = new QGraphicsLinearLayout( Qt::Horizontal );
	mMenuLayout->setContentsMargins( 10, 10, 10, 10 );
	mMenuLayout->setSpacing( 10 );
	
	// left menu spacer
	mMenuLayout->insertStretch( 0, 100 );
	
	// properties
	const Qt::Alignment playersAlignFlags = Qt::AlignLeft | Qt::AlignVCenter;
	const int pixelSize = 28;
	
	// left menu
	mPlayersMenu = new GSMenu;
	
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		GSMenuItem* item = mPlayersMenu->addTextItem( cursor, tr( "%1 Player" ).arg( i+1 ), playersAlignFlags, pixelSize );
		item->setActiveColor( QColor( Qt::transparent ) );
		item->setActiveDisabledColor( QColor( Qt::transparent ) );
	}
	
	mPlayersMenu->setSelectedIndex( 0 );
	
	mMenuLayout->addItem( mPlayersMenu );
	
	// right menu
	mStatesMenu = new GSMenu;
	const PlayerList& players = engine->players();
	const PadSettingsList& pads = engine->padsSettings();
	
	for ( uint i = 0; i < Globals::MaxPlayers; i++ )
	{
		GSStateItem* item = new GSStateItem( players.at( i ).state(), pixelSize );
		item->setEnabled( pads.at( i ).isValid() );
		mStatesMenu->addItem( item );
	}
	
	mMenuLayout->addItem( mStatesMenu );
	
	// right menu spacer
	mMenuLayout->insertStretch( 3, 100 );
	
	mMainLayout->addItem( mMenuLayout );
	
	// vertical spacer 2
	mMainLayout->insertStretch( 3, 100 );
}

void GSMultiPlayerSetup::cleanup()
{
	AbstractGameState::cleanup();
	
	mBackgroundValue = 0;
	mBackground = QPixmap();
	setLayout( 0 );
	mMainLayout = 0;
	mMenuLayout = 0;
	//Q_CLEANUP( mMainLayout );
	//Q_CLEANUP( mMenuLayout );
	Q_CLEANUP( mTitle );
	Q_CLEANUP( mPlayersMenu );
	Q_CLEANUP( mStatesMenu );
}

void GSMultiPlayerSetup::pause()
{
}

void GSMultiPlayerSetup::resume()
{
}

void GSMultiPlayerSetup::handleEvents( GameEngine* game )
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
						game->changeState( GSMode::instance() );
						break;
					}
					case Qt::Key_Return:
					case Qt::Key_Enter:
					{
						if ( validateState( game ) )
							game->changeState( GSMultiPlayerChoice::instance() );
						break;
					}
					case Qt::Key_Up:
					{
						mPlayersMenu->selectPreviousItem();
						break;
					}
					case Qt::Key_Down:
					{
						mPlayersMenu->selectNextItem();
						break;
					}
					case Qt::Key_Left:
					{
						const int index = mPlayersMenu->selectedIndex();
						GSStateItem* item = static_cast<GSStateItem*>( mStatesMenu->item( index ) );
						if ( item->isEnabled() )
							item->previousState();
						break;
					}
					case Qt::Key_Right:
					{
						const int index = mPlayersMenu->selectedIndex();
						GSStateItem* item = static_cast<GSStateItem*>( mStatesMenu->item( index ) );
						if ( item->isEnabled() )
							item->nextState();
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

void GSMultiPlayerSetup::update( GameEngine* game )
{
	Q_UNUSED( game );
	
	mBackgroundValue++;
	
	if ( mBackgroundValue > mBackground.height() )
	{
		mBackgroundValue = 0;
	}
}

bool GSMultiPlayerSetup::validateState( GameEngine* game ) const
{
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
	
	return false;
}

void GSMultiPlayerSetup::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( boundingRect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
