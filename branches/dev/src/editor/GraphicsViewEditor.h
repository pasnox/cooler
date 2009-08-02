#ifndef GRAPHICSVIEWEDITOR_H
#define GRAPHICSVIEWEDITOR_H

#include "GraphicsView.h"

class MapEditorItem;

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

protected:
	MapEditorItem* mEditedMap;
	
	virtual void mouseMoveEvent( QMouseEvent* event );
	virtual void dragMoveEvent( QDragMoveEvent* event );
	virtual void dropEvent( QDropEvent* event );

signals:
	void mouseMoved( const QPointF& pos );
	void mouseTileMoved( const QPoint& pos );
};

#endif // GRAPHICSVIEWEDITOR_H
