#ifndef GRAPHICSVIEWEDITOR_H
#define GRAPHICSVIEWEDITOR_H

#include "GraphicsView.h"

class MapEditorItem;
class AbstractTile;

class GraphicsViewEditor : public GraphicsView
{
	Q_OBJECT
	
public:
	GraphicsViewEditor( QWidget* parent = 0 );
	virtual ~GraphicsViewEditor();
	
	MapEditorItem* editedMap() const;
	
	QPointF mousePos() const;
	QPoint mouseTilePos( const QPointF& pos ) const;
	bool isMouseOnMap( const QPointF& pos ) const;
	
	void setCurrentLayer( int layer );
	void setCurrentTile( AbstractTile* tile );

protected:
	MapEditorItem* mEditedMap;
	int mLastDropLayer;
	AbstractTile* mLastDropTile;
	
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseMoveEvent( QMouseEvent* event );
	virtual void dragMoveEvent( QDragMoveEvent* event );
	virtual void dropEvent( QDropEvent* event );
	
	void setLayerTile( int layer, const QPoint& gridPos, AbstractTile* tile );

signals:
	void mouseMoved( const QPointF& pos );
	void mouseTileMoved( const QPoint& pos );
};

#endif // GRAPHICSVIEWEDITOR_H
