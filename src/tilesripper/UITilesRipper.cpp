#include "UITilesRipper.h"

#include <QFileDialog>

UITilesRipper::UITilesRipper( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
	
	connect( sbXOffset, SIGNAL( valueChanged( int ) ), trpPreview, SLOT( setXOffset( int ) ) );
	connect( sbYOffset, SIGNAL( valueChanged( int ) ), trpPreview, SLOT( setYOffset( int ) ) );
	connect( sbVSpacing, SIGNAL( valueChanged( int ) ), trpPreview, SLOT( setVSpacing( int ) ) );
	connect( sbHSpacing, SIGNAL( valueChanged( int ) ), trpPreview, SLOT( setHSpacing( int ) ) );
	connect( sbTileWidth, SIGNAL( valueChanged( int ) ), trpPreview, SLOT( setTileWidth( int ) ) );
	connect( sbTileHeight, SIGNAL( valueChanged( int ) ), trpPreview, SLOT( setTileHeight( int ) ) );
}

UITilesRipper::~UITilesRipper()
{
}

void UITilesRipper::on_pbPreview_clicked()
{
	const QList<QPixmap> pixmaps = trpPreview->tiles();
	lwTiles->clear();
	lwTiles->setIconSize( trpPreview->tileSize() );
	
	foreach ( const QPixmap& pixmap, pixmaps )
	{
		QListWidgetItem* item = new QListWidgetItem( lwTiles );
		item->setIcon( pixmap );
	}
}

void UITilesRipper::on_pbExport_clicked()
{
	const QString path = QFileDialog::getExistingDirectory( this, "", tr( "Choose a path to save your tiles" ) );
	
	if ( path.isNull() )
	{
		return;
	}
	
	const QList<QPixmap> pixmaps = trpPreview->tiles();
	const QSize size = QSize( sbExportWidth->value(), sbExportHeight->value() );
	const QString baseName = leExportBaseName->text();
	int index = 0;
	
	foreach ( const QPixmap& pixmap, pixmaps )
	{
		const QString fn = QString( "%1/%2 - %3.png" ).arg( path ).arg( baseName ).arg( index );
		const QPixmap pix = pixmap.scaled( size, Qt::KeepAspectRatio, Qt::SmoothTransformation );
		pix.save( fn );
		index++;
	}
}

void UITilesRipper::on_pbOpen_clicked()
{
	const QString fn = QFileDialog::getOpenFileName( this, "", tr( "Choose the tiles set to open" ) );
	
	if ( fn.isNull() )
	{
		return;
	}
	
	trpPreview->setPixmap( QPixmap( fn ) );
	lFileName->setText( QFileInfo( fn ).fileName() );
}
