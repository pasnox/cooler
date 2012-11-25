#include "QA.h"

#include <QDataStream>
#include <QBuffer>

namespace QA {

const QByteArray Header::MagicKey = "qa";

/*
	Header
*/
Header::Header( QA::QAVersion version, QA::QAFormat format )
{
	Version = version;
	Format = format;
}

bool Header::isValid() const
{
	return Version >= QA::Version1 && Version <= QA::VersionLast && Format != QA::FormatBad;
}

QByteArray Header::formatString() const
{
	return formatString( Format );
}

QByteArray Header::formatString( QA::QAFormat format )
{
	switch ( format )
	{
		case QA::FormatPNG:
			return "png";
		case QA::FormatJPG:
			return "jpg";
		case QA::FormatBad:
		default:
			Q_ASSERT( 0 );
			break;
	}
	
	return QByteArray();
}

Header& Header::operator=( const Header& other )
{
	if( *this != other )
	{
		Version = other.Version;
		Format = other.Format;
	}

	return *this;
}

bool Header::operator==( const Header& other ) const
{
	return Version == other.Version &&
		Format == other.Format;
}

bool Header::operator!=( const Header& other ) const
{
	return !operator==( other );
}

QDataStream& operator<<( QDataStream& stream, const Header& header )
{
	return stream << (qint8)header.Version << (qint8)header.Format;
}

QDataStream& operator>>( QDataStream& stream, Header& header )
{
	qint8 v, f;
	stream >> v >> f;
	header.Version = (QA::QAVersion)v;
	header.Format = (QA::QAFormat)f;
	return stream;
}

/*
	EXTENDEDHEADER
*/
ExtendedHeader::ExtendedHeader( const QString& name, const QSize& frameSize, qint32 frameCount )
{
	Name = name;
	FrameSize = frameSize;
	FrameCount = frameCount;
}

bool ExtendedHeader::isValid() const
{
	return !Name.isNull() && !FrameSize.isNull();
}

ExtendedHeader& ExtendedHeader::operator=( const ExtendedHeader& other )
{
	if( *this != other )
	{
		Name = other.Name;
		FrameSize = other.FrameSize;
		FrameCount = other.FrameCount;
	}

	return *this;
}

bool ExtendedHeader::operator==( const ExtendedHeader& other ) const
{
	return Name == other.Name &&
		FrameSize == other.FrameSize &&
		FrameCount == other.FrameCount;
}

bool ExtendedHeader::operator!=( const ExtendedHeader& other ) const
{
	return !operator==( other );
}

QDataStream& operator<<( QDataStream& stream, const ExtendedHeader& header )
{
	return stream << header.Name << header.FrameSize << header.FrameCount;
}

QDataStream& operator>>( QDataStream& stream, ExtendedHeader& header )
{
	return stream >> header.Name >> header.FrameSize >> header.FrameCount;
}

/*
	FRAME
*/
Frame::Frame( const QString& name, qint32 frameId, const QByteArray& data, const QPoint& origin, const QRect& hitRect, const QString& source )
{
	Name = name;
	FrameId = frameId;
	Data = data;
	Origin = origin;
	HitRect = hitRect;
	Source = source;
}

bool Frame::isValid() const
{
	return !Name.isNull() && FrameId >= 0 && !Data.isNull();
}

QPixmap Frame::pixmap() const
{
	QPixmap pixmap;
	
	pixmap.loadFromData( Data );
	
	return pixmap;
}

QImage Frame::image() const
{
	QImage image;
	
	image.loadFromData( Data );
	
	return image;
}

Frame& Frame::operator=( const Frame& other )
{
	if( *this != other )
	{
		Name = other.Name;
		FrameId = other.FrameId;
		Data = other.Data;
		Origin = other.Origin;
		HitRect = other.HitRect;
		Source = other.Source;
	}

	return *this;
}

bool Frame::operator==( const Frame& other ) const
{
	return Name == other.Name &&
		FrameId == other.FrameId &&
		Data == other.Data &&
		Origin == other.Origin &&
		HitRect == other.HitRect &&
		Source == other.Source;
}

bool Frame::operator!=( const Frame& other ) const
{
	return !operator==( other );
}

QDataStream& operator<<( QDataStream& stream, const Frame& frame )
{
	return stream << frame.Name << frame.FrameId << frame.Data << frame.Origin << frame.HitRect << frame.Source;
}

QDataStream& operator>>( QDataStream& stream, Frame& frame )
{
	return stream >> frame.Name >> frame.FrameId >> frame.Data >> frame.Origin >> frame.HitRect >> frame.Source;
}

QByteArray toByteArray( const QImage& image, QA::QAFormat format, int quality, bool* ok )
{
	QByteArray data;
	QBuffer buffer( &data );
	bool result = false;
	
	if ( !image.isNull() && buffer.open( QIODevice::WriteOnly ) )
	{
		switch ( format )
		{
			case QA::FormatPNG:
				result = image.save( &buffer, "png", quality );
				break;
			case QA::FormatJPG:
				result = image.save( &buffer, "jpg", quality );
				break;
			default:
				break;
		}
	}
	
	if ( ok )
	{
		*ok = result;
	}
	
	return data;
}

QByteArray toByteArray( const QPixmap& pixmap, QA::QAFormat format, int quality, bool* ok )
{
	return toByteArray( pixmap.toImage(), format, quality, ok );
}

};
