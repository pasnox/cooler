#include "GraphicsView.h"
#include "TilesManager.h"
#include "MapItem.h"
#include "PlayerItem.h"
#include "PadSettings.h"

#include <QGLWidget>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>

#include <math.h>

GraphicsView::GraphicsView( QWidget* parent )
	: QGraphicsView( parent )
{
	setViewport( new QGLWidget );
	mScene = new QGraphicsScene( this );
	mTiles = TilesManager::instance();
	mMap = new MapItem();
	mMap->setPos( 0, 0 );
	
	mScene->addItem( mMap );
	setScene( mScene );
}

GraphicsView::~GraphicsView()
{
}

TilesManager* GraphicsView::tiles() const
{
	return mTiles;
}

MapItem* GraphicsView::map() const
{
	return mMap;
}

PlayersMap GraphicsView::players() const
{
	return mPlayers;
}

void GraphicsView::addPlayer( const QString& name, PlayerItem* player )
{
	Q_ASSERT( mMap );
	Q_ASSERT( !mPlayers.contains( name ) );
	const PlayersPositionMap& pos = mMap->playersPosition();
	
	player->setParentItem( mMap );
	player->setPosAt( 0, mMap->gridToPos( pos.value( player->tile()->name() ) ) );
	mPlayers[ name ] = player;
}

void GraphicsView::keyPressEvent( QKeyEvent* event )
{
	foreach ( PlayerItem* player, mPlayers.values() )
	{
		const PadSettings* pad = player->pad();
		
		if ( pad->isKeyUsed( event->key() ) )
		{
			player->handleKeyboard( event );
		}
	}
	
	QGraphicsView::keyPressEvent( event );
}

void GraphicsView::keyReleaseEvent( QKeyEvent* event )
{
	foreach ( PlayerItem* player, mPlayers.values() )
	{
		if ( event->isAutoRepeat() )
		{
			continue;
		}
		
		const PadSettings* pad = player->pad();
		
		if ( pad->isKeyUsed( event->key() ) )
		{
			player->handleKeyboard( event );
		}
	}
	
	QGraphicsView::keyReleaseEvent( event );
}

void GraphicsView::drawForeground( QPainter* painter, const QRectF& rect )
{
	QGraphicsView::drawForeground( painter, rect );
}
