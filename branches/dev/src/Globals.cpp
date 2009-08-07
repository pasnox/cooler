#include "Globals.h"

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
