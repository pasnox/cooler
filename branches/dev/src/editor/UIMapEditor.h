#ifndef UIMAPEDITOR_H
#define UIMAPEDITOR_H

#include "ui_UIMapEditor.h"

class UIMapEditor : public QMainWindow, public Ui::UIMapEditor
{
	Q_OBJECT

public:
	UIMapEditor( QWidget* parent = 0 );
	virtual ~UIMapEditor();

protected slots:
	void on_aOpenMap_triggered();
	void on_aSaveMap_triggered();
	void on_aClearMap_triggered();
	
	void on_aAddLayer_triggered();
	void on_aAddSkyLayer_triggered();
	void on_aRemoveLayer_triggered();
	void on_aClearLayers_triggered();
	
	void on_leName_textChanged( const QString& text );
	void on_sbWidth_valueChanged( int value );
	void on_sbHeight_valueChanged( int value );
	void on_twLayers_currentLayerChanged( int layer );
	void on_twLayers_layerRemoved( int layer );
	void on_twLayers_layersCleared();
	void on_cbExclusiveLayer_toggled( bool checked );
	void on_twTiles_itemSelectionChanged();
	
	void on_gvView_mouseTileMoved( const QPoint& pos );

protected:
	void clearGui();
};

#endif // UIMAPEDITOR_H
