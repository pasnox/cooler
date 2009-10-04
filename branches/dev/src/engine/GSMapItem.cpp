#include "GSMapItem.h"
#include "AbstractTile.h"

#include <QPixmapCache>
#include <QPainter>

GSMapItem::GSMapItem( const QFileInfoList& maps )
	: GSGenericStateItem( 0, maps.count() -1 )
{
	mMaps = &maps;
	mMargin = 25;
	
	Q_ASSERT( !maps.isEmpty() );
	
	updateCachePixmap();
}

GSMapItem::~GSMapItem()
{
}

void GSMapItem::setMap( const QFileInfo& map )
{
	const int index = mMaps->indexOf( map );
	
	if ( index != -1 )
	{
		mState = index;
	}
	
	updateCachePixmap();
}

const QFileInfo& GSMapItem::map() const
{
	return (*mMaps)[ mState ];
}

void GSMapItem::updateCachePixmap()
{
	prepareGeometryChange();
	
	const QFileInfo& map = this->map();
	
	if ( map.fileName().isEmpty() )
	{
		Q_ASSERT( 0 );
		return;
	}
	
	// cache key
	mCacheKey = QString( "MapItem%1%2" ).arg( map.fileName() ).arg( mState );
	
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
	mText = map.baseName();
	
	r = br;
	r.moveLeft( br.center().x() -( r.width() /2 ) );
	r.moveTop( br.center().y() -( r.height() /2 ) );
	
	QPointF p = r.topLeft();
	p.setX( ( br.width() -fm.width( mText ) ) /2 );
	p.setY( p.y() +fm.ascent() );
	
	QPainterPath path( QPointF( 0, 0 ) );
	path.addText( p, font(), mText );
	
	painter.setPen( penColor );
	painter.setBrush( brush );
	painter.drawPath( path );
	
	// player
	//painter.drawPixmap( mMargin, mMargin, tile->tile( 0 ) );
	
	// finish painting
	painter.end();
	
	// non active players are grayscaled
	if ( !isEnabled() )
	{
		pixmap = QPixmap::fromImage( Globals::toGrayscale( pixmap.toImage() ) );
	}
	
	// cache result pixmap
	QPixmapCache::insert( mCacheKey, pixmap );
	
	updateGeometry();
}

QSizeF GSMapItem::sizeHint( Qt::SizeHint which, const QSizeF& constraint ) const
{
	switch ( which )
	{
		case Qt::MinimumSize:
		case Qt::PreferredSize:
		{
			const QFileInfo& map = this->map();
	
			if ( !map.fileName().isEmpty() )
			{
				return QSize( 400, 300 ) +QSize( mMargin *2, mMargin *2 );
			}
			
			Q_ASSERT( 0 );
		}
		case Qt::MaximumSize:
		case Qt::MinimumDescent:
		default:
			break;
	}
	
	return QGraphicsWidget::sizeHint( which, constraint );
}

void GSMapItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	Q_UNUSED( option );
	Q_UNUSED( widget );
	
	QPixmap pixmap;
	if ( QPixmapCache::find( mCacheKey, pixmap ) )
	{
		QRectF pr( QPointF(), pixmap.size() );
		pr.moveCenter( boundingRect().center() );
		
		painter->drawPixmap( pr.topLeft(), pixmap );
	}
	else
	{
		updateCachePixmap();
	}
}
