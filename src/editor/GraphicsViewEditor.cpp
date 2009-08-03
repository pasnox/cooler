#include "GraphicsViewEditor.h"
#include "MapEditorItem.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>

#include <math.h>

GraphicsViewEditor::GraphicsViewEditor( QWidget* parent )
	: GraphicsView( parent )
{
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

void GraphicsViewEditor::mouseMoveEvent( QMouseEvent* event )
{
	const QPointF pos = mousePos();
	
	emit mouseMoved( pos );
	emit mouseTileMoved( isMouseOnMap( pos ) ? mouseTilePos( mousePos() ) : QPoint() );
	
	GraphicsView::mouseMoveEvent( event );
}

void GraphicsViewEditor::dragMoveEvent( QDragMoveEvent* event )
{
	const QMimeData* mimeData = event->mimeData();
	
	if ( event->proposedAction() == Qt::CopyAction && isMouseOnMap( mousePos() ) &&
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
	
	const int layer = mimeData->data( "application/x-cooler-layer" ).toInt();
	AbstractTile* tile = AbstractTile::fromByteArray( mimeData->data( "application/x-cooler-tile" ) );
	
	mEditedMap->setLayerTile( layer, mouseTilePos( mousePos() ), tile );
	
	GraphicsView::dropEvent( event );
}
