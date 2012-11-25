#include "QAIOHandler.h"

#include <QVariant>
#include <QRect>
#include <QDebug>
#include <QFile>

QAIOHandler::QAIOHandler()
{
}

QAIOHandler::~QAIOHandler()
{
}

bool QAIOHandler::canRead( QIODevice* device )
{
	const qint64 pos = device->pos();
	const bool ok = QAHandlerPrivate().loadDevice( device );
	device->seek( pos );
	return ok;
}

bool QAIOHandler::canRead() const
{
	if ( !nextImageDelay() )
	{
		if ( !mQAHandler.isOpen() )
		{
			if ( mQAHandler.open( device() ) )
			{
				setFormat( "qa" );
				return true;
			}
			
			return false;
		}
	}
	
	bool ok = mQAHandler.chunk( mChunk.FrameId +1 ).isValid();
	return ok;
}

bool QAIOHandler::read( QImage* image )
{
	const qint32 id = mChunk.FrameId +1;
	mChunk = mQAHandler.chunk( id );
	*image = mChunk.pixmap().toImage();
	return mChunk.isValid();
}

int QAIOHandler::currentImageNumber() const
{
	return mChunk.FrameId;
}
/*
QRect QAIOHandler::currentImageRect() const
{
	return QImageIOHandler::currentImageRect();
}

QIODevice* QAIOHandler::device() const
{
	return QImageIOHandler::device();
}

QByteArray QAIOHandler::format() const
{
	return QImageIOHandler::format();
}
*/
int QAIOHandler::imageCount() const
{
	return mQAHandler.frameCount();
}
/*
bool QAIOHandler::jumpToImage( int imageNumber )
{
	return QImageIOHandler::jumpToImage( imageNumber );
}

bool QAIOHandler::jumpToNextImage()
{
	return QImageIOHandler::jumpToNextImage();
}
*/
int QAIOHandler::loopCount() const
{
	return -1;
}

int QAIOHandler::nextImageDelay() const
{
	return mQAHandler.frameDelay();
}

QVariant QAIOHandler::option( ImageOption option ) const
{
	if ( option == Size )
	{
		return mQAHandler.frameSize();
	}
	else if ( option == Animation )
	{
		return true;
	}
	else if ( option == Name )
	{
		return mChunk.Name;
	}
	
	return QVariant();
}

/*
void QAIOHandler::setDevice( QIODevice* device )
{
	QImageIOHandler::setDevice( device );
}

void QAIOHandler::setFormat( const QByteArray& format )
{
	QImageIOHandler::setFormat( format );
}

void QAIOHandler::setFormat( const QByteArray& format ) const
{
	QImageIOHandler::setFormat( format );
}
*/
void QAIOHandler::setOption( ImageOption option, const QVariant& value )
{
	Q_UNUSED( option );
	Q_UNUSED( value );
}

bool QAIOHandler::supportsOption( ImageOption option ) const
{
	return option == Size || option == Animation || option == Name;
}

bool QAIOHandler::write( const QImage& image )
{
	return QImageIOHandler::write( image );
}

QStringList QAPlugin::keys() const
{
    return QStringList() << QLatin1String( "qa" );
}

QImageIOPlugin::Capabilities QAPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if ( format == "qa" || ( device && device->isReadable() && QAIOHandler::canRead( device ) ) )
	{
        return Capabilities( CanRead );
	}
    return 0;
}

QImageIOHandler *QAPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *handler = new QAIOHandler;
    handler->setDevice(device);
    handler->setFormat(format);
    return handler;
}

Q_EXPORT_PLUGIN2(qa, QAPlugin)
