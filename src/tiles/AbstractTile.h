#ifndef ABSTRACTTILE_H
#define ABSTRACTTILE_H

#include "Globals.h"

#include <QFileInfo>
#include <QPixmap>
#include <QMap>

#include <QDebug>

struct AbstractTile
{
	AbstractTile( const QFileInfo& fn = QFileInfo(), Globals::TypeTile type = Globals::InvalidTile );
	virtual ~AbstractTile();
	
	virtual QString absoluteFilePath() const;
	virtual QString name() const;
	virtual int steps() const = 0;
	virtual QPixmap tile( int step ) const = 0;
	
	static QByteArray toByteArray( const AbstractTile* tile );
	static AbstractTile* fromByteArray( const QByteArray& datas );
	
	Globals::TypeTile Type;
	QFileInfo FileName;
	QPixmap Pixmap;
};

typedef QMap<QString, AbstractTile*> TilesMap;
typedef QMap<Globals::TypeTile, TilesMap> TypesTilesMap;

Q_DECLARE_METATYPE( AbstractTile* )

#endif // ABSTRACTTILE_H
