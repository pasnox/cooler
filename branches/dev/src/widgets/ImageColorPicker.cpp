#include "ImageColorPicker.h"

#include <QLabel>
#include <QMouseEvent>

#include <QDebug>

ImageColorPicker::ImageColorPicker( const QImage& image, QWidget* parent )
	: QScrollArea( parent )
{
	mLabel = new QLabel( this );
	mImage = image;
	mPixel = 0;
	
	setWidget( mLabel );
	setWidgetResizable( true );
	setMouseTracking( true );
	
	mLabel->setFrameStyle( QFrame::NoFrame );
	mLabel->setAlignment( Qt::AlignCenter );
	mLabel->setPixmap( QPixmap::fromImage( mImage ) );
	mLabel->setMouseTracking( true );
}

void ImageColorPicker::mouseMoveEvent( QMouseEvent* event )
{
	QScrollArea::mouseMoveEvent( event );
	
	if ( mImage.isNull() )
	{
		return;
	}
	
	emit pixelHighlighted( event->pos(), mImage.pixel( event->pos() ) );
}
	
void ImageColorPicker::mousePressEvent( QMouseEvent* event )
{
	QScrollArea::mousePressEvent( event );
	
	if ( mImage.isNull() )
	{
		return;
	}
	
	mPixel = mImage.pixel( event->pos() );
	emit pixelClicked( event->pos(), mPixel );
}

QLabel* ImageColorPicker::label() const
{
	return mLabel;
}

QRgb ImageColorPicker::pixel( const QPoint& position ) const
{
	return mImage.pixel( position );
}

QRgb ImageColorPicker::pixel() const
{
	return mPixel;
}

#include <QVBoxLayout>
#include <QPainter>

ImageColorPickerDialog::ImageColorPickerDialog( const QImage& image, QWidget* parent )
	: QDialog( parent )
{
	mPicker = new ImageColorPicker( image, this );
	mPicker->setFrameStyle( QFrame::NoFrame );
	
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->addWidget( mPicker );
	
	int left, top, right, bottom;
	
	getContentsMargins( &left, &top, &right, &bottom );
	setContentsMargins( left, 21, right, bottom );
	setFixedSize( sizeHint() );
	
	connect( mPicker, SIGNAL( pixelHighlighted( const QPoint&, QRgb ) ), this, SLOT( update() ) );
	connect( mPicker, SIGNAL( pixelClicked( const QPoint&, QRgb ) ), this, SLOT( picker_pixelClicked( const QPoint&, QRgb ) ) );
	connect( mPicker, SIGNAL( pixelHighlighted( const QPoint&, QRgb ) ), this, SIGNAL( pixelHighlighted( const QPoint&, QRgb ) ) );
	connect( mPicker, SIGNAL( pixelClicked( const QPoint&, QRgb ) ), this, SIGNAL( pixelClicked( const QPoint&, QRgb ) ) );
}

void ImageColorPickerDialog::paintEvent( QPaintEvent* event )
{
	QDialog::paintEvent( event );
	
	QPainter painter( this );
	
	const QPoint position = mPicker->viewport()->mapFromGlobal( QCursor::pos() );
	
	if ( position.x() < 0 || position.y() < 0 )
	{
		return;
	}
	
	const QRgb pixel = mPicker->pixel( position );
	const QString text = QString( "#%1%2%3%4" )
		.arg( qAlpha( pixel ), 2, 16 )
		.arg( qRed( pixel ), 2, 16 )
		.arg( qGreen( pixel ), 2, 16 )
		.arg( qBlue( pixel ), 2, 16 )
		.replace( " ", "0" ).toUpper();
	QRect pixelRect( 3, 3, mPicker->width(), 21 -3 -3 );
	QRect nameRect( 0, 0, painter.fontMetrics().width( text ), painter.fontMetrics().height() );
	
	pixelRect.moveCenter( rect().center() );
	pixelRect.moveTop( 3 );
	
	nameRect.moveCenter( pixelRect.center() );
	nameRect.moveTop( pixelRect.bottom() +1 );
	
	painter.save();
	
	painter.setPen( Qt::NoPen );
	painter.setBrush( QColor( pixel ) );
	painter.drawRect( pixelRect );
	
	painter.setPen( Qt::black );
	painter.setBrush( Qt::NoBrush );
	painter.drawText( nameRect, Qt::AlignCenter, text );
	
	painter.restore();
}

QRgb ImageColorPickerDialog::pixel() const
{
	return mPicker->pixel();
}

void ImageColorPickerDialog::picker_pixelClicked( const QPoint& position, QRgb pixel )
{
	Q_UNUSED( position );
	Q_UNUSED( pixel );
	
	accept();
}

QRgb ImageColorPickerDialog::getPixel( QWidget* parent, const QImage& image, bool* ok )
{
	ImageColorPickerDialog dlg( image, parent );
	
	const int result = dlg.exec();
	
	if ( ok )
	{
		*ok = result;
	}
	
	return dlg.pixel();
}
