#ifndef PICONMANAGER_H
#define PICONMANAGER_H

#include <QPixmapCache>
#include <QCache>
#include <QIcon>

typedef QCache<QString, QIcon> QIconCache;

class pIconManager
{
public:
	// return the filepath of the icon named fileName in prefix folder ( check is done recursively )
	static QString filePath( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	// return the QPixmap of the pixmap named fileName in prefix folder ( check is done recursively )
	static QPixmap pixmap( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	// return the QIcon of the icon named fileName in prefix folder ( check is done recursively )
	static QIcon icon( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );

protected:
	static QIconCache mIconCache;
};

#endif // PICONMANAGER_H
