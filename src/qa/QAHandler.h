#ifndef QAHANDLER_H
#define QAHANDLER_H

#include "QA.h"

#include <QSize>
#include <QPoint>
#include <QDataStream>
#include <QPointer>
#include <QPixmap>
#include <QDebug>

/*
	A Qt Animation (qa) consist of a mark "qa" in the first 2 chars of the device,
	followed by a minimal header (QAHeader) that is followed
	by a per version extended header (QAExtendedHeader).
	Then follow FrameCount pairs of chunk header (QAChunk) and data (brut image data as QByteArray).
	The chunk is dependant of the QAHeader Version.
*/

/*
	QAHANDLERPRIVATE
*/
class QAHandlerPrivate
{
	friend class QAHandler;
	friend class QAIOHandler;
	
public:
	~QAHandlerPrivate()
	{
		clear();
	}
	
protected:
	QPointer<QIODevice> mDevice;
	mutable QDataStream mStream;
	QA::Header* mHeader;
	QA::ExtendedHeader* mExtendedHeader;
	qint64 mFirstFramePosition;
	QMap<qint32, qint64> mFramePositions;
	
	QAHandlerPrivate()
	{
		mDevice = 0;
		mStream.setVersion( QDataStream::Qt_4_0 );
		mHeader = 0;
		mExtendedHeader = 0;
		mFirstFramePosition = 0;
	}
	
	void clear()
	{
		mDevice = 0;
		delete mHeader;
		mHeader = 0;
		delete mExtendedHeader;
		mExtendedHeader = 0;
		mFirstFramePosition = 0;
		mFramePositions.clear();
	}
	
	bool loadDevice( QIODevice* device )
	{
		clear();
		
		if ( !device )
		{
			return false;
		}
		
		mDevice = device;
		mHeader = new QA::Header;
		
		if ( !mDevice->reset() )
		{
			return false;
		}
		
		mStream.setDevice( mDevice );
		QByteArray mark;
		
		mStream >> mark;
		
		if ( QString( mark ).compare( QA::Header::MagicKey ) == 0 )
		{
			// get header
			mStream >> *mHeader;
			
			if ( !mHeader->isValid() )
			{
				Q_ASSERT( 0 );
				return false;
			}
			
			// get extended header
			switch ( mHeader->Version )
			{
				case QA::Version1:
					mExtendedHeader = new QA::ExtendedHeader;
					mStream >> *mExtendedHeader;
					
					if ( !mExtendedHeader->isValid() )
					{
						Q_ASSERT( 0 );
						return false;
					}
					
					break;
				default:
					return false;
			}
			
			mFirstFramePosition = mDevice->pos();
			
			// get chunks informations
			QA::Frame chunk;
			for ( qint32 i = 0; i < mExtendedHeader->FrameCount; i++ )
			{
				mFramePositions[ i ] = mDevice->pos();
				mStream >> chunk;
			}
			
			// goes to first frame
			mDevice->seek( mFirstFramePosition );
			
			return true;
		}
		
		return false;
	}
	
	QA::Frame chunk( qint32 index ) const
	{
		QA::Frame chunk;
		
		if ( !mDevice || !mExtendedHeader || index < 0 || index >= mExtendedHeader->FrameCount )
		{
			return chunk;
		}
		
		if ( mDevice->seek( mFramePositions[ index ] ) )
		{
			mStream >> chunk;
		}
		
		return chunk;
	}
	
	QA::FrameList chunks() const
	{
		QA::FrameList chunks;
		QA::Frame chunk;
		
		if ( !mDevice->seek( mFirstFramePosition ) )
		{
			Q_ASSERT( 0 );
			return chunks;
		}
		
		for ( qint32 i = 0; i < mExtendedHeader->FrameCount; i++ )
		{
			mStream >> chunk;
			chunks << chunk;
		}
		
		return chunks;
	}
};

/*
	QAHANDLER
*/
class QAHandler
{
public:
	QAHandler( const QString& filePath = QString::null );
	virtual ~QAHandler();
	
	/*
	QAIOHandler( const QAHandler& other );
	QAIOHandler& operator=( const QAHandler& other );
	bool operator==( const QAHandler& other ) const;
	bool operator!=( const QAHandler& other ) const;
	*/
	
	QIODevice* device() const;
	
	QString filePath() const;
	void setFilePath( const QString& filePath );
	
	bool isOpen() const;
	bool open();
	bool open( QIODevice* device );
	bool createNew();
	
	qint32 frameCount() const;
	QSize frameSize() const;
	qint32 frameDelay() const;
	
	QA::Header* header() const;
	QA::ExtendedHeader* extendedHeader() const;
	QA::Frame chunk( qint32 index ) const;
	QA::FrameList chunks() const;

protected:
	QString mFilePath;
	bool mIsOpen;
	QAHandlerPrivate mHandler;
	static QPointer<QIODevice> mStaticDevice;
};

#endif // QAHANDLER_H
