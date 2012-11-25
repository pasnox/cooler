#include "ImageSplitter.h"
#include "QAAbstractProperties.h"
#include "QAPropertyManager.h"
#include "QAEditorFactory.h"

#include <math.h>

#include <QDialog>
#include <QDialogButtonBox>

namespace QA {

// IMAGESPLITTERMODEL

ImageSplitterModel::ImageSplitterModel( const QImage& image, const QMap<QString, QtVariantProperty*>& properties, QObject* parent )
	: QAbstractItemModel( parent )
{
	mRowCount = 0;
	mImage = &image;
	mProperties = &properties;
	
	connect( mProperties->begin().value()->propertyManager(), SIGNAL( propertyChanged( QtProperty* ) ), this, SLOT( propertiesChanged() ) );
}

int ImageSplitterModel::columnCount( const QModelIndex& parent ) const
{
	return parent.isValid() ? 0 : 1;
}

QVariant ImageSplitterModel::data( const QModelIndex& index, int role ) const
{	
	if ( !index.isValid() )
	{
		return QVariant();
	}
	
	switch ( role )
	{
		case Qt::SizeHintRole:
			return mProperties->value( "size" )->value().toSize() +QSize( 28, 14 );
		case Qt::CheckStateRole:
			return mData.value( index ).value( role, Qt::Unchecked );
		case Qt::DecorationRole:
		{
			const QPoint offset = mProperties->value( "topLeft" )->value().toPoint();
			const QSize size = mProperties->value( "size" )->value().toSize();
			const int columns = ceil( mImage->width() /size.width() );
			const int y = index.row() /columns;
			const int x = index.row() -( y *columns );
			const QPoint pos( x *size.width(), y *size.height() );
			return mImage->copy( QRect( offset +pos, size ) );
		}
		case Qt::DisplayRole:
			return QString::number( index.row() );
	}
	
	return QVariant();
}

QModelIndex ImageSplitterModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( parent.isValid() || row < 0 || row >= mRowCount || column < 0 || column >= columnCount( parent ) )
	{
		return QModelIndex();
	}
	
	return createIndex( row, column, row );
}

QModelIndex ImageSplitterModel::parent( const QModelIndex& index ) const
{
	Q_UNUSED( index );
	return QModelIndex();
}

int ImageSplitterModel::rowCount( const QModelIndex& parent ) const
{
	return !parent.isValid() ? mRowCount : 0;
}

Qt::ItemFlags ImageSplitterModel::flags( const QModelIndex& index ) const
{
	return QAbstractItemModel::flags( index ) | Qt::ItemIsUserCheckable;
}

bool ImageSplitterModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	switch ( role )
	{
		case Qt::CheckStateRole:
			mData[ index ][ role ] = value;
			break;
		default:
			return false;
	}
	
	emit dataChanged( index, index );
	
	return true;
}

void ImageSplitterModel::propertiesChanged()
{
	QtVariantProperty* property = mProperties->value( "size" );
	const QSize size = property->value().toSize();
	mRowCount = size.isNull() || size.width() == 0 || size.height() == 0 ? 0 : ceil( mImage->width() /size.width() ) *ceil( mImage->height() /size.height() );
	emit reset();
}

// IMAGESPLITTER

ImageSplitter::ImageSplitter( const QImage& image, QWidget* parent )
	: QWidget( parent )
{
	Q_ASSERT( !image.isNull() );
	mImage = image;
	mPropertyManager = QA::AbstractProperties::propertyManager();
	mEditorFactory = QA::AbstractProperties::editorFactory();
	
	setupUi( this );
	lPreview->setPixmap( QPixmap::fromImage( mImage ) );
	
	twpEditor->setFactoryForManager( (QtAbstractPropertyManager*)mPropertyManager, (QtAbstractEditorFactory<QtAbstractPropertyManager>*)mEditorFactory );
	
	addProperty( "topLeft", mPropertyManager->addProperty( QVariant::Point, tr( "TopLeft" ) ) );
	addProperty( "size", mPropertyManager->addProperty( QVariant::Size, tr( "Size" ) ) );
	addProperty( "vspacing", mPropertyManager->addProperty( QVariant::Int, tr( "Vertical Spacing" ) ) );
	addProperty( "hspacing", mPropertyManager->addProperty( QVariant::Int, tr( "Horizontal Spacing" ) ) );
	
	mModel = new ImageSplitterModel( mImage, mProperties, this );
	lvFrames->setModel( mModel );
	
	//connect( mPropertyManager, SIGNAL( propertyChanged( QtProperty* ) ), this, SLOT( _q_propertyChanged( QtProperty* ) ) );
	
	mModel->propertiesChanged();
}

ImageSplitter::~ImageSplitter()
{
	qDeleteAll( mProperties );
}

void ImageSplitter::addProperty( const QString& name, QtVariantProperty* property )
{
	mProperties[ name ] = property;
	twpEditor->addProperty( property );
}

QList<QImage> ImageSplitter::checkedFrames() const
{
	QList<QImage> images;
	
	for ( int i = 0; i < mModel->rowCount(); i++ )
	{
		const QModelIndex index = mModel->index( i, 0 );
		
		if ( index.data( Qt::CheckStateRole ).toInt() != Qt::Checked )
		{
			continue;
		}
		
		const QImage image = index.data( Qt::DecorationRole ).value<QImage>();
		
		if ( image.isNull() )
		{
			continue;
		}
		
		images << image;
	}
	
	return images;
}

QList<QImage> ImageSplitter::getCheckedFrames( const QImage& image, QWidget* parent )
{
	QDialog dlg( parent );
	ImageSplitter* is = new ImageSplitter( image, &dlg );
	is->layout()->setMargin( 0 );
	QDialogButtonBox* dbb = new QDialogButtonBox( &dlg );
	dbb->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
	QVBoxLayout* hl = new QVBoxLayout( &dlg );
	hl->addWidget( is );
	hl->addWidget( dbb );
	
	connect( dbb, SIGNAL( rejected() ), &dlg, SLOT( reject() ) );
	connect( dbb, SIGNAL( accepted() ), &dlg, SLOT( accept() ) );
	
	if ( dlg.exec() == QDialog::Accepted )
	{
		return is->checkedFrames();
	}
	
	return QList<QImage>();
}

};
