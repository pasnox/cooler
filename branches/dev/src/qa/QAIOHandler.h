#ifndef QAIOHANDLER_H
#define QAIOHANDLER_H

#include <QImageIOHandler>

#include "QAHandler.h"

class QAIOHandler : public QImageIOHandler
{
public:
	QAIOHandler();
	virtual ~QAIOHandler();

	// pure virtual
	virtual bool canRead() const;
	virtual bool read( QImage* image );
	
	// virtual
	virtual int currentImageNumber() const;
	/*
	virtual QRect currentImageRect() const;
	QIODevice* device() const;
	QByteArray format() const;
	*/
	virtual int imageCount() const;
	/*
	virtual bool jumpToImage( int imageNumber );
	virtual bool jumpToNextImage();
	*/
	virtual int loopCount() const;
	virtual int nextImageDelay() const;
	virtual QVariant option( ImageOption option ) const;
	/*
	void setDevice( QIODevice* device );
	void setFormat( const QByteArray& format );
	void setFormat( const QByteArray& format ) const;
	*/
	virtual void setOption( ImageOption option, const QVariant& value );
	virtual bool supportsOption( ImageOption option ) const;
	virtual bool write( const QImage& image );
	
	static bool canRead( QIODevice* device );

protected:
	mutable QAHandler mQAHandler;
	mutable QA::Frame mChunk;
};

#endif // QAIOHANDLER_H

#ifndef QAPLUGIN_H
#define QAPLUGIN_H

class QAPlugin : public QImageIOPlugin
{
public:	
    QStringList keys() const;
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
    QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const;
};

Q_IMPORT_PLUGIN( qa )

#endif // QAPLUGIN_H
