#include "GSMultiGamePlay.h"
#include "GSMultiPlayerChoice.h"
#include "GSMultiMapChoice.h"
#include "GSCOMLevelItem.h"
#include "GSOnOffItem.h"
#include "GSBattleItem.h"
#include "GSTimeItem.h"

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QGraphicsPixmapItem>

GSMultiGamePlay* GSMultiGamePlay::mInstance = 0;

GSMultiGamePlay* GSMultiGamePlay::instance()
{
	if ( !mInstance )
	{
		mInstance = new GSMultiGamePlay();
	}
	
	return mInstance;
}

void GSMultiGamePlay::init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::init( engine, size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mBackgroundValue = 0;
	
	mBackground = mTiles.value( "game screens/multiplayerchoice_background.png" )->tile( 0 );
	const QPixmap cursor = mTiles.value( "game screens/cursor_head.png" )->tile( 0 );
	
	// main layout
	mMainLayout = new QGraphicsLinearLayout( Qt::Vertical, this );
	mMainLayout->setContentsMargins( 10, 10, 10, 10 );
	mMainLayout->setSpacing( 10 );
	
	// title
	mTitle = new GSMenuItem( tr( "Select Game Play" ), Qt::AlignCenter, 38 );
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
	const Qt::Alignment optionsAlignFlags = Qt::AlignLeft | Qt::AlignVCenter;
	const int pixelSize = 28;
	
	// left menu
	mOptionsMenu = new GSMenu;
	
	mOptionsMenu->addTextItem( cursor, tr( "COM Level" ), optionsAlignFlags, pixelSize );
	mOptionsMenu->addTextItem( cursor, tr( "Battle" ), optionsAlignFlags, pixelSize );
	mOptionsMenu->addTextItem( cursor, tr( "Time" ), optionsAlignFlags, pixelSize );
	mOptionsMenu->addTextItem( cursor, tr( "Sudden Death" ), optionsAlignFlags, pixelSize );
	mOptionsMenu->addTextItem( cursor, tr( "Bad Bomber" ), optionsAlignFlags, pixelSize );
	mOptionsMenu->addTextItem( cursor, tr( "Soccer Bomb" ), optionsAlignFlags, pixelSize );
	
	foreach ( GSMenuItem* item, mOptionsMenu->items() )
	{
		item->setActiveColor( QColor( Qt::transparent ) );
		item->setActiveDisabledColor( QColor( Qt::transparent ) );
	}
	
	mOptionsMenu->setSelectedIndex( 0 );
	
	mMenuLayout->addItem( mOptionsMenu );
	
	// right menu
	mStatesMenu = new GSMenu;
	GSGenericStateItem* item;
	
	item = new GSCOMLevelItem( Globals::MediumLevel, pixelSize );
	mStatesMenu->addItem( item );
	item = new GSBattleItem( 3, pixelSize );
	mStatesMenu->addItem( item );
	item = new GSTimeItem( Globals::TimeState3, pixelSize );
	mStatesMenu->addItem( item );
	item = new GSOnOffItem( Globals::InterruptStateOff, pixelSize );
	mStatesMenu->addItem( item );
	item = new GSOnOffItem( Globals::InterruptStateOff, pixelSize );
	mStatesMenu->addItem( item );
	item = new GSOnOffItem( Globals::InterruptStateOff, pixelSize );
	mStatesMenu->addItem( item );
	
	mMenuLayout->addItem( mStatesMenu );
	
	// right menu spacer
	mMenuLayout->insertStretch( 3, 100 );
	
	mMainLayout->addItem( mMenuLayout );
	
	// vertical spacer 2
	mMainLayout->insertStretch( 3, 100 );
}

void GSMultiGamePlay::cleanup()
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
	Q_CLEANUP( mOptionsMenu );
	Q_CLEANUP( mStatesMenu );
}

void GSMultiGamePlay::pause()
{
}

void GSMultiGamePlay::resume()
{
}

void GSMultiGamePlay::handleEvents( GameEngine* game )
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
						game->changeState( GSMultiPlayerChoice::instance() );
						break;
					}
					case Qt::Key_Return:
					case Qt::Key_Enter:
					{
						if ( validateState( game ) )
							game->changeState( GSMultiMapChoice::instance() );
						break;
					}
					case Qt::Key_Up:
					{
						mOptionsMenu->selectPreviousItem();
						break;
					}
					case Qt::Key_Down:
					{
						mOptionsMenu->selectNextItem();
						break;
					}
					case Qt::Key_Left:
					{
						const int index = mOptionsMenu->selectedIndex();
						GSGenericStateItem* item = static_cast<GSGenericStateItem*>( mStatesMenu->item( index ) );
						item->previousState();
						break;
					}
					case Qt::Key_Right:
					{
						const int index = mOptionsMenu->selectedIndex();
						GSGenericStateItem* item = static_cast<GSGenericStateItem*>( mStatesMenu->item( index ) );
						item->nextState();
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
}

void GSMultiGamePlay::update( GameEngine* game )
{
	Q_UNUSED( game );
	
	mBackgroundValue++;
	
	if ( mBackgroundValue > mBackground.height() )
	{
		mBackgroundValue = 0;
	}
}

bool GSMultiGamePlay::validateState( GameEngine* game ) const
{
	Q_UNUSED( game );
	return true;
}

void GSMultiGamePlay::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( boundingRect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
