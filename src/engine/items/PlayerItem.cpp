#include "PlayerItem.h"
#include "Player.h"
#include "PlayerTile.h"

/*
#include "TilesManager.h"
#include "PadSettings.h"
#include "MapItem.h"
*/

/*
#include <QTimer>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
*/

PlayerItem::PlayerItem( const Player* player, QGraphicsItem* parent )
	: QObject(), MapObjectItem( 0, parent )
{
	setZValue( Globals::PlayerLayer );
	mPlayer = player;
	mPlayerTile = 0;
	mStep = 0;
	
	setTile( player->tile() );
	
	/*
	mPad = 0;
	setTile( tile );
	mStroke = Globals::PadStrokeDown;
	mStrokeStep = 0;
	mStrokeSpeed = 10;
	mStrokeTimer = new QTimer( this );
	
	connect( mStrokeTimer, SIGNAL( timeout() ), this, SLOT( strokeTimer_timeout() ) );
	*/
}

PlayerItem::~PlayerItem()
{
}

void PlayerItem::advance( int phase )
{
	switch ( phase )
	{
		case 0:
			break;
		case 1:
		{
			if ( !mPlayerTile )
			{
				break;
			}
			
			mStep += .5;
			
			if ( mStep >= mPlayerTile->steps() )
			{
				mStep = 0;
			}
			
			if ( mStrokes.testFlag( Globals::PadStrokeUp ) )
			{
				setPixmap( mPlayerTile->tile( Globals::PadStrokeUp, mStep ) );
			}
			
			if ( mStrokes.testFlag( Globals::PadStrokeDown ) )
			{
				setPixmap( mPlayerTile->tile( Globals::PadStrokeDown, mStep ) );
			}
			
			if ( mStrokes.testFlag( Globals::PadStrokeLeft ) )
			{
				setPixmap( mPlayerTile->tile( Globals::PadStrokeLeft, mStep ) );
			}
			
			if ( mStrokes.testFlag( Globals::PadStrokeRight ) )
			{
				setPixmap( mPlayerTile->tile( Globals::PadStrokeRight, mStep ) );
			}
			
			break;
		}
	}
}

void PlayerItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	MapObjectItem::paint( painter, option, widget );
}

QRectF PlayerItem::explosiveBoundingRect() const
{
	return MapObjectItem::explosiveBoundingRect();
	/*
	QRectF br = boundingRect();
	int height = br.height() /2;
	br.moveTop( height );
	br.setHeight( height );
	return br;
	*/
}

void PlayerItem::setTile( AbstractTile* tile )
{
	if ( tile->Type != Globals::PlayerTile )
	{
		Q_ASSERT( 0 );
		return;
	}
	
	MapObjectItem::setTile( tile );
	mPlayerTile = static_cast<PlayerTile*>( tile );
}

Globals::PadStrokes PlayerItem::padStrokes() const
{
	return mStrokes;
}

void PlayerItem::setPadStrokes( Globals::PadStrokes strokes )
{
	mStrokes = strokes;
}

Globals::PadActions PlayerItem::padActions() const
{
	return mActions;
}

void PlayerItem::setPadActions( Globals::PadActions actions )
{
	mActions = actions;
}

/*
PadSettings* PlayerItem::pad() const
{
	return mPad;
}

void PlayerItem::setPad( PadSettings* pad )
{
	mPad = pad;
}

void PlayerItem::setPosAt( qreal step, const QPoint& pos )
{
	mStrokeStep = step;
	setPixmap( mPlayerTile->tile( mStroke, mStrokeStep ) );
	setPos( pos );
	update();
}

void PlayerItem::handleKeyboard( QKeyEvent* event )
{	
	switch ( event->type() )
	{
		case QEvent::KeyPress:
			if ( mPad->isStrokeKeyUsed( event->key() ) )
			{
				mStroke = mPad->keyStroke( event->key() );
				
				if ( !mStrokeTimer->isActive() )
				{
					mStrokeTimer->start( mStrokeSpeed );
				}
			}
			break;
		case QEvent::KeyRelease:
			if ( event->isAutoRepeat() )
			{
				return;
			}
			
			if ( mPad->isStrokeKeyUsed( event->key() ) )
			{
				if ( mStrokeTimer->isActive() )
				{
					mStrokeTimer->stop();
				}
				
				setPosAt( 0, pos().toPoint() );
			}
			else if ( mPad->isActionKeyUsed( event->key() ) )
			{
				Globals::PadAction action = mPad->keyAction( event->key() );
				handleAction( action );
			}
			break;
		default:
			Q_ASSERT( 0 );
			break;
	}
}

void PlayerItem::handleAction( Globals::PadAction action )
{
	switch ( action )
	{
		case Globals::PadAction1:
			dropBomb();
			break;
		case Globals::PadAction2:
			break;
		case Globals::PadAction3:
			break;
		case Globals::PadAction4:
			break;
		case Globals::PadActionNo:
			break;
	}
}

void PlayerItem::dropBomb()
{
	const BombTile bombTile = TilesManager::instance()->bombTiles().values().first();
	QPoint thisPos = mapRectToItem( map(), bodyBoundingRect() ).toRect().topLeft();
	BombItem* bomb = new BombItem( bombTile, scene() );
	bomb->setZValue( Globals::BombLayer );
	bomb->setPos( map()->closestPos( thisPos ) );
	bomb->setParentItem( map() );
	
	//scene()->addItem( bomb );
}

void PlayerItem::strokeTimer_timeout()
{
	mStrokeStep += 0.2;
	
	if ( mStrokeStep > mPlayerTile->steps() )
	{
		mStrokeStep = 0;
	}
	
	QPoint newPos = pos().toPoint();
	const QPoint strokablePos = map()->canStrokeTo( this, mStroke );
	
	newPos += strokablePos;
	
	setPosAt( mStrokeStep, newPos );
}
*/
