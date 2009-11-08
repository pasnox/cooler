#include "PlayerItem.h"
#include "Player.h"
#include "PlayerTile.h"
#include "MapItem.h"

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
	mProperties.mPlayer = player;
	setZValue( Globals::PlayerLayer );
	setTile( player->tile() );
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
			if ( !mProperties.mPlayerTile )
			{
				break;
			}
			
			// animation update
			mProperties.mFrameStep += .5;
			
			if ( mProperties.mFrameStep >= mProperties.mPlayerTile->steps() )
			{
				mProperties.mFrameStep = 0;
			}
			
			if ( mProperties.mStrokes.testFlag( Globals::PadStrokeUp ) )
			{
				setPixmap( mProperties.mPlayerTile->tile( Globals::PadStrokeUp, mProperties.mFrameStep ) );
			}
			
			if ( mProperties.mStrokes.testFlag( Globals::PadStrokeDown ) )
			{
				setPixmap( mProperties.mPlayerTile->tile( Globals::PadStrokeDown, mProperties.mFrameStep ) );
			}
			
			if ( mProperties.mStrokes.testFlag( Globals::PadStrokeLeft ) )
			{
				setPixmap( mProperties.mPlayerTile->tile( Globals::PadStrokeLeft, mProperties.mFrameStep ) );
			}
			
			if ( mProperties.mStrokes.testFlag( Globals::PadStrokeRight ) )
			{
				setPixmap( mProperties.mPlayerTile->tile( Globals::PadStrokeRight, mProperties.mFrameStep ) );
			}
			
			if ( mProperties.mStrokes == Globals::PadStrokeNo )
			{
				setPixmap( mProperties.mPlayerTile->tile( Globals::PadStrokeDown, 0 ) );
			}
			
			// update stroking
			QPoint steps;
			
			if ( mProperties.mStrokes.testFlag( Globals::PadStrokeDown ) )
			{
				steps.ry() += mProperties.mSpeed;
			}
			
			if ( mProperties.mStrokes.testFlag( Globals::PadStrokeRight ) )
			{
				steps.rx() += mProperties.mSpeed;
			}
			
			if ( mProperties.mStrokes.testFlag( Globals::PadStrokeLeft ) )
			{
				steps.rx() -= mProperties.mSpeed;
			}
			
			if ( mProperties.mStrokes.testFlag( Globals::PadStrokeUp ) )
			{
				steps.ry() -= mProperties.mSpeed;
			}
			
			if ( !steps.isNull() )
			{
				map()->movePlayerBySteps( this, steps );
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
	mProperties.mPlayerTile = static_cast<PlayerTile*>( tile );
}

const PlayerProperties& PlayerItem::properties() const
{
	return mProperties;
}

Globals::PadStrokes PlayerItem::padStrokes() const
{
	return mProperties.mStrokes;
}

void PlayerItem::setPadStrokes( Globals::PadStrokes strokes )
{
	mProperties.mStrokes = strokes;
}

Globals::PadActions PlayerItem::padActions() const
{
	return mProperties.mActions;
}

void PlayerItem::setPadActions( Globals::PadActions actions )
{
	mProperties.mActions = actions;
}
