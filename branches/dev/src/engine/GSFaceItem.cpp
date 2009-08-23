#include "GSFaceItem.h"
#include "AbstractTile.h"

#include <QPixmapCache>
#include <QPainter>

GSFaceItem::GSFaceItem( AbstractTile* tile, uint id )
	: GSMenuItem( QString::null, Qt::AlignCenter, 20 )
{
	Q_ASSERT( tile );
	mTile = tile;
	mMargin = 25;
	mId = id;
	
	updateCachePixmap();
}

GSFaceItem::~GSFaceItem()
{
}

void GSFaceItem::updateCachePixmap()
{
	prepareGeometryChange();
	
	// cache key
	mCacheKey = QString( "FaceItem%1%2" ).arg( mTile->name() ).arg( mId );
	
	// pixmap & painter
	const QRectF br = QRectF( QPointF(), sizeHint( Qt::PreferredSize ) );
	QPixmap pixmap( br.size().toSize() );
	pixmap.fill( Qt::transparent );
	QPainter painter( &pixmap );
	painter.setRenderHint( QPainter::Antialiasing );
	
	// frame
	const int penWidth = 3;
	QRectF r = br;
	r.setSize( r.size() -QSize( penWidth, penWidth ) );
	painter.setPen( QPen( QBrush( Qt::red ), penWidth ) );
	painter.drawRect( r );
	
	// text
	const QFontMetricsF fm( font() );
	const QColor penColor = QColor( Qt::black );
	const QColor color1 = QColor( Qt::red );
	const QColor color2 = QColor( Qt::yellow );
	
	QLinearGradient gradient( QPointF( 0, 0 ), QPointF( 0, font().pixelSize() ) );
	gradient.setSpread( QGradient::RepeatSpread );
	gradient.setColorAt( 0, color1 );
	gradient.setColorAt( 1, color2 );
	
	QBrush brush( gradient );
	
	r = br;
	r.moveLeft( br.center().x() -( r.width() /2 ) );
	r.moveTop( br.center().y() -( r.height() /2 ) );
	
	QPointF p = r.topLeft();
	p.setX( p.x() +mMargin );
	p.setY( p.y() +fm.ascent() +mMargin );
	
	QPainterPath path( QPointF( 0, 0 ) );
	path.addText( p, font(), tr( "P%1" ).arg( mId +1 ) );
	
	painter.setPen( penColor );
	painter.setBrush( brush );
	//painter.drawPath( path );
	
	// player
	painter.drawPixmap( mMargin, mMargin, mTile->tile( 0 ) );
	
	// finish painting
	painter.end();
	
	// cache result pixmap
	QPixmapCache::insert( mCacheKey, pixmap );
	
	updateGeometry();
}

QSizeF GSFaceItem::sizeHint( Qt::SizeHint which, const QSizeF& constraint ) const
{
	switch ( which )
	{
		case Qt::MinimumSize:
		case Qt::PreferredSize:
		{
			return mTile->tile( 0 ).size() +QSize( mMargin *2, mMargin *2 );
		}
		case Qt::MaximumSize:
		case Qt::MinimumDescent:
		default:
			break;
	}
	
	return QGraphicsWidget::sizeHint( which, constraint );
}

void GSFaceItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	Q_UNUSED( option );
	Q_UNUSED( widget );
	
	QPixmap pixmap;
	if ( QPixmapCache::find( mCacheKey, pixmap ) )
	{
		painter->drawPixmap( 0, 0, pixmap );
	}
	else
	{
		updateCachePixmap();
	}
}
