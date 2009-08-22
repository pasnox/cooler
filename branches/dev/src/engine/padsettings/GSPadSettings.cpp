#include "GSPadSettings.h"
#include "GSMode.h"

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QGraphicsPixmapItem>

GSPadSettings* GSPadSettings::mInstance = 0;

GSPadSettings* GSPadSettings::instance( PadSettingsMap* pads )
{
	if ( !mInstance )
	{
		mInstance = new GSPadSettings( pads );
	}
	
	return mInstance;
}

GSPadSettings::GSPadSettings( PadSettingsMap* pads )
{
	mPads = pads;
}

void GSPadSettings::Init( const QSizeF& size )
{
	AbstractGameState::Init( size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	
	mBackground = mTiles.value( "game screens/padsettings_background.png" )->tile( 0 );
	mBackgroundValue = 0;
	
	mBackgroundTimer = startTimer( 20 );
	mUpdateTimer = startTimer( 20 );
	
	// main layout
	mMainLayout = new QGraphicsLinearLayout( Qt::Vertical, this );
	mMainLayout->setContentsMargins( 10, 10, 10, 10 );
	mMainLayout->setSpacing( 10 );
	
	// title
	mTitle = new GSMenuItem( tr( "Pad Settings" ), Qt::AlignCenter, 38 );
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
	
	// left menu alignment flags
	Qt::Alignment alignFlags = Qt::AlignVCenter | Qt::AlignRight;
	
	// left menu
	mLabelsMenu = new GSMenu;
	mLabelsMenu->addItem( tr( "PLAYER" ), alignFlags );
	mLabelsMenu->addItem( tr( "UP" ), alignFlags );
	mLabelsMenu->addItem( tr( "DOWN" ), alignFlags );
	mLabelsMenu->addItem( tr( "LEFT" ), alignFlags );
	mLabelsMenu->addItem( tr( "RIGHT" ), alignFlags );
	mLabelsMenu->addItem( tr( "BUTTON 1" ), alignFlags );
	mLabelsMenu->addItem( tr( "BUTTON 2" ), alignFlags );
	mLabelsMenu->addItem( tr( "BUTTON 3" ), alignFlags );
	mLabelsMenu->addItem( tr( "BUTTON 4" ), alignFlags );
	mMenuLayout->addItem( mLabelsMenu );
	
	// right menu
	mKeysMenu = new GSMenu;
	mKeysMenu->addItem( QString::null );
	mKeysMenu->addItem( QString::null );
	mKeysMenu->addItem( QString::null );
	mKeysMenu->addItem( QString::null );
	mKeysMenu->addItem( QString::null );
	mKeysMenu->addItem( QString::null );
	mKeysMenu->addItem( QString::null );
	mKeysMenu->addItem( QString::null );
	mKeysMenu->addItem( QString::null );
	mMenuLayout->addItem( mKeysMenu );
	
	// right menu spacer
	mMenuLayout->insertStretch( 3, 100 );
	
	mMainLayout->addItem( mMenuLayout );
	
	// vertical spacer 2
	mMainLayout->insertStretch( 3, 100 );
	
	// form menu
	mFormMenu = new GSMenu( Qt::Horizontal );
	mFormMenu->addItem( tr( "Cancel" ) )->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	mFormMenu->addItem( tr( "Accept" ) )->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	mMainLayout->addItem( mFormMenu );
	mMainLayout->setAlignment( mFormMenu, Qt::AlignRight );
	
	// selected item
	mKeysMenu->setSelectedIndex( 0 );
	
	// load pad settings
	loadPadSettings( 1 );
}

void GSPadSettings::Cleanup()
{
	AbstractGameState::Cleanup();
	
	killTimer( mUpdateTimer );
	mUpdateTimer = -1;
	killTimer( mBackgroundTimer );
	mBackgroundTimer = -1;
	mBackground = QPixmap();
	
	Q_CLEANUP( mMenuLayout );
	Q_CLEANUP( mMainLayout );
	Q_CLEANUP( mTitle );
	Q_CLEANUP( mLabelsMenu );
	Q_CLEANUP( mKeysMenu );
	Q_CLEANUP( mFormMenu );
}

void GSPadSettings::Pause()
{
}

void GSPadSettings::Resume()
{
}

void GSPadSettings::HandleEvents( GameEngine* game )
{
	Q_UNUSED( game );
	
	foreach ( Event* event, mEvents )
	{
		switch ( event->type )
		{
			case Event::KeyPress:
			{
				KeyEvent* ke = static_cast<KeyEvent*>( event );
				const int keyIndex = mKeysMenu->selectedIndex();
				const int formIndex = mFormMenu->selectedIndex();
				
				switch ( ke->key )
				{
					case Qt::Key_Escape:
						game->ChangeState( GSMode::instance() );
						break;
					case Qt::Key_Up:
					{
						if ( keyIndex != -1 )
						{
							if ( keyIndex == 0 )
							{
								mKeysMenu->setSelectedItem( 0 );
								mFormMenu->setSelectedIndex( 1 );
							}
							else
							{
								mKeysMenu->selectPreviousItem();
							}
						}
						else if ( formIndex != -1 )
						{
							mFormMenu->setSelectedItem( 0 );
							mKeysMenu->setSelectedIndex( mKeysMenu->count() -1 );
						}
						
						break;
					}
					case Qt::Key_Down:
					{
						if ( keyIndex != -1 )
						{
							if ( keyIndex == mKeysMenu->count() -1 )
							{
								mKeysMenu->setSelectedItem( 0 );
								mFormMenu->setSelectedIndex( 0 );
							}
							else
							{
								mKeysMenu->selectNextItem();
							}
						}
						else if ( formIndex != -1 )
						{
							mFormMenu->setSelectedItem( 0 );
							mKeysMenu->setSelectedIndex( 0 );
						}
						
						break;
					}
					case Qt::Key_Left:
					{
						if ( keyIndex == 0 )
						{
							if ( mCurrentPadIndex == 1 )
							{
								loadPadSettings( Globals::MaxPlayers );
							}
							else
							{
								loadPadSettings( mCurrentPadIndex -1 );
							}
						}
						else if ( formIndex != -1 )
						{
							if ( formIndex == 0 )
							{
								mFormMenu->setSelectedItem( 0 );
								mKeysMenu->setSelectedIndex( mKeysMenu->count() -1 );
							}
							else
							{
								mFormMenu->selectPreviousItem();
							}
						}
						
						break;
					}
					case Qt::Key_Right:
					{
						if ( keyIndex == 0 )
						{
							if ( mCurrentPadIndex == Globals::MaxPlayers )
							{
								loadPadSettings( 1 );
							}
							else
							{
								loadPadSettings( mCurrentPadIndex +1 );
							}
						}
						else if ( formIndex != -1 )
						{
							if ( formIndex == mFormMenu->count() -1 )
							{
								mFormMenu->setSelectedItem( 0 );
								mKeysMenu->setSelectedIndex( 0 );
							}
							else
							{
								mFormMenu->selectNextItem();
							}
						}
						
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

void GSPadSettings::Update( GameEngine* game )
{
	Q_UNUSED( game );
}

void GSPadSettings::Draw( GameEngine* game )
{
	Q_UNUSED( game );
}

void GSPadSettings::loadPadSettings( int index )
{
	mCurrentPadIndex = index;
	PadSettings& pad = (*mPads)[ index -1 ];
	
	mKeysMenu->item( 0 )->setText( QString( "#%1" ).arg( index ) );
	mKeysMenu->item( 1 )->setText( Globals::keyToString( pad.strokeKey( Globals::PadStrokeUp ) ) );
	mKeysMenu->item( 2 )->setText( Globals::keyToString( pad.strokeKey( Globals::PadStrokeDown ) ) );
	mKeysMenu->item( 3 )->setText( Globals::keyToString( pad.strokeKey( Globals::PadStrokeLeft ) ) );
	mKeysMenu->item( 4 )->setText( Globals::keyToString( pad.strokeKey( Globals::PadStrokeRight ) ) );
	mKeysMenu->item( 5 )->setText( Globals::keyToString( pad.actionKey( Globals::PadAction1 ) ) );
	mKeysMenu->item( 6 )->setText( Globals::keyToString( pad.actionKey( Globals::PadAction2 ) ) );
	mKeysMenu->item( 7 )->setText( Globals::keyToString( pad.actionKey( Globals::PadAction3 ) ) );
	mKeysMenu->item( 8 )->setText( Globals::keyToString( pad.actionKey( Globals::PadAction4 ) ) );
}

void GSPadSettings::timerEvent( QTimerEvent* event )
{
	if ( event->timerId() == mBackgroundTimer )
	{
		mBackgroundValue++;
	
		if ( mBackgroundValue > mBackground.height() )
		{
			mBackgroundValue = 0;
		}
	}
	else if ( event->timerId() == mUpdateTimer )
	{
		update();
	}
}

void GSPadSettings::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->drawTiledPixmap( rect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
	AbstractGameState::paint( painter, option, widget );
}
