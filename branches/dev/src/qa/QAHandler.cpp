#include "QAHandler.h"

#include <QTemporaryFile>
#include <QCoreApplication>
#include <QDebug>

QPointer<QIODevice> QAHandler::mStaticDevice = 0;

QAHandler::QAHandler( const QString& filePath )
{
	mIsOpen = false;
	mFilePath = filePath;
}

QAHandler::~QAHandler()
{
}

/*
QAHandler::QAHandler( const QAHandler& other )
{
	// copy datas there
}

QAHandler& QAHandler::operator=( const QAHandler& other )
{
	// In order to support the operation "Obj = Obj;"
	if( *this != other )
	{
		// Copy datas there
	}

	return *this;
}

bool QAHandler::operator==( const QAHandler& other ) const
{
	// Comparaison of datas here

	return false;	// Change it
}

bool QAHandler::operator!=( const QAHandler& other ) const
{
	return !operator==( other );
}
*/

QIODevice* QAHandler::device() const
{
	return mHandler.mDevice;
}

QString QAHandler::filePath() const
{
	return mFilePath;
}

void QAHandler::setFilePath( const QString& filePath )
{
	if ( mFilePath == filePath )
	{
		return;
	}
	
	mFilePath = filePath;
	mHandler.clear();
	mIsOpen = false;
}

bool QAHandler::isOpen() const
{
	return mIsOpen;
}

bool QAHandler::open()
{
	if ( mFilePath.isEmpty() )
	{
		return false;
	}
	
	mHandler.clear();
	
	delete mStaticDevice;
	mStaticDevice = new QFile( mFilePath, qApp );
	
	if ( !mStaticDevice->open( QIODevice::ReadOnly ) )
	{
		delete mStaticDevice;
		return false;
	}
	
	mIsOpen = mHandler.loadDevice( mStaticDevice );
	return mIsOpen;
}

bool QAHandler::open( QIODevice* device )
{
	mIsOpen = mHandler.loadDevice( device );
	return mIsOpen;
}

bool QAHandler::createNew()
{
	mHandler.clear();
	
	delete mStaticDevice;
	mStaticDevice = new QTemporaryFile( mFilePath, qApp );
	
	if ( !qobject_cast<QTemporaryFile*>( mStaticDevice )->open() )
	{
		delete mStaticDevice;
		return false;
	}
	
	QA::Header* header = new QA::Header( QA::Version1, QA::FormatPNG );
	QA::ExtendedHeader* extendedHeader = new QA::ExtendedHeader;
	
	mHandler.mDevice = mStaticDevice;
	mHandler.mHeader = header;
	mHandler.mExtendedHeader = extendedHeader;
	mHandler.mStream.setDevice( mStaticDevice );
	
	mHandler.mStream << header << extendedHeader;
	mIsOpen = true;
	return mIsOpen;
}

qint32 QAHandler::frameCount() const
{
	return mHandler.mExtendedHeader ? mHandler.mExtendedHeader->FrameCount : 0;
}

QSize QAHandler::frameSize() const
{
	return mHandler.mExtendedHeader ? mHandler.mExtendedHeader->FrameSize : QSize();
}

qint32 QAHandler::frameDelay() const
{
#warning fix me
	return 0; //mHandler.mExtendedHeader ? mHandler.mExtendedHeader->FrameDelay : 0;
}

QA::Header* QAHandler::header() const
{
	return mHandler.mHeader;
}

QA::ExtendedHeader* QAHandler::extendedHeader() const
{
	return mHandler.mExtendedHeader;
}

QA::Frame QAHandler::chunk( qint32 index ) const
{
	return mHandler.chunk( index );
}

QA::FrameList QAHandler::chunks() const
{
	return mHandler.chunks();
}
