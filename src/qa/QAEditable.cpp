#include "QAEditable.h"

#include <QFile>
#include <QBitmap>

namespace QA {

Container::Container( QObject* parent )
	: QObject( parent )
{
	mExHeader = 0;
	reset();
}

Container::~Container()
{
}

Container& Container::operator=( const Container& other )
{
	if( *this != other )
	{
		reset();
		
		mHeader = other.mHeader;
		*mExHeader = *other.mExHeader;
		mFrames = other.mFrames;
	}

	return *this;
}

bool Container::operator==( const Container& other ) const
{
	Q_ASSERT( mExHeader );
	Q_ASSERT( other.mExHeader );
	
	return mHeader == other.mHeader &&
		*mExHeader == *other.mExHeader &&
		mFrames == other.mFrames;
}

bool Container::operator!=( const Container& other ) const
{
	return !operator==( other );
}

QA::Header& Container::header() const
{
	return mHeader;
}

QA::ExtendedHeader* Container::extendedHeader() const
{
	Q_ASSERT( mExHeader );
	return mExHeader;
}

QA::FrameList& Container::frames() const
{
	return mFrames;
}

void Container::reset()
{
	// header
	mHeader = QA::Header( QA::Version1, QA::FormatPNG );
	
	// ex header
	delete mExHeader;
	mExHeader = new QA::ExtendedHeader( "<unnamed>", QSize( 64, 64 ), 0 );
	
	// chunks
	mFrames.clear();
	
	emit contentChanged();
}
/*
bool Container::resizeChunks()
{
	const QSize size = mExHeader->FrameSize;
	bool result;
	
	for ( int i = 0; i < mFrames.count(); i++ )
	{
		QA::Frame& chunk = mFrames[ i ];
		
		if ( QFile::exists( chunk.Source ) )
		{
			chunk.Data = QA::toByteArray( QImage( chunk.Source ).scaled( size, Qt::KeepAspectRatio, Qt::SmoothTransformation ), mHeader.Format, -1, &result );
		}
		else
		{
			chunk.Data = QA::toByteArray( chunk.pixmap().scaled( size, Qt::KeepAspectRatio, Qt::SmoothTransformation ), mHeader.Format, -1, &result );
		}
		
		if ( !result )
		{
			Q_ASSERT( 0 );
			return false;
		}
	}
	
	return true;
}

bool Container::setTransparentPixel( QRgb pixel )
{
	bool result;
	
	for ( int i = 0; i < mFrames.count(); i++ )
	{
		QA::Frame& chunk = mFrames[ i ];
		QPixmap pixmap = chunk.pixmap();
		QBitmap mask = pixmap.createHeuristicMask(); //pixmap.createMaskFromColor( QColor( pixel ) );
		
		pixmap.setMask( mask );
		
		chunk.Data = QA::toByteArray( pixmap, mHeader.Format, -1, &result );
		
		if ( !result )
		{
			Q_ASSERT( 0 );
			return false;
		}
	}
	
	return true;
}

void Container::clearFrames()
{
	mFrames.clear();
	mExHeader->FrameCount = 0;
}

QA::Frame* Container::frame( int index ) const
{
	return index < 0 || index >= mFrames.count() ? 0 : &mFrames[ index ];
}
*/

void Container::updateFrameIds()
{
	for ( qint32 i = 0; i < mExHeader->FrameCount; i++ )
	{
		mFrames[ i ].FrameId = i;
	}
	
	emit contentChanged();
}

void Container::addFrame( int index, const QA::Frame& frame )
{
	mFrames.insert( index, frame );
	mExHeader->FrameCount++;
	updateFrameIds();
}

void Container::removeFrame( int index )
{
	mFrames.removeAt( index );
	mExHeader->FrameCount--;
	updateFrameIds();
}

bool Container::open( const QString& filePath )
{
	QFile file( filePath );
	
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		return false;
	}
	
	reset();
	
	QDataStream stream( &file );
	QByteArray mark;
	
	stream >> mark;
	
	if ( mark == QA::Header::MagicKey )
	{
		// get header
		stream >> mHeader;
		
		if ( !mHeader.isValid() )
		{
			Q_ASSERT( 0 );
			return false;
		}
		
		// get extended header
		switch ( mHeader.Version )
		{
			case QA::Version1:
				stream >> *mExHeader;
				
				if ( !mExHeader->isValid() )
				{
					Q_ASSERT( 0 );
					return false;
				}
				
				break;
			default:
				return false;
		}
		
		// get frames
		QA::Frame frame;
		
		for ( qint32 i = 0; i < mExHeader->FrameCount; i++ )
		{
			stream >> frame;
			mFrames << frame;
		}
		
		emit contentChanged();
		
		return true;
	}
	
	return false;
}

bool Container::saveAs( const QString& filePath )
{
	const bool ok = createAnimation( &mHeader, mExHeader, mFrames, filePath );
	
	if ( ok )
	{
		emit contentChanged();
	}
	
	return ok;
}

bool Container::createAnimation( QA::Header* header, QA::ExtendedHeader* extendedHeader, const QA::FrameList& chunks, const QString& filePath  )
{
	QFile file( filePath );
	
	if ( !file.open( QIODevice::WriteOnly ) )
	{
		return false;
	}
	
	file.resize( 0 );
	
	QDataStream stream( &file );
	stream.setVersion( QDataStream::Qt_4_0 );
	
	stream << QA::Header::MagicKey << *header << *extendedHeader;
	
	foreach ( const QA::Frame& chunk, chunks )
	{
		stream << chunk;
	}
	
	file.close();
	
	return stream.status() == QDataStream::Ok;
}

};
