#include "GraphicsViewEditor.h"
#include "MapEditorItem.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>

#include <math.h>

GraphicsViewEditor::GraphicsViewEditor( QWidget* parent )
	: GraphicsView( parent )
{
	mLastDropLayer = -1;
	mLastDropTile = 0;
	delete mMap;
	mMap = 0;
	
	mEditedMap = new MapEditorItem();
	mScene->addItem( mEditedMap );
}

GraphicsViewEditor::~GraphicsViewEditor()
{
}

MapEditorItem* GraphicsViewEditor::editedMap() const
{
	return mEditedMap;
}

QPointF GraphicsViewEditor::mousePos() const
{
	return mapToScene( mapFromGlobal( QCursor::pos() ) );
}

QPoint GraphicsViewEditor::mouseTilePos( const QPointF& pos ) const
{
	const QSize tileSize = mTiles->tileSize();
	const QPoint tilePos( pos.x() /tileSize.width(), pos.y() /tileSize.height() );
	return tilePos;
}

bool GraphicsViewEditor::isMouseOnMap( const QPointF& pos ) const
{
	const QSizeF mapSize = mEditedMap->rect().size();
	return pos.x() >= 0 && pos.x() < mapSize.width() && pos.y() >= 0 && pos.y() < mapSize.height();
}

void GraphicsViewEditor::setCurrentLayer( int layer )
{
	mLastDropLayer = layer;
}

void GraphicsViewEditor::setCurrentTile( AbstractTile* tile )
{
	mLastDropTile = tile;
}

void GraphicsViewEditor::mousePressEvent( QMouseEvent* event )
{
	const QPointF globalPos = mousePos();
	const bool onMap = isMouseOnMap( globalPos );
	
	if ( mLastDropLayer != -1 && onMap )
	{
		const QPoint gridPos = mouseTilePos( globalPos );
		
		if ( mLastDropTile && ( event->button() == Qt::LeftButton ) )
		{
			setLayerTile( mLastDropLayer, gridPos, mLastDropTile );
		}
		else if ( event->button() == Qt::RightButton )
		{
			setLayerTile( mLastDropLayer, gridPos, 0 );
		}
	}
	
	GraphicsView::mousePressEvent( event );
}

void GraphicsViewEditor::mouseMoveEvent( QMouseEvent* event )
{
	const QPointF globalPos = mousePos();
	const QPoint gridPos = mouseTilePos( globalPos );
	const bool onMap = isMouseOnMap( globalPos );
	
	emit mouseMoved( globalPos );
	emit mouseTileMoved( onMap ? mouseTilePos( globalPos ) : QPoint() );
	
	if ( mLastDropLayer != -1 && onMap )
	{
		if ( mLastDropTile && ( event->buttons() & Qt::LeftButton ) )
		{
			setLayerTile( mLastDropLayer, gridPos, mLastDropTile );
		}
		else if ( event->buttons() & Qt::RightButton )
		{
			setLayerTile( mLastDropLayer, gridPos, 0 );
		}
	}
	
	GraphicsView::mouseMoveEvent( event );
}

void GraphicsViewEditor::dragMoveEvent( QDragMoveEvent* event )
{
	const QMimeData* mimeData = event->mimeData();
	const bool onMap = isMouseOnMap( mousePos() );
	
	if ( ( event->proposedAction() == Qt::CopyAction ) && onMap &&
		mimeData->hasFormat( "application/x-cooler-layer" ) &&
		mimeData->hasFormat( "application/x-cooler-tile" ) )
	{
		event->acceptProposedAction();
		return;
	}
	
	GraphicsView::dragMoveEvent( event );
}

void GraphicsViewEditor::dropEvent( QDropEvent* event )
{
	const QMimeData* mimeData = event->mimeData();
	
	if ( !mimeData->hasFormat( "application/x-cooler-layer" ) ||
		!mimeData->hasFormat( "application/x-cooler-tile" ) )
	{
		GraphicsView::dropEvent( event );
		return;
	}
	
	const QPoint gridPos = mouseTilePos( mousePos() );
	mLastDropLayer = mimeData->data( "application/x-cooler-layer" ).toInt();
	mLastDropTile = AbstractTile::fromByteArray( mimeData->data( "application/x-cooler-tile" ) );
	
	setLayerTile( mLastDropLayer, gridPos, mLastDropTile );
	
	GraphicsView::dropEvent( event );
}

void GraphicsViewEditor::setLayerTile( int layer, const QPoint& gridPos, AbstractTile* tile )
{
	mEditedMap->setLayerTile( layer, gridPos, tile );
}
