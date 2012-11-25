#ifndef QA_H
#define QA_H

#include <QByteArray>
#include <QSize>
#include <QString>
#include <QPixmap>

namespace QA {

enum QAVersion
{
	VersionBad = -1,
	Version1 = 1,
	VersionLast = Version1
};

enum QAFormat
{
	FormatBad = -1,
	FormatPNG,
	FormatJPG
};

/*
	HEADER
*/
struct Header
{
	Header( QA::QAVersion version = QA::VersionBad, QA::QAFormat format = QA::FormatBad );
	bool isValid() const;
	QByteArray formatString() const;
	
	static QByteArray formatString( QA::QAFormat format );
	
	Header& operator=( const QA::Header& other );
	bool operator==( const QA::Header& other ) const;
	bool operator!=( const QA::Header& other ) const;
	
	QA::QAVersion Version;
	QA::QAFormat Format;
	
	static const QByteArray MagicKey;
};

QDataStream& operator<<( QDataStream& stream, const QA::Header& header );
QDataStream& operator>>( QDataStream& stream, QA::Header& header );

/*
	EXTENDEDHEADER
*/
struct ExtendedHeader
{
	ExtendedHeader( const QString& name = QString::null, const QSize& frameSize = QSize(), qint32 frameCount = -1 );
	bool isValid() const;
	
	ExtendedHeader& operator=( const QA::ExtendedHeader& other );
	bool operator==( const QA::ExtendedHeader& other ) const;
	bool operator!=( const QA::ExtendedHeader& other ) const;
	
	QString Name;
	QSize FrameSize;
	qint32 FrameCount;
};

QDataStream& operator<<( QDataStream& stream, const QA::ExtendedHeader& header );
QDataStream& operator>>( QDataStream& stream, QA::ExtendedHeader& header );

/*
	FRAME
*/
struct Frame
{
	Frame( const QString& name = QString::null, qint32 frameId = -1, const QByteArray& data = QByteArray(), const QPoint& origin = QPoint(), const QRect& hitRect = QRect(), const QString& source = QString::null );
	bool isValid() const;
	QPixmap pixmap() const;
	QImage image() const;
	
	Frame& operator=( const QA::Frame& other );
	bool operator==( const QA::Frame& other ) const;
	bool operator!=( const QA::Frame& other ) const;
	
	QString Name;
	qint32 FrameId;
	QByteArray Data;
	QPoint Origin;
	QRect HitRect;
	QString Source;
};

typedef QList<QA::Frame> FrameList;

QDataStream& operator<<( QDataStream& stream, const QA::Frame& frame );
QDataStream& operator>>( QDataStream& stream, QA::Frame& frame );

QByteArray toByteArray( const QImage& image, QA::QAFormat format, int quality = -1, bool* ok = 0 );
QByteArray toByteArray( const QPixmap& pixmap, QA::QAFormat format, int quality = -1, bool* ok = 0 );

};

#endif // QA_H
