#include "PlayerItem.h"

#include <TilesManager.h>
#include <MapItem.h>

#include <QTimer>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>

PlayerItem::PlayerItem( AbstractTile* tile, QGraphicsItem* parent )
	: QObject(), MapObjectItem( tile, parent )
{
	setTile( tile );
	mStroke = Globals::DownStroke;
	mStrokeStep = 0;
	mStrokeSpeed = 10;
	mStrokeTimer = new QTimer( this );
	
	mPad.StrokeMapping[ Qt::Key_Up ] = Globals::UpStroke;
	mPad.StrokeMapping[ Qt::Key_Down ] = Globals::DownStroke;
	mPad.StrokeMapping[ Qt::Key_Left ] = Globals::LeftStroke;
	mPad.StrokeMapping[ Qt::Key_Right ] = Globals::RightStroke;
	
	mPad.ActionMapping[ Qt::Key_Q ] = PadSettings::Action1;
	mPad.ActionMapping[ Qt::Key_S ] = PadSettings::Action2;
	mPad.ActionMapping[ Qt::Key_A ] = PadSettings::Action3;
	mPad.ActionMapping[ Qt::Key_Z ] = PadSettings::Action4;
	
	connect( mStrokeTimer, SIGNAL( timeout() ), this, SLOT( strokeTimer_timeout() ) );
}

PlayerItem::~PlayerItem()
{
}

void PlayerItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	MapObjectItem::paint( painter, option, widget );
	
	painter->setPen( Qt::NoPen );
	
	QRect br = boundingRect().toRect();
	QRect bbr = bodyBoundingRect();
	/*
	painter->setBrush( Qt::red );
	painter->drawRect( br );
	
	painter->setBrush( Qt::green );
	painter->drawRect( bbr );
	*/
	painter->setPen( Qt::black );
	painter->setBrush( Qt::SolidPattern );
}

void PlayerItem::setTile( AbstractTile* tile )
{
	Q_ASSERT( mTile->Type == Globals::PlayerTile );
	MapObjectItem::setTile( tile );
	mPlayerTile = static_cast<PlayerTile*>( tile );
	setZValue( Globals::PlayerLayer );
}

QRect PlayerItem::bodyBoundingRect() const
{
	const QRect br = boundingRect().toRect();
	return QRect( br.x(), br.height() /2, br.width(), br.height() /2 );
}

const PadSettings& PlayerItem::padSettings() const
{
	return mPad;
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
			if ( mPad.StrokeMapping.contains( event->key() ) )
			{
				mStroke = mPad.StrokeMapping[ event->key() ];
				
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
			
			if ( mPad.StrokeMapping.contains( event->key() ) )
			{
				if ( mStrokeTimer->isActive() )
				{
					mStrokeTimer->stop();
				}
				
				setPosAt( 0, pos().toPoint() );
			}
			else if ( mPad.ActionMapping.contains( event->key() ) )
			{
				PadSettings::Action action = mPad.ActionMapping[ event->key() ];
				handleAction( action );
			}
			break;
		default:
			Q_ASSERT( 0 );
			break;
	}
}

void PlayerItem::handleAction( PadSettings::Action action )
{
	switch ( action )
	{
		case PadSettings::Action1:
			dropBomb();
			break;
		case PadSettings::Action2:
			break;
		case PadSettings::Action3:
			break;
		case PadSettings::Action4:
			break;
	}
}

void PlayerItem::dropBomb()
{
	/*
	const BombTile bombTile = TilesManager::instance()->bombTiles().values().first();
	QPoint thisPos = mapRectToItem( map(), bodyBoundingRect() ).toRect().topLeft();
	BombItem* bomb = new BombItem( bombTile, scene() );
	bomb->setZValue( Globals::BombLayer );
	bomb->setPos( map()->closestPos( thisPos ) );
	bomb->setParentItem( map() );
	
	//scene()->addItem( bomb );
	*/
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
	map()->mDebugRect.moveTopLeft( map()->mDebugRect.topLeft() +strokablePos );
	
	setPosAt( mStrokeStep, newPos );
}
