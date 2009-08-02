#ifndef OBJECTTILE_H
#define OBJECTTILE_H

#include "Globals.h"

#include <QFileInfo>

struct ObjectTile
{
	ObjectTile();
	ObjectTile( Globals::TypeObject type, const QFileInfo& fn );
	bool operator==( const ObjectTile& other ) const;
	bool operator!=( const ObjectTile& other ) const;
	
	Globals::TypeObject Type;
	QString FileName;
	QPixmap Pixmap;
};

Q_DECLARE_METATYPE( ObjectTile )

inline QDataStream& operator<<( QDataStream& stream, const ObjectTile& tile )
{
	stream << (qint8)tile.Type << tile.FileName << tile.Pixmap;
	return stream;
}

inline QDataStream& operator>>( QDataStream& stream, ObjectTile& tile )
{
	qint8 type;
	stream >> type >> tile.FileName >> tile.Pixmap;
	tile.Type = (Globals::TypeObject)type;
	return stream;
}

#endif // OBJECTTILE_H
