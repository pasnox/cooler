#include "TilesRipperPreview.h"

#include <QPainter>

TilesRipperPreview::TilesRipperPreview( QWidget* parent )
	: QLabel( parent )
{
	mOffset = QPoint( 0, 0 );
	mHSpacing = 0;
	mVSpacing = 0;
	mTileSize = QSize( 0, 0 );
	mColor = QColor( Qt::red );
}

TilesRipperPreview::~TilesRipperPreview()
{
}

QList<QPixmap> TilesRipperPreview::tiles() const
{
	QList<QPixmap> pixmaps;
	
	const QPixmap* pix = pixmap();
	
	if ( !pix || mTileSize.width() == 0 || mTileSize.height() == 0 )
	{
		return pixmaps;
	}
	
	for ( int x = mOffset.x(); x < pix->width(); x += mTileSize.width() )
	{
		for ( int y = mOffset.y(); y < pix->height(); y += mTileSize.height() )
		{
			const int xx = x +( ( x /mTileSize.width() ) *mHSpacing );
			const int yy = y +( ( y /mTileSize.height() ) *mVSpacing );
			
			if ( xx +mTileSize.width() >= pix->width() || yy +mTileSize.height() >= pix->height() )
			{
				continue;
			}
			
			pixmaps << pix->copy( QRect( QPoint( xx, yy ), mTileSize ) );
		}
	}
	
	return pixmaps;
}

QPoint TilesRipperPreview::offset() const
{
	return mOffset;
}

int TilesRipperPreview::hSpacing() const
{
	return mHSpacing;
}

int TilesRipperPreview::vSpacing() const
{
	return mVSpacing;
}

QSize TilesRipperPreview::tileSize() const
{
	return mTileSize;
}

void TilesRipperPreview::setOffset( const QPoint& offset )
{
	mOffset = offset;
	update();
}

void TilesRipperPreview::setXOffset( int offset )
{
	mOffset.setX( offset );
	update();
}

void TilesRipperPreview::setYOffset( int offset )
{
	mOffset.setY( offset );
	update();
}

void TilesRipperPreview::setHSpacing( int spacing )
{
	mHSpacing = spacing;
	update();
}

void TilesRipperPreview::setVSpacing( int spacing )
{
	mVSpacing = spacing;
	update();
}

void TilesRipperPreview::setTileSize( const QSize& size )
{
	mTileSize = size;
	update();
}

void TilesRipperPreview::setTileWidth( int width )
{
	mTileSize.setWidth( width );
	update();
}

void TilesRipperPreview::setTileHeight( int height )
{
	mTileSize.setHeight( height );
	update();
}

void TilesRipperPreview::paintEvent( QPaintEvent* event )
{
	QLabel::paintEvent( event );
	
	const QPixmap* pix = pixmap();
	
	if ( !pix || mTileSize.width() == 0 || mTileSize.height() == 0 )
	{
		return;
	}
	
	QPainter painter( this );
	
	painter.setPen( mColor );
	
	for ( int x = mOffset.x(); x < pix->width(); x += mTileSize.width() )
	{
		for ( int y = mOffset.y(); y < pix->height(); y += mTileSize.height() )
		{
			const int xx = x +( ( x /mTileSize.width() ) *mHSpacing );
			const int yy = y +( ( y /mTileSize.height() ) *mVSpacing );
			
			if ( xx +mTileSize.width() -1 >= pix->width() || yy +mTileSize.height() -1 >= pix->height() )
			{
				continue;
			}
			
			painter.drawRect( xx, yy, mTileSize.width() -1, mTileSize.height() -1 );
		}
	}
}
