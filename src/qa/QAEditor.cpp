#include "QAEditor.h"
#include "HeaderProperties.h"
#include "QAFrameProperties.h"
#include "QAFrameModel.h"
#include "ImageColorPicker.h"
#include "ImageSplitter.h"
#include "pIconManager.h"

#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QClipboard>
#include <QDebug>

QAEditor::QAEditor( QWidget* parent )
	: QMainWindow( parent ), mHandle( this )
{
	setupUi( this );
	setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
	setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
	//setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
	//setCorner( Qt::BottomRightCorner, Qt::LeftDockWidgetArea );
	
	// menu
	aNew->setIcon( pIconManager::icon( "document-new.png" ) );
	aOpen->setIcon( pIconManager::icon( "document-open.png" ) );
	aSave->setIcon( pIconManager::icon( "document-save.png" ) );
	
	// frames menubar
	mbFrame = new QMenuBar( dwFrames );
	QMenu* fmFrame = mbFrame->addMenu( tr( "Frame" ) );
	fmFrame->addAction( pIconManager::icon( "list-add.png" ), tr( "Append..." ), this, SLOT( addFrame() ) )->setData( 0 );
	fmFrame->addAction( pIconManager::icon( "list-add.png" ), tr( "Insert..." ), this, SLOT( addFrame() ) )->setData( 1 );
	fmFrame->addAction( pIconManager::icon( "list-remove.png" ), tr( "Remove..." ), this, SLOT( removeFrame() ) );
	fmFrame->addAction( pIconManager::icon( "color-picker-white.png" ), tr( "Set transparent pixel..." ), this, SLOT( setTransparentPixel() ) );
	hlFrames->setMenuBar( mbFrame );
	
	mHeaderProperties = new QA::HeaderProperties( &mHandle, tpbHeader, this );
	mFrameProperties = new QA::FrameProperties( &mHandle, tpbFrames, this );
	
	mFrameModel = new QA::FrameModel( &mHandle, this );
	lvFrames->setModel( mFrameModel );
	
	connect( lvFrames->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvFrames_selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
	connect( &mHandle, SIGNAL( contentChanged() ), mHeaderProperties, SLOT( readValues() ) );
	connect( &mHandle, SIGNAL( contentChanged() ), mFrameProperties, SLOT( readValues() ) );
	
	aNew->trigger();
}

QAEditor::~QAEditor()
{
}

bool QAEditor::importFrame( int index, const QImage& image, const QString& name, const QString& source )
{
	QA::Frame frame;
	bool result;
	
	frame.Name = name;
	frame.Data = QA::toByteArray( image, mHandle.header().Format, -1, &result );
	frame.Source = source;
	
	if ( !result )
	{
		return false;
	}
	
	frame.HitRect = QRect( QPoint(), image.size() );
	frame.Origin = frame.HitRect.center();
	frame.Origin.ry() = image.height();
	
	return mFrameModel->addFrame( index, frame );
}

bool QAEditor::importFrames( int index, const QImage& image, const QString& source )
{
	const QList<QImage> images = QA::ImageSplitter::getCheckedFrames( image, this );
	
	foreach ( const QImage& img, images )
	{
		if ( !importFrame( index, img, QString( "Frame #%1" ).arg( index ), source ) )
		{
			return false;
		}
		
		index++;
	}
	
	return true;
}

void QAEditor::addFrame()
{
	QAction* action = qobject_cast<QAction*>( sender() );
	const int index = action->data().toInt() == 1 ? lvFrames->selectionModel()->selectedIndexes().value( 0 ).row() : mHandle.frames().count();
	
	QMessageBox msg( this );
	msg.setText( tr( "How to read data ?" ) );
	QAbstractButton* open = msg.addButton( tr( "From local file" ), QMessageBox::YesRole );
	QAbstractButton* ok = msg.addButton( tr( "From clipboard" ), QMessageBox::ApplyRole );
	QAbstractButton* cancel = msg.addButton( tr( "Cancel" ), QMessageBox::RejectRole );
	
	msg.exec();
	
	if ( msg.clickedButton() == cancel )
	{
		return;
	}
	
	const QSize size = mHandle.extendedHeader()->FrameSize;
	QString fn;
	QImage image;
	
	if ( msg.clickedButton() == open )
	{
		fn = QFileDialog::getOpenFileName( this, tr( "Choose the image to open" ) );
		image = QImage( fn );
		
		if ( image.isNull() )
		{
			if ( !fn.isNull() )
			{
				QMessageBox::information( this, QString::null, tr( "Can't open the image." ) );
			}
			
			return;
		}
	}
	else if ( msg.clickedButton() == ok )
	{
		image = qApp->clipboard()->image();
		
		if ( image.isNull() )
		{
			QMessageBox::information( this, QString::null, tr( "The clipboard does not contains a valid/supported image." ) );
			return;
		}
	}
	
	const bool result = size == image.size() ? importFrame( index, image, QFileInfo( fn ).fileName(), fn ) : importFrames( index, image, fn );
	
	if ( result )
	{
		lvFrames->setCurrentIndex( mFrameModel->index( index, 0 ) );
	}
	else
	{
		QMessageBox::information( this, QString::null, tr( "An error occur while importing the image." ) );
	}
}
/*
void QAEditor::importFrames()
{
	QMessageBox msg( this );
	msg.setText( tr( "How to read animation ?" ) );
	QAbstractButton* file = msg.addButton( tr( "From flat animation file" ), QMessageBox::YesRole );
	QAbstractButton* clipboard = msg.addButton( tr( "From flat animation clipboard" ), QMessageBox::ApplyRole );
	QAbstractButton*cancel = msg.addButton( tr( "Cancel" ), QMessageBox::RejectRole );
	
	msg.exec();
	
	if ( msg.clickedButton() == cancel )
	{
		return;
	}
	
	QA::ExtendedHeader* exHeader = mHandle.extendedHeader();
	QImage tiles;
	
	mHandle.clearChunks();
	
	if ( msg.clickedButton() == file )
	{
		const QString fn = QFileDialog::getOpenFileName( this, tr( "Choose the animation to open" ) );
		tiles = QImage( fn );
		
		if ( tiles.isNull() )
		{
			QMessageBox::information( this, QString::null, tr( "Can't open the animation." ) );
			return;
		}
	}
	else if ( msg.clickedButton() == clipboard )
	{
		tiles = qApp->clipboard()->image();
		
		if ( tiles.isNull() )
		{
			QMessageBox::information( this, QString::null, tr( "The clipboard does not contains a valid/supported image." ) );
			return;
		}
	}
	
	if ( tiles.width() %exHeader->FrameSize.width() != 0 || tiles.height() %exHeader->FrameSize.height() != 0 )
	{
		QMessageBox::information( this, QString::null, tr( "The animation is not proportional to the current frame size." ) );
		return;
	}
	
	// create chunks from top left to bottom right
	int index = 0;
	QA::Frame chunk;
	QImage tile;
	bool result;
	
	for ( int y = 0; y < tiles.height(); y += exHeader->FrameSize.height() )
	{
		for ( int x = 0; x < tiles.width(); x += exHeader->FrameSize.width() )
		{
			tile = tiles.copy( QRect( QPoint( x, y ), exHeader->FrameSize ) );
			
			chunk.Name = QString( "Tile #%1" ).arg( index );
			chunk.Data = QA::toByteArray( tile, mHandle.header().Format, -1, &result );
	
			if ( !result )
			{
				QMessageBox::information( this, QString::null, tr( "Can't store the animation data." ) );
				return;
			}
			
			mModel->addChunk( index, chunk );
			index++;
		}
	}
}
*/
void QAEditor::removeFrame()
{
	const int index = lvFrames->selectionModel()->selectedIndexes().value( 0 ).row();
	
	if ( index != -1 )
	{
		mFrameModel->removeFrame( index );
	}
}

void QAEditor::setTransparentPixel()
{
	/*
	const QModelIndex index = lvFrames->selectionModel()->selectedIndexes().value( 0 );
	QA::Frame* chunk = mModel->frame( index.row() );
	
	if ( !chunk )
	{
		return;
	}
	
	bool ok;
	const QRgb pixel = ImageColorPickerDialog::getPixel( this, chunk->image(), &ok );
	
	if ( !ok )
	{
		return;
	}
	
	mHandle.setTransparentPixel( pixel );
	//mModel->refresh();
	*/
}

void QAEditor::lvFrames_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
	Q_UNUSED( deselected );
	const QModelIndex index = selected.indexes().value( 0 );
	mFrameProperties->readValues( index );
}

bool QAEditor::on_aSave_triggered()
{
	if ( mFilePath.isEmpty() )
	{
		mFilePath = QFileDialog::getSaveFileName( this, tr( "Choose a qa file" ) );
	}
	
	if ( !mFilePath.isEmpty() )
	{
		if ( mHandle.saveAs( mFilePath ) )
		{
			return true;
		}
	}
	
	QMessageBox::information( this, QString::null, tr( "An error occur while saving the file." ) );
	
	return false;
}

bool QAEditor::on_aOpen_triggered()
{
	const QString fn = QFileDialog::getOpenFileName( this, tr( "Choose a qa file" ) );
	
	if ( mFilePath == fn )
	{
		return true;
	}
	
	const bool result = mHandle.open( fn );
	
	if ( result )
	{
		mFilePath = fn;
	}
	else
	{
		QMessageBox::information( this, QString::null, tr( "An error occur while opening the file." ) );
	}
	
	return result;
}

void QAEditor::on_aNew_triggered()
{
	mFilePath.clear();
	mHandle.reset();
}

/*
bool QAEditor::apply()
{
	
	bool sizeChanged = false;
	bool formatChanged = false;
	
	// header
	const QA::QAFormat format = QA::QAFormat( cbFormat->itemData( cbFormat->currentIndex() ).toInt() );
	mHandle.header().Version = QA::QAVersion( sbVersion->value() );
	
	if ( mHandle.header().Format != format )
	{
		mHandle.header().Format = format;
		formatChanged = true;
	}
	
	// ex header
	switch ( mHandle.header().Version )
	{
		case QA::Version1:
		{
			QA::ExtendedHeader* exHeader = mHandle.extendedHeader();
			const QSize size = QSize( sbFrameWidth->value(), sbFrameHeight->value() );
			
			exHeader->Name = leName->text();
			exHeader->LoopCount = sbLoopCount->value();
			
			if ( exHeader->FrameSize != size )
			{
				exHeader->FrameSize = size;
				sizeChanged = true;
			}
			
			exHeader->FrameCount = sbFrameCount->value();
			exHeader->FrameDelay = sbFrameDelay->value();
			break;
		}
		case QA::VersionBad:
			Q_ASSERT( 0 );
			QMessageBox::information( this, QString::null, tr( "An error occur while applying the properties." ) );
			return false;
	}
	
	// chunk
	const QModelIndex index = lvFrames->selectionModel()->selectedIndexes().value( 0 );
	
	if ( index.isValid() )
	{
		const QPoint origin = QPoint( sbChunkOriginX->value(), sbChunkOriginY->value() );
		
		mModel->setData( index, leChunkName->text(), QA::Model::NameRole );
		mModel->setData( index, sbChunkFrameId->value(), QA::Model::FrameIdRole );
		mModel->setData( index, origin, QA::Model::OriginRole );
		mModel->setData( index, leChunkSource->text(), QA::Model::SourceRole );
	}
	
	
	// resize tiles if needed
	if ( formatChanged || sizeChanged )
	{
		if ( !mHandle.resizeChunks() )
		{
			QMessageBox::information( this, QString::null, tr( "Can't resize chunks." ) );
			QMessageBox::information( this, QString::null, tr( "An error occur while applying the properties." ) );
			return false;
		}
		else
		{
			mModel->refresh();
		}
	}
	
	return true;
}
*/
