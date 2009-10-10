#include "GSFaceItem.h"
#include "AbstractTile.h"

#include <QPixmapCache>
#include <QPainter>

GSFaceItem::GSFaceItem( const TilesMap& tiles, uint playerId )
	: GSGenericStateItem( 0, tiles.count() -1 )
{
	mTiles = &tiles;
	mPlayerId = playerId;
	mMargin = 25;
	
	Q_ASSERT( !tiles.isEmpty() );
	
	updateCachePixmap();
}

GSFaceItem::~GSFaceItem()
{
}

void GSFaceItem::setTile( AbstractTile* tile )
{
	if ( tile )
	{
		mState = mTiles->values().indexOf( tile );
	}
	
	updateCachePixmap();
}

AbstractTile* GSFaceItem::tile() const
{
	return mTiles->values().value( mState );
}

void GSFaceItem::updateCachePixmap()
{
	prepareGeometryChange();
	
	AbstractTile* tile = this->tile();
	
	if ( !tile )
	{
		Q_ASSERT( tile );
		return;
	}
	
	// cache key
	mCacheKey = QString( "FaceItem%1%2%3" ).arg( tile->name() ).arg( mState ).arg( mPlayerId );
	
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
	mText = tr( "P%1" ).arg( mPlayerId +1 );
	
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
	painter.drawPixmap( mMargin, mMargin, tile->tile( 0 ) );
	
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

QSizeF GSFaceItem::sizeHint( Qt::SizeHint which, const QSizeF& constraint ) const
{
	switch ( which )
	{
		case Qt::MinimumSize:
		case Qt::PreferredSize:
		{
			AbstractTile* tile = this->tile();
	
			if ( tile )
			{
				return tile->tile( 0 ).size() +QSize( mMargin *2, mMargin *2 );
			}
			
			Q_ASSERT( tile );
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
