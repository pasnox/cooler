#include "UIMapEditor.h"
#include "MapEditorItem.h"

#include <UITilesRipper.h>

#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

UIMapEditor::UIMapEditor( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
	mRipper = 0;
	
	twLayers->addAction( aAddLayer );
	twLayers->addAction( aRemoveLayer );
	twLayers->addAction( aClearLayers );
	/*
	gvView->addAction( aOpenMap );
	gvView->addAction( aSaveMap );
	gvView->addAction( aClearMap );
	*/
}

UIMapEditor::~UIMapEditor()
{
}

void UIMapEditor::on_aOpenMap_triggered()
{
	MapEditorItem* map = gvView->editedMap();
	const QString fn = QFileDialog::getOpenFileName( this, "Choose a map", "." );
	
	if ( fn.isNull() )
	{
		return;
	}
	
	if ( !map->load( fn ) )
	{
		QMessageBox::warning( this, "", tr( "Can't load map" ) );
		return;
	}
	
	const bool leNameLocked = leName->blockSignals( true );
	const bool sbWidthLocked = sbWidth->blockSignals( true );
	const bool sbHeightLocked = sbHeight->blockSignals( true );
	const bool twLayersLocked = twLayers->blockSignals( true );
	
	leName->setText( map->name() );
	sbWidth->setValue( map->size().width() );
	sbHeight->setValue( map->size().height() );
	twLayers->setLayers( map->layers().keys() );
	
	leName->blockSignals( leNameLocked );
	sbWidth->blockSignals( sbWidthLocked );
	sbHeight->blockSignals( sbHeightLocked );
	twLayers->blockSignals( twLayersLocked );
	
	twLayers->setCurrentLayer( -1 );
}

void UIMapEditor::on_aSaveMap_triggered()
{
	MapEditorItem* map = gvView->editedMap();
	const QString fn = QFileDialog::getSaveFileName( this, "Choose a map filename", QString( "%1.ini" ).arg( leName->text() ) );
	
	if ( !fn.isNull() )
	{
		if ( !map->save( fn ) )
		{
			QMessageBox::warning( this, "", tr( "Can't save map" ) );
		}
	}
}

void UIMapEditor::on_aClearMap_triggered()
{
	clearGui();
	gvView->editedMap()->clear();
}

void UIMapEditor::on_aAddLayer_triggered()
{
	twLayers->addLayer();
}

void UIMapEditor::on_aAddSkyLayer_triggered()
{
	twLayers->addSkyLayer();
}

void UIMapEditor::on_aRemoveLayer_triggered()
{
	twLayers->removeLayer();
}

void UIMapEditor::on_aClearLayers_triggered()
{
	twLayers->clearLayers();
}

void UIMapEditor::on_aTilesRipper_triggered()
{
	if ( ! mRipper )
	{
		mRipper = new UITilesRipper( this );
	}
	
	mRipper->show();
}

void UIMapEditor::on_leName_textChanged( const QString& text )
{
	gvView->editedMap()->setName( text );
}

void UIMapEditor::on_sbWidth_valueChanged( int value )
{
	gvView->editedMap()->setSize( QSize( value, sbHeight->value() ) );
}

void UIMapEditor::on_sbHeight_valueChanged( int value )
{
	gvView->editedMap()->setSize( QSize( sbWidth->value(), value ) );
}

void UIMapEditor::on_twLayers_currentLayerChanged( int layer )
{
	twTiles->setCurrentLayer( layer );
	gvView->setCurrentLayer( layer );
	gvView->editedMap()->setCurrentLayer( layer, cbExclusiveLayer->isChecked() );
}

void UIMapEditor::on_twLayers_layerRemoved( int layer )
{
	gvView->editedMap()->removeLayer( layer );
}

void UIMapEditor::on_twLayers_layersCleared()
{
	gvView->editedMap()->clearLayers();
}

void UIMapEditor::on_cbExclusiveLayer_toggled( bool checked )
{
	gvView->editedMap()->setCurrentLayer( twLayers->currentLayer(), checked );
}

void UIMapEditor::on_twTiles_itemSelectionChanged()
{
	const QTreeWidgetItem* item = twTiles->selectedItems().value( 0 );
	AbstractTile* tile = item ? item->data( 0, Qt::UserRole ).value<AbstractTile*>() : 0;
	gvView->setCurrentTile( tile );
}

void UIMapEditor::on_gvView_mouseTileMoved( const QPoint& pos )
{
	sbBar->showMessage( tr( "Current Tile: X: %1 - Y: %2" ).arg( pos.x() ).arg( pos.y() ) );
}

void UIMapEditor::clearGui()
{
	leName->clear();
	sbWidth->setValue( 0 );
	sbHeight->setValue( 0 );
	twLayers->clear();
}
