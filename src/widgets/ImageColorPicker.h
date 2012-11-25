#ifndef IMAGECOLORPICKER_H
#define IMAGECOLORPICKER_H

#include <QScrollArea>
#include <QImage>

class QLabel;

class ImageColorPicker : public QScrollArea
{
	Q_OBJECT
	
public:
	ImageColorPicker( const QImage& image = QImage(), QWidget* parent = 0 );
	
	QLabel* label() const;
	QRgb pixel( const QPoint& position ) const;
	QRgb pixel() const;

protected:
	QLabel* mLabel;
	QImage mImage;
	QRgb mPixel;
	
	virtual void mouseMoveEvent( QMouseEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );
	
signals:
	void pixelHighlighted( const QPoint& position, QRgb pixel );
	void pixelClicked( const QPoint& position, QRgb pixel );
};

#endif // IMAGECOLORPICKER_H

#ifndef IMAGECOLORPICKERDIALOG_H
#define IMAGECOLORPICKERDIALOG_H

#include <QDialog>

class ImageColorPickerDialog : QDialog
{
	Q_OBJECT
	
public:
	static QRgb getPixel( QWidget* parent = 0, const QImage& image = QImage(), bool* ok = 0 );

protected:
	ImageColorPicker* mPicker;
	
	ImageColorPickerDialog( const QImage& image, QWidget* parent = 0 );
	
	virtual void paintEvent( QPaintEvent* event );
	QRgb pixel() const;

protected slots:
	void picker_pixelClicked( const QPoint& position, QRgb pixel );

signals:
	void pixelHighlighted( const QPoint& position, QRgb pixel );
	void pixelClicked( const QPoint& position, QRgb pixel );
};

#endif // IMAGECOLORPICKERDIALOG_H
