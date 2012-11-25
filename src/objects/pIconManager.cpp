#include "pIconManager.h"

#include <QApplication>
#include <QDir>

QIconCache pIconManager::mIconCache( 200 );

QString findFile( QDir& dir, const QString& fileName )
{
	foreach ( const QFileInfo& fi, dir.entryInfoList( QStringList( fileName ), QDir::Files | QDir::CaseSensitive ) )
	{
		if ( fi.fileName() == fileName )
		{
			return fi.canonicalFilePath();
		}
	}
	
	foreach ( const QFileInfo& fi, dir.entryInfoList( QDir::AllDirs ) )
	{
		dir.setPath( fi.canonicalFilePath() );
		QString fn = findFile( dir, fileName );
		if ( !fn.isNull() )
			return fn;
	}
	
	return QString::null;
}

QString pIconManager::filePath( const QString& fileName, const QString& prefix )
{
	QString path = prefix;
	if ( path.isEmpty() )
		path = QLatin1String( ":/" );
	QDir dir( path );
	return findFile( dir, fileName );
}

QPixmap pIconManager::pixmap( const QString& fileName, const QString& prefix )
{
	QPixmap pixmap;
	const QString fn = filePath( fileName, prefix );
	if ( !QPixmapCache::find( fn, pixmap ) )
	{
		if ( pixmap.load( fn ) )
		{
			QPixmapCache::insert( fn, pixmap );
		}
	}
	return pixmap;
}

QIcon pIconManager::icon( const QString& fileName, const QString& prefix )
{
	QIcon* icon = 0;
	const QString fn = filePath( fileName, prefix );
	if ( mIconCache.contains( fn ) )
	{
		icon = mIconCache[ fn ];
	}
	else
	{
		icon = new QIcon( pixmap( fileName, prefix ) );
		if ( icon->isNull() )
		{
			delete icon;
			icon = 0;
		}
		else
		{
			mIconCache.insert( fn, icon );
		}
	}
	return icon ? QIcon( *icon ) : QIcon();
}
