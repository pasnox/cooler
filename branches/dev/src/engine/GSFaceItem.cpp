#include "GSFaceItem.h"
#include "AbstractTile.h"

#include <QPixmapCache>
#include <QPainter>

GSFaceItem::GSFaceItem( AbstractTile* tile )
	: GSMenuItem( QString::null, Qt::AlignCenter, 20 )
{
	Q_ASSERT( tile );
	mTile = tile;
	
	updateCachePixmap();
}

GSFaceItem::~GSFaceItem()
{
}

void GSFaceItem::updateCachePixmap()
{
	prepareGeometryChange();
	
	mCacheKey = QString( "FaceItem%1" ).arg( mTile->name() );
	QPixmapCache::insert( mCacheKey, mTile->tile( 0 ) );
	
	updateGeometry();
}

QSizeF GSFaceItem::sizeHint( Qt::SizeHint which, const QSizeF& constraint ) const
{
	switch ( which )
	{
		case Qt::MinimumSize:
		case Qt::PreferredSize:
		{
			QSize size;
			QPixmap pixmap;
			if ( QPixmapCache::find( mCacheKey, pixmap ) )
				size = pixmap.size();
			
			size += QSize( mMargin *2, mMargin *2 );
			return size;
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
	
	const int penWidth = 3;
	QRectF r = boundingRect();
	r.setSize( r.size() -QSize( penWidth, penWidth ) );
	painter->setPen( QPen( QBrush( Qt::red ), penWidth ) );
	painter->drawRect( r );
	
	QPixmap pixmap;
	if ( QPixmapCache::find( mCacheKey, pixmap ) )
	{
		painter->drawPixmap( mMargin, mMargin, pixmap );
	}
}
