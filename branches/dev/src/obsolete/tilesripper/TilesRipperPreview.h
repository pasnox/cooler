#ifndef TILESRIPPERPREVIEW_H
#define TILESRIPPERPREVIEW_H

#include <QLabel>

class TilesRipperPreview : public QLabel
{
	Q_OBJECT
	
public:
	TilesRipperPreview( QWidget* parent = 0 );
	virtual ~TilesRipperPreview();
	
	QList<QPixmap> tiles() const;
	
	QPoint offset() const;
	int hSpacing() const;
	int vSpacing() const;
	QSize tileSize() const;

public slots:
	void setOffset( const QPoint& offset );
	void setXOffset( int offset );
	void setYOffset( int offset );
	void setHSpacing( int spacing );
	void setVSpacing( int spacing );
	void setTileSize( const QSize& size );
	void setTileWidth( int width );
	void setTileHeight( int height );

protected:
	QPoint mOffset;
	int mHSpacing;
	int mVSpacing;
	QSize mTileSize;
	QColor mColor;
	
	void paintEvent( QPaintEvent* event );
};

#endif // TILESRIPPERPREVIEW_H
