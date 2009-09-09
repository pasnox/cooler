#include "GSPadSettings.h"
#include "GSMode.h"

#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QGraphicsPixmapItem>

GSPadSettings* GSPadSettings::mInstance = 0;

GSPadSettings* GSPadSettings::instance( const PadSettingsList& pads )
{
	if ( !mInstance )
	{
		mInstance = new GSPadSettings( pads );
	}
	else
	{
		mInstance->mPads = pads;
	}
	
	return mInstance;
}

GSPadSettings::GSPadSettings( const PadSettingsList& pads )
{
	mPads = pads;
}

void GSPadSettings::Init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::Init( engine, size );
	
	mCurrentPadIndex = 1;
	mIsWaitingInput = false;
	mActiveColor = QColor( Qt::yellow );
	mActiveGoodColor = QColor( 117, 247, 88 );
	mActiveBadColor = QColor( 250, 115, 118 );
	mKeyBrushColors = qMakePair( QColor( Qt::yellow ), QColor( Qt::red ).lighter( 150 ) );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mBackgroundValue = 0;
	
	mBackground = mTiles.value( "game screens/padsettings_background.png" )->tile( 0 );
	
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
	mLabelsMenu->addTextItem( tr( "PLAYER" ), alignFlags );
	mLabelsMenu->addTextItem( tr( "UP" ), alignFlags );
	mLabelsMenu->addTextItem( tr( "DOWN" ), alignFlags );
	mLabelsMenu->addTextItem( tr( "LEFT" ), alignFlags );
	mLabelsMenu->addTextItem( tr( "RIGHT" ), alignFlags );
	mLabelsMenu->addTextItem( tr( "BUTTON 1" ), alignFlags );
	mLabelsMenu->addTextItem( tr( "BUTTON 2" ), alignFlags );
	mLabelsMenu->addTextItem( tr( "BUTTON 3" ), alignFlags );
	mLabelsMenu->addTextItem( tr( "BUTTON 4" ), alignFlags );
	mMenuLayout->addItem( mLabelsMenu );
	
	// right menu
	GSMenuItem* item = 0;
	mKeysMenu = new GSMenu;
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	item = mKeysMenu->addTextItem( QString::null );
	item->setActiveColor( mActiveColor );
	item->setBrushColors( mKeyBrushColors );
	mMenuLayout->addItem( mKeysMenu );
	
	// right menu spacer
	mMenuLayout->insertStretch( 3, 100 );
	
	mMainLayout->addItem( mMenuLayout );
	
	// vertical spacer 2
	mMainLayout->insertStretch( 3, 100 );
	
	// form menu
	mFormMenu = new GSMenu( Qt::Horizontal );
	mFormMenu->addTextItem( tr( "Cancel" ) )->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	mFormMenu->addTextItem( tr( "Accept" ) )->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
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
	
	mBackgroundValue = 0;
	mBackground = QPixmap();
	setLayout( 0 );
	mMainLayout = 0;
	mMenuLayout = 0;
	//Q_CLEANUP( mMainLayout );
	//Q_CLEANUP( mMenuLayout );
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
				
				if ( mIsWaitingInput )
				{
					PadSettings& pad = mPads[ mCurrentPadIndex -1 ];
					
					switch ( ke->key )
					{
						case Qt::Key_Escape:
						{
							mIsWaitingInput = false;
							mKeysMenu->item( keyIndex )->setActiveColor( mActiveColor );
							break;
						}
						default:
						{
							if ( keyIndex >= 1 && keyIndex <= 4 )
							{
								Globals::PadStroke stroke = Globals::PadStrokeNo;
								
								switch ( keyIndex )
								{
									case 1:
										stroke = Globals::PadStrokeUp;
										break;
									case 2:
										stroke = Globals::PadStrokeDown;
										break;
									case 3:
										stroke = Globals::PadStrokeLeft;
										break;
									case 4:
										stroke = Globals::PadStrokeRight;
										break;
								}
								
								if ( canChangeStrokeKey( stroke, ke->key ) )
								{
									pad.setStrokeKey( stroke, ke->key );
									mKeysMenu->item( keyIndex )->setText( Globals::keyToString( ke->key ) );
									mKeysMenu->item( keyIndex )->setActiveColor( mActiveColor );
									mIsWaitingInput = false;
								}
								else
								{
									mKeysMenu->item( keyIndex )->setActiveColor( mActiveBadColor );
								}
							}
							else if ( keyIndex >= 5 && keyIndex <= 8 )
							{
								Globals::PadAction action = Globals::PadActionNo;
								
								switch ( keyIndex )
								{
									case 5:
										action = Globals::PadAction1;
										break;
									case 6:
										action = Globals::PadAction2;
										break;
									case 7:
										action = Globals::PadAction3;
										break;
									case 8:
										action = Globals::PadAction4;
										break;
								}
								
								if ( canChangeActionKey( action, ke->key ) )
								{
									pad.setActionKey( action, ke->key );
									mKeysMenu->item( keyIndex )->setText( Globals::keyToString( ke->key ) );
									mKeysMenu->item( keyIndex )->setActiveColor( mActiveColor );
									mIsWaitingInput = false;
								}
								else
								{
									mKeysMenu->item( keyIndex )->setActiveColor( mActiveBadColor );
								}
							}
							break;
						}
					}
					
					continue;
				}
				
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
						if ( keyIndex >= 1 && keyIndex <= 8 )
						{
							mIsWaitingInput = true;
							mKeysMenu->item( keyIndex )->setActiveColor( mActiveGoodColor );
						}
						else if ( formIndex == 0 )
						{
							game->ChangeState( GSMode::instance() );
						}
						else if ( formIndex == 1 )
						{
							game->setPadSettings( mPads );
							game->ChangeState( GSMode::instance() );
						}
						break;
					}
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
	
	mBackgroundValue++;
	
	if ( mBackgroundValue > mBackground.height() )
	{
		mBackgroundValue = 0;
	}
}

void GSPadSettings::loadPadSettings( int index )
{
	mCurrentPadIndex = index;
	PadSettings& pad = mPads[ index -1 ];
	
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

bool GSPadSettings::canChangeStrokeKey( Globals::PadStroke stroke, int key )
{
	for ( int i = 0; i < mPads.count(); i++ )
	{
		const PadSettings& pad = mPads[ i ];
		
		if ( pad.isKeyUsed( key ) )
		{
			if ( mCurrentPadIndex -1 == (uint)i )
			{
				if ( pad.keyStroke( key ) == stroke )
				{
					return true;
				}
			}
			
			return false;;
		}
	}
	
	return true;
}

bool GSPadSettings::canChangeActionKey( Globals::PadAction action, int key )
{
	for ( int i = 0; i < mPads.count(); i++ )
	{
		const PadSettings& pad = mPads[ i ];
		
		if ( pad.isKeyUsed( key ) )
		{			
			if ( mCurrentPadIndex -1 == (uint)i )
			{
				if ( pad.keyAction( key ) == action )
				{
					return true;
				}
			}
			
			return false;
		}
	}
	
	return true;
}

void GSPadSettings::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( boundingRect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
