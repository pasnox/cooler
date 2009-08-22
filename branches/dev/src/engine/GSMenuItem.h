#ifndef GSMENUITEM_H
#define GSMENUITEM_H

#include <QGraphicsWidget>
#include <QPainterPath>

class GSMenuItem : public QGraphicsWidget
{
public:
	typedef QPair<QColor, QColor> BrushColors;
	
	GSMenuItem( const QString& text, Qt::Alignment align = Qt::AlignCenter, int pixelSize = 20 );
	GSMenuItem( const QString& text, QGraphicsItem* parent, Qt::Alignment align = Qt::AlignCenter, int pixelSize = 20 );
	virtual ~GSMenuItem();
	
	void setActive( bool active );
	bool isActive() const;
	
	void setText( const QString& text );
	QString text() const;
	
	void setAlignment( Qt::Alignment align = Qt::AlignCenter );
	Qt::Alignment alignment() const;
	
	void setPixelSize( int pixelSize = 20 );
	int pixelSize() const;
	
	void setActiveColor( const QColor& enabled );
	QColor activeColor() const;
	
	void setActiveDisabledColor( const QColor& disabled );
	QColor activeDisabledColor() const;
	
	void setPenColor( const QColor& color );
	QColor penColor() const;
	
	void setBrushColors( const GSMenuItem::BrushColors& colors );
	GSMenuItem::BrushColors brushColors() const;
	
	void setBrushDisabledColors( const GSMenuItem::BrushColors& colors );
	GSMenuItem::BrushColors brushDisabledColors() const;
	
	QString cacheKey() const;
	void updateCachePixmap();

protected:
	QString mCacheKey;
	int mMargin;
	bool mIsActive;
	QString mText;
	Qt::Alignment mAlignment;
	QColor mActiveColor;
	QColor mActiveDisabledColor;
	QColor mPenColor;
	BrushColors mBrushColors;
	BrushColors mBrushDisabledColors;
	
	void init();
	
	virtual QSizeF sizeHint( Qt::SizeHint which, const QSizeF& constraint = QSizeF() ) const;
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

typedef QList<GSMenuItem*> GSMenuItemList;

#endif // GSMENUITEM_H
