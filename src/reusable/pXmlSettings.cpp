#include "pXmlSettings.h"

#include <QFileInfo>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QColor>
#include <QDateTime>
#include <QFont>
#include <QKeySequence>
#include <QLineF>
#include <QSizeF>
#include <QRectF>
#include <QSizePolicy>
#include <QTextLength>
#include <QUrl>
#include <QBitmap>
#include <QBuffer>
#include <QDebug>

const QSettings::Format pXmlSettings::XmlFormat = QSettings::registerFormat( "xml", pXmlSettings::readXmlFile, pXmlSettings::writeXmlFile );

pXmlSettings::pXmlSettings( QObject* parent )
	: QSettings( pXmlSettings::fileName( QString::null ), pXmlSettings::XmlFormat, parent )
{
	setIniCodec( "UTF-8" );
}

pXmlSettings::pXmlSettings( const QString& name, QObject* parent )
	: QSettings( pXmlSettings::fileName( name ), pXmlSettings::XmlFormat, parent )
{
	setIniCodec( "UTF-8" );
}

pXmlSettings::~pXmlSettings()
{
}

QString pXmlSettings::fileName( const QString& name )
{
	if ( QFileInfo( name ).isAbsolute() )
	{
		return name;
	}
	
	Q_ASSERT( !QCoreApplication::organizationName().isEmpty() );
	Q_ASSERT( !QCoreApplication::applicationName().isEmpty() );
	
	const QString path = QDesktopServices::storageLocation( QDesktopServices::DataLocation );
	return QString( "%1/%2.xml" ).arg( path ).arg( name.isEmpty() ? QCoreApplication::applicationName() : name );
}

QVariant pXmlSettings::elementToVariant( const QDomElement& element )
{
	const QVariant::Type type = (QVariant::Type)element.attribute( "metatype", QString::number( QVariant::Invalid ) ).toInt();
	const QString value = element.attribute( "value" );
	
	switch ( type )
	{
		case QVariant::Bool:
			return QVariant( value ).toBool();
		case QVariant::Color:
			return QColor( value );
		case QVariant::Date:
			return QDate::fromString( value, Qt::ISODate );
		case QVariant::DateTime:
			return QDateTime::fromString( value, Qt::ISODate );
		case QVariant::Double:
			return value.toDouble();
		case QVariant::Font:
		{
			QFont font;
			font.fromString( value );
			return font;
		}
		case QVariant::Int:
			return value.toInt();
		case QVariant::KeySequence:
			return QKeySequence::fromString( value );
		case QVariant::Line:
		{
			const int x1 = element.attribute( "x1" ).toInt();
			const int y1 = element.attribute( "y1" ).toInt();
			const int x2 = element.attribute( "x2" ).toInt();
			const int y2 = element.attribute( "y2" ).toInt();
			return QLine( x1, y1, x2, y2 );
		}
		case QVariant::LineF:
		{
			const qreal x1 = element.attribute( "x1" ).toDouble();
			const qreal y1 = element.attribute( "y1" ).toDouble();
			const qreal x2 = element.attribute( "x2" ).toDouble();
			const qreal y2 = element.attribute( "y2" ).toDouble();
			return QLineF( x1, y1, x2, y2 );
		}
		case QVariant::Locale:
			return QLocale( value );
		case QVariant::LongLong:
			return value.toLongLong();
		case QVariant::Point:
		{
			const int x = element.attribute( "x" ).toInt();
			const int y = element.attribute( "y" ).toInt();
			return QPoint( x, y );
		}
		case QVariant::PointF:
		{
			const qreal x = element.attribute( "x" ).toDouble();
			const qreal y = element.attribute( "y" ).toDouble();
			return QPointF( x, y );
		}
		case QVariant::Rect:
		{
			const int top = element.attribute( "top" ).toInt();
			const int left = element.attribute( "left" ).toInt();
			const int width = element.attribute( "width" ).toInt();
			const int height = element.attribute( "height" ).toInt();
			return QRect( QPoint( top, left ), QSize( width, height ) );
		}
		case QVariant::RectF:
		{
			const qreal top = element.attribute( "top" ).toDouble();
			const qreal left = element.attribute( "left" ).toDouble();
			const qreal width = element.attribute( "width" ).toDouble();
			const qreal height = element.attribute( "height" ).toDouble();
			return QRectF( QPointF( top, left ), QSizeF( width, height ) );
		}
		case QVariant::RegExp:
		{
			const Qt::CaseSensitivity cs = (Qt::CaseSensitivity)element.attribute( "case" ).toInt();
			const bool min = QVariant( element.attribute( "minimal" ) ).toBool();
			const QString pat = value;
			const QRegExp::PatternSyntax ps = (QRegExp::PatternSyntax)element.attribute( "syntax" ).toInt();
			QRegExp rx( pat, cs, ps );
			rx.setMinimal( min );
			return rx;
		}
		case QVariant::Size:
		{
			const int width = element.attribute( "width" ).toInt();
			const int height = element.attribute( "height" ).toInt();
			return QSize( width, height );
		}
		case QVariant::SizeF:
		{
			const qreal width = element.attribute( "width" ).toDouble();
			const qreal height = element.attribute( "height" ).toDouble();
			return QSizeF( width, height );
		}
		case QVariant::SizePolicy:
		{
			const QSizePolicy::Policy h = (QSizePolicy::Policy)element.attribute( "horizontal" ).toInt();
			const QSizePolicy::Policy v = (QSizePolicy::Policy)element.attribute( "vertical" ).toInt();
			return QSizePolicy( h, v );
		}
		case QVariant::String:
			return value;
		case QVariant::TextLength:
		{
			const QTextLength::Type t = (QTextLength::Type)element.attribute( "type" ).toInt();
			const qreal v = value.toDouble();
			return QTextLength( t, v );
		}
		case QVariant::Time:
			return QTime::fromString( value, Qt::ISODate );
		case QVariant::UInt:
			return value.toUInt();
		case QVariant::ULongLong:
			return value.toULongLong();
		case QVariant::Url:
			return QUrl( value );
		case QVariant::ByteArray:
			return qUncompress( QByteArray::fromBase64( value.toAscii() ) );
			break;
		case QVariant::Pixmap:
		{
			QByteArray data = QByteArray::fromBase64( value.toAscii() );
			QPixmap pixmap;
			pixmap.loadFromData( data, "png" );
			return pixmap;
		}
		case QVariant::Bitmap:
		{
			QByteArray data = QByteArray::fromBase64( value.toAscii() );
			QBitmap pixmap;
			pixmap.loadFromData( data, "png" );
			return pixmap;
		}
		case QVariant::Image:
		{
			QByteArray data = QByteArray::fromBase64( value.toAscii() );
			QImage pixmap;
			pixmap.loadFromData( data, "png" );
			return pixmap;
		}
		
		case QVariant::Matrix:
		case QVariant::Transform:
		case QVariant::Palette:
		case QVariant::Pen:
		case QVariant::StringList:
		
		//case QVariant::PointArray:
		case QVariant::Polygon:
		
		case QVariant::BitArray:
		
		case QVariant::Brush:
		case QVariant::Char:
		case QVariant::Cursor:
		case QVariant::Hash:
		case QVariant::Icon:
		case QVariant::Map:
		case QVariant::List:
		case QVariant::Region:
		case QVariant::TextFormat:
		case QVariant::UserType:
		case QVariant::Invalid:
		case QVariant::LastType:
		{
			const bool ok = element.attributes().count() == 1;
			Q_ASSERT( ok );
			return ok;
		}
	}
	
	return QVariant();
}

bool pXmlSettings::variantToElement( const QVariant& variant, QDomElement& element )
{
	const QVariant::Type type = variant.type();
	const QString valueKey( "value" );
	
	switch ( type )
	{
		case QVariant::Bool:
			element.setAttribute( valueKey, variant.toString() );
			break;
		case QVariant::Color:
			element.setAttribute( valueKey, variant.value<QColor>().name() );
			break;
		case QVariant::Date:
			element.setAttribute( valueKey, variant.value<QDate>().toString( Qt::ISODate ) );
			break;
		case QVariant::DateTime:
			element.setAttribute( valueKey, variant.value<QDateTime>().toString( Qt::ISODate ) );
			break;
		case QVariant::Double:
			element.setAttribute( valueKey, QString::number( variant.toDouble() ) );
			break;
		case QVariant::Font:
			element.setAttribute( valueKey, variant.value<QFont>().toString() );
			break;
		case QVariant::Int:
			element.setAttribute( valueKey, variant.toInt() );
			break;
		case QVariant::KeySequence:
			element.setAttribute( valueKey, variant.value<QKeySequence>().toString() );
			break;
		case QVariant::Line:
		{
			const QLine line = variant.toLine();
			element.setAttribute( "x1", line.x1() );
			element.setAttribute( "y1", line.y1() );
			element.setAttribute( "x2", line.x2() );
			element.setAttribute( "y2", line.y2() );
			break;
		}
		case QVariant::LineF:
		{
			const QLineF line = variant.toLineF();
			element.setAttribute( "x1", line.x1() );
			element.setAttribute( "y1", line.y1() );
			element.setAttribute( "x2", line.x2() );
			element.setAttribute( "y2", line.y2() );
			break;
		}
		case QVariant::Locale:
			element.setAttribute( valueKey, variant.toLocale().name() );
			break;
		case QVariant::LongLong:
			element.setAttribute( valueKey, variant.toLongLong() );
			break;
		case QVariant::Point:
		{
			const QPoint point = variant.toPoint();
			element.setAttribute( "x", point.x() );
			element.setAttribute( "y", point.y() );
			break;
		}
		case QVariant::PointF:
		{
			const QPointF point = variant.toPointF();
			element.setAttribute( "x", point.x() );
			element.setAttribute( "y", point.y() );
			break;
		}
		case QVariant::Rect:
		{
			const QRect rect = variant.toRect();
			element.setAttribute( "top", rect.top() );
			element.setAttribute( "left", rect.left() );
			element.setAttribute( "width", rect.width() );
			element.setAttribute( "height", rect.height() );
			break;
		}
		case QVariant::RectF:
		{
			const QRectF rect = variant.toRectF();
			element.setAttribute( "top", rect.top() );
			element.setAttribute( "left", rect.left() );
			element.setAttribute( "width", rect.width() );
			element.setAttribute( "height", rect.height() );
			break;
		}
		case QVariant::RegExp:
		{
			const QRegExp rx = variant.toRegExp();
			element.setAttribute( "case", rx.caseSensitivity() );
			element.setAttribute( "minimal", rx.isMinimal() );
			element.setAttribute( valueKey, rx.pattern() );
			element.setAttribute( "syntax", rx.patternSyntax() );
			break;
		}
		case QVariant::Size:
		{
			const QSize size = variant.toSize();
			element.setAttribute( "width", size.width() );
			element.setAttribute( "height", size.height() );
			break;
		}
		case QVariant::SizeF:
		{
			const QSizeF size = variant.toSizeF();
			element.setAttribute( "width", size.width() );
			element.setAttribute( "height", size.height() );
			break;
		}
		case QVariant::SizePolicy:
		{
			const QSizePolicy sp = variant.value<QSizePolicy>();
			element.setAttribute( "horizontal", sp.horizontalPolicy() );
			element.setAttribute( "vertical", sp.verticalPolicy() );
			break;
		}
		case QVariant::String:
			element.setAttribute( valueKey, variant.toString() );
			break;
		case QVariant::TextLength:
		{
			const QTextLength tl = variant.value<QTextLength>();
			element.setAttribute( "type", tl.type() );
			element.setAttribute( valueKey, tl.rawValue() );
			break;
		}
		case QVariant::Time:
			element.setAttribute( valueKey, variant.toTime().toString( Qt::ISODate ) );
			break;
		case QVariant::UInt:
			element.setAttribute( valueKey, variant.toUInt() );
			break;
		case QVariant::ULongLong:
			element.setAttribute( valueKey, variant.toULongLong() );
			break;
		case QVariant::Url:
			element.setAttribute( valueKey, variant.toUrl().toString() );
			break;
		case QVariant::ByteArray:
		{
			QByteArray data = qCompress( variant.toByteArray(), 9 ).toBase64();
			QString value = QString::fromAscii( data.constData(), data.length() );
			element.setAttribute( valueKey, value );
			break;
		}
		case QVariant::Pixmap:
		{
			QPixmap pixmap = variant.value<QPixmap>();
			QByteArray data;
			QBuffer buffer( &data );
			buffer.open( QIODevice::WriteOnly );
			pixmap.save( &buffer, "png" );
			buffer.close();
			data = data.toBase64();
			QString value = QString::fromAscii( data.constData(), data.length() );
			element.setAttribute( valueKey, value );
			break;
		}
		case QVariant::Bitmap:
		{
			QBitmap pixmap = variant.value<QBitmap>();
			QByteArray data;
			QBuffer buffer( &data );
			buffer.open( QIODevice::WriteOnly );
			pixmap.save( &buffer, "png" );
			buffer.close();
			data = data.toBase64();
			QString value = QString::fromAscii( data.constData(), data.length() );
			element.setAttribute( valueKey, value );
			break;
		}
		case QVariant::Image:
		{
			QImage pixmap = variant.value<QImage>();
			QByteArray data;
			QBuffer buffer( &data );
			buffer.open( QIODevice::WriteOnly );
			pixmap.save( &buffer, "png" );
			buffer.close();
			data = data.toBase64();
			QString value = QString::fromAscii( data.constData(), data.length() );
			element.setAttribute( valueKey, value );
			break;
		}
		
		case QVariant::Matrix:
		case QVariant::Transform:
		case QVariant::Palette:
		case QVariant::Pen:
		case QVariant::StringList:
		
		//case QVariant::PointArray:
		case QVariant::Polygon:
		
		case QVariant::BitArray:
		case QVariant::Brush:
		case QVariant::Char:
		case QVariant::Cursor:
		case QVariant::Hash:
		case QVariant::Icon:
		case QVariant::Map:
		case QVariant::List:
		case QVariant::Region:
		case QVariant::TextFormat:
		case QVariant::UserType:
		case QVariant::Invalid:
		case QVariant::LastType:
		{
			const bool ok = element.attributes().count() == 1;
			Q_ASSERT( ok );
			return ok;
		}
	}
	
	element.setAttribute( "metatype", type );
	
	return true;
}

bool pXmlSettings::recursiveNodeReader( const QString& path, const QDomElement& element, QSettings::SettingsMap& map )
{
	const QDomNodeList nodes = element.childNodes();
	
	for ( int i = 0; i < nodes.count(); i++ )
	{
		const QDomElement el = nodes.at( i ).toElement();
		QString curPath = path +"/" +el.attribute( "name" );
		
		if ( curPath.startsWith( "/" ) )
		{
			curPath.remove( 0, 1 );
		}
		
		if ( el.isNull() )
		{
			continue;
		}
		
		if ( el.hasAttribute( "value" ) || el.hasAttribute( "metatype" ) )
		{
			map[ curPath ] = elementToVariant( el );
		}
		
		if ( el.hasChildNodes() )
		{
			if ( !recursiveNodeReader( curPath, el, map ) )
			{
				return false;
			}
		}
	}
	
	return true;
}


bool pXmlSettings::readXmlFile( QIODevice& device, QSettings::SettingsMap& map )
{
	QDomDocument document;
	QString msg;
	int line;
	int column;
	
	if ( !document.setContent( device.readAll(), &msg, &line, &column ) )
	{
		qWarning() << "pXmlSettings::readXmlFile : " << msg << " line:" << line << " column:" << column;
		return false;
	}
	
	return pXmlSettings::recursiveNodeReader( QString(), document.documentElement(), map );
}

bool pXmlSettings::writeXmlFile( QIODevice& device, const QSettings::SettingsMap& map )
{
	QDomDocument document( "pXmlSettings" );
	QMap<QString, QDomElement> elements;
	elements[ QString::null ] = document.createElement( "Settings" );
	document.appendChild( elements[ QString::null ] );
	
	foreach ( const QString& key, map.keys() )
	{
		const QStringList parts = key.split( "/", QString::SkipEmptyParts );
		QString path;
		QDomElement parent = elements[ QString::null ];
		
		foreach ( const QString& part, parts )
		{
			path += "/" +part;
			
			if ( path.startsWith( "/" ) )
			{
				path.remove( 0, 1 );
			}
			
			if ( elements.contains( path ) )
			{
				parent = elements[ path ];
			}
			else
			{
				QDomElement element = document.createElement( "node" );
				element.setAttribute( "name", part );
				parent.appendChild( element );
				parent = element;
				elements[ path ] = element;
			}
		}
		
		QDomElement element = elements[ key ];
		variantToElement( map[ key ], element );
	}
	
	return device.write( document.toByteArray( 4 ) ) != -1;
}
