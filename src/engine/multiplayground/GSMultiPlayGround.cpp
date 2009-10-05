#include "GSMultiPlayGround.h"

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

void GSMultiPlayGround::Init( GameEngine* engine, const QSizeF& size )
{
	AbstractGameState::Init( engine, size );
	
	mTiles = TilesManager::instance()->tiles( Globals::GameScreenTile );
	mBackgroundValue = 0;
	
	mBackground = mTiles.value( "game screens/multiplayerchoice_background.png" )->tile( 0 );
	
	// main layout
	mMainLayout = new QGraphicsLinearLayout( Qt::Vertical, this );
	mMainLayout->setContentsMargins( 10, 10, 10, 10 );
	mMainLayout->setSpacing( 10 );
	
	// title
	mTitle = new GSMenuItem( tr( "** PlayGround **" ), Qt::AlignCenter, 38 );
	mTitle->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	mMainLayout->addItem( mTitle );
	
	// vertical spacer 1
	mMainLayout->insertStretch( 1, 100 );
	
	// map item
	
	// vertical spacer 2
	//mMainLayout->insertStretch( 3, 100 );
}

void GSMultiPlayGround::Cleanup()
{
	AbstractGameState::Cleanup();
	
	mBackgroundValue = 0;
	mBackground = QPixmap();
	setLayout( 0 );
	mMainLayout = 0;
	//Q_CLEANUP( mMainLayout );
	Q_CLEANUP( mTitle );
	//Q_CLEANUP( mMap );
}

void GSMultiPlayGround::Pause()
{
}

void GSMultiPlayGround::Resume()
{
}

void GSMultiPlayGround::HandleEvents( GameEngine* game )
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
						//game->ChangeState( GSMultiGamePlay::instance() );
						break;
					}
					case Qt::Key_Return:
					case Qt::Key_Enter:
					{
						/*
						if ( validateState( game ) )
						{
							//game->ChangeState( GSMultiGamePlay::instance() );
							qWarning() << "start game...";
						}
						*/
					}
				}
				
				break;
			}
			default:
				break;
		}
	}
}

void GSMultiPlayGround::Update( GameEngine* game )
{
	Q_UNUSED( game );
	
	mBackgroundValue++;
	
	if ( mBackgroundValue > mBackground.height() )
	{
		mBackgroundValue = 0;
	}
}

bool GSMultiPlayGround::validateState( GameEngine* game ) const
{
	Q_UNUSED( game );
	return false;
}

void GSMultiPlayGround::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	AbstractGameState::paint( painter, option, widget );
	painter->drawTiledPixmap( boundingRect(), mBackground, QPointF( -mBackgroundValue, mBackgroundValue ) );
}
