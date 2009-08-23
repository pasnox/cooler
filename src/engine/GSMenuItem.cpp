#include "GSMenuItem.h"

#include <QIcon>
#include <QPainter>
#include <QDebug>

#include <QLinearGradient>
#include <QPixmapCache>

GSMenuItem::GSMenuItem( const QString& text, Qt::Alignment align, int pixelSize )
	: QGraphicsWidget()
{
	init();
	
	setText( text );
	setAlignment( align );
	setPixelSize( pixelSize );
}

GSMenuItem::GSMenuItem( const QString& text, QGraphicsItem* parent, Qt::Alignment align, int pixelSize )
	: QGraphicsWidget( parent )
{
	init();
	
	setText( text );
	setAlignment( align );
	setPixelSize( pixelSize );
}

GSMenuItem::~GSMenuItem()
{
	//qWarning( "Deleting %s", mText.toLocal8Bit().constData() );
}

void GSMenuItem::init()
{
	mMargin = 5;
	mIsActive = false;
	
	mActiveColor = QColor( 0, 0, 255, 70 );
	mActiveDisabledColor = QColor( 255, 0, 0, 70 );
	mPenColor = QColor( Qt::black );
	mBrushColors = qMakePair( QColor( Qt::red ), QColor( Qt::yellow ) );
	mBrushDisabledColors = qMakePair( QColor( Qt::darkGray ), QColor( Qt::gray ) );
}

void GSMenuItem::setActive( bool active )
{
	if ( mIsActive == active )
		return;
	
	mIsActive = active;
	updateCachePixmap();
}

bool GSMenuItem::isActive() const
{
	return mIsActive;
}

void GSMenuItem::setText( const QString& text )
{
	if ( mText == text )
		return;
	
	mText = text;
	updateCachePixmap();
}

QString GSMenuItem::text() const
{
	return mText;
}

void GSMenuItem::setAlignment( Qt::Alignment align )
{
	if ( mAlignment == align )
		return;
	
	mAlignment = align;
	updateCachePixmap();
}

Qt::Alignment GSMenuItem::alignment() const
{
	return mAlignment;
}

void GSMenuItem::setPixelSize( int pixelSize )
{
	if ( font().pixelSize() == pixelSize )
		return;
	
	QFont f( font() );
	f.setPixelSize( pixelSize );
	f.setBold( true );
	//f.setStretch( 200 );
	setFont( f );
	updateCachePixmap();
}

int GSMenuItem::pixelSize() const
{
	return font().pixelSize();
}

void GSMenuItem::setActiveColor( const QColor& enabled )
{
	if ( mActiveColor == enabled )
		return;
	
	mActiveColor = enabled;
	updateCachePixmap();
}

QColor GSMenuItem::activeColor() const
{
	return mActiveColor;
}

void GSMenuItem::setActiveDisabledColor( const QColor& disabled )
{
	if ( mActiveDisabledColor == disabled )
		return;
	
	mActiveDisabledColor = disabled;
	updateCachePixmap();
}

QColor GSMenuItem::activeDisabledColor() const
{
	return mActiveDisabledColor;
}

void GSMenuItem::setPenColor( const QColor& color )
{
	if ( mPenColor == color )
		return;
	
	mPenColor = color;
	updateCachePixmap();
}

QColor GSMenuItem::penColor() const
{
	return mPenColor;
}

void GSMenuItem::setBrushColors( const GSMenuItem::BrushColors& colors )
{
	if ( mBrushColors == colors )
		return;
	
	mBrushColors = colors;
	updateCachePixmap();
}

GSMenuItem::BrushColors GSMenuItem::brushColors() const
{
	return mBrushColors;
}

void GSMenuItem::setBrushDisabledColors( const GSMenuItem::BrushColors& colors )
{
	if ( mBrushDisabledColors == colors )
		return;
	
	mBrushDisabledColors = colors;
	updateCachePixmap();
}

GSMenuItem::BrushColors GSMenuItem::brushDisabledColors() const
{
	return mBrushDisabledColors;
}

QString GSMenuItem::cacheKey() const
{
	return mCacheKey;
}

void GSMenuItem::updateCachePixmap()
{
	QRectF br = boundingRect();
	
	if ( br.isNull() )
	{
		br = QRectF( QPointF(), sizeHint( Qt::PreferredSize ) );
	}
	
	prepareGeometryChange();
	
	QString key = QString::number( qHash( QString( "%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12" )
		.arg( mMargin )
		.arg( mIsActive )
		.arg( mText )
		.arg( mAlignment )
		.arg( mActiveColor.name() )
		.arg( mActiveDisabledColor.name() )
		.arg( mPenColor.name() )
		.arg( mBrushColors.first.name() )
		.arg( mBrushColors.second.name() )
		.arg( mBrushDisabledColors.first.name() )
		.arg( mBrushDisabledColors.second.name() )
		.arg( font().toString() ) ) );
	
	QFontMetricsF fm( font() );
	QRectF r = QRectF( QPointF(), sizeHint( Qt::PreferredSize ) );
	
	QPixmap pixmap( br.size().toSize() );
	pixmap.fill( Qt::transparent );
	QPainter painter( &pixmap );
	painter.setRenderHint( QPainter::Antialiasing );
	
	if ( mIsActive )
	{
		painter.setPen( isEnabled() ? mActiveColor : mActiveDisabledColor );
		painter.setBrush( painter.pen().color().darker( 200 ) );
		
		painter.drawRoundedRect( br, 15, 15 );
	}
	
	const QColor color1 = isEnabled() ? mBrushColors.first : mBrushDisabledColors.first;
	const QColor color2 = isEnabled() ? mBrushColors.second : mBrushDisabledColors.second;
	
	QLinearGradient gradient( QPointF( 0, 0 ), QPointF( 0, font().pixelSize() ) );
	gradient.setSpread( QGradient::RepeatSpread );
	gradient.setColorAt( 0, color1 );
	gradient.setColorAt( 1, color2 );
	
	QBrush brush( gradient );
	
	if ( mAlignment & Qt::AlignLeft )
		r.moveLeft( 0 );
	if ( mAlignment & Qt::AlignRight )
		r.moveRight( br.right() );
	if ( mAlignment & Qt::AlignHCenter || mAlignment & Qt::AlignJustify )
		r.moveLeft( br.center().x() -( r.width() /2 ) );
	if ( mAlignment & Qt::AlignTop )
		r.moveTop( 0 );
	if ( mAlignment & Qt::AlignBottom )
		r.moveBottom( br.bottom() );
	if ( mAlignment & Qt::AlignVCenter )
		r.moveTop( br.center().y() -( r.height() /2 ) );
	
	QPointF p = r.topLeft();
	p.setX( p.x() +mMargin );
	p.setY( p.y() +fm.ascent() +mMargin );
	
	QPainterPath path( QPointF( 0, 0 ) );
	path.addText( p, font(), mText );
	
	painter.setPen( mPenColor );
	painter.setBrush( brush );
	painter.drawPath( path );
	painter.end();
	
	QPixmapCache::insert( key, pixmap );
	mCacheKey = key;
	
	updateGeometry();
}

QSizeF GSMenuItem::sizeHint( Qt::SizeHint which, const QSizeF& constraint ) const
{
	switch ( which )
	{
		case Qt::MinimumSize:
		case Qt::PreferredSize:
		{
			QFontMetricsF fm( font() );
			QSizeF size = fm.size( Qt::TextSingleLine, mText );
			size.setWidth( size.width() +( mMargin *2 ) );
			size.setHeight( size.height() +( mMargin *2 ) );
			return size;
		}
		case Qt::MaximumSize:
		case Qt::MinimumDescent:
		default:
			break;
	}
	
	return QGraphicsWidget::sizeHint( which, constraint );
}

void GSMenuItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	Q_UNUSED( option );
	Q_UNUSED( widget );
	
	QPixmap pixmap;
	if ( QPixmapCache::find( mCacheKey, pixmap ) )
	{
		if ( pixmap.size() != boundingRect().size().toSize() )
		{
			updateCachePixmap();
		}
		else
		{
			painter->drawPixmap( 0, 0, pixmap );
		}
	}
	else
	{
		updateCachePixmap();
	}
}
