#include "Globals.h"

#include <QKeySequence>

QString Globals::relativeFilePath( const QString& path, const QString& fn )
{
	QDir dir( path );
	return dir.relativeFilePath( fn );
}

QString Globals::relativeFilePath( const QFileInfo& path, const QFileInfo& fn )
{
	return relativeFilePath( path.absoluteFilePath(), fn.absoluteFilePath() );
}

QFileInfoList Globals::getFiles( QDir& fromDir, const QStringList& filters )
{
	const QFileInfoList entries = fromDir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name );
	QFileInfoList files;
	
	foreach ( const QFileInfo& file, entries )
	{
		if ( file.isFile() && ( filters.isEmpty() || QDir::match( filters, file.fileName() ) ) )
		{
			files << file;
		}
		else if ( file.isDir() )
		{
			fromDir.cd( file.filePath() );
			files << getFiles( fromDir, filters );
			fromDir.cdUp();
		}
	}
	
	return files;
}

QFileInfoList Globals::getFiles( const QString& path, const QStringList& filters )
{
	QDir dir( path );
	return getFiles( dir, filters );
}

QString Globals::keyToString( int key )
{
	if ( key == -1 )
	{
		return QObject::tr( "Undefined" );
	}
	
	return QKeySequence( key ).toString( QKeySequence::NativeText );
}

QImage Globals::toGrayscale( const QImage& srcImage, bool keepAlpha )
{
	if ( srcImage.isNull() )
	{
		return QImage();
	}

	QImage dstImage( srcImage.size(), keepAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32 );
	dstImage.fill( qRgb( 0, 0, 0 ) );

	for ( int y = 0; y < srcImage.height(); y++ )
	{
		for ( int x = 0; x < srcImage.width(); x++ )
		{
			QRgb srcPixel = srcImage.pixel( x, y );
			int grayPixel = qGray( srcPixel );
			QRgb dstPixel = qRgba( grayPixel, grayPixel, grayPixel, keepAlpha ? qAlpha( srcPixel ) : 255 );
			dstImage.setPixel( x, y, dstPixel );
		}
	}

	return dstImage;
}
