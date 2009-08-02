#ifndef BOMBTILE_H
#define BOMBTILE_H

#include "Globals.h"

#include <QFileInfo>

struct BombTile
{
	BombTile();
	BombTile( const QFileInfo& fn );
	
	QPixmap tile( int step );
	int steps() const;
	
	bool operator==( const BombTile& other ) const;
	bool operator!=( const BombTile& other ) const;
	
	QString FileName;
	QString Name;
	QSize Size;
	QPixmap Pixmap;
};

Q_DECLARE_METATYPE( BombTile )

inline QDataStream& operator<<( QDataStream& stream, const BombTile& tile )
{
	stream << tile.FileName << tile.Name << tile.Size << tile.Pixmap;
	return stream;
}

inline QDataStream& operator>>( QDataStream& stream, BombTile& tile )
{
	stream >> tile.FileName >> tile.Name >> tile.Size >> tile.Pixmap;
	return stream;
}

#endif // BOMBTILE_H
