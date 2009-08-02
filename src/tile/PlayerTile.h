#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include "Globals.h"

#include <QFileInfo>

struct PlayerTile
{
	PlayerTile();
	PlayerTile( const QFileInfo& fn );
	
	QPixmap tile( Globals::PlayerStroke stroke, int step );
	int steps() const;
	
	bool operator==( const PlayerTile& other ) const;
	bool operator!=( const PlayerTile& other ) const;
	
	QString FileName;
	QString Name;
	QSize Size;
	QPixmap Pixmap;
	QPixmap DeadPixmap;
};

Q_DECLARE_METATYPE( PlayerTile )

inline QDataStream& operator<<( QDataStream& stream, const PlayerTile& tile )
{
	stream << tile.FileName << tile.Name << tile.Size << tile.Pixmap << tile.DeadPixmap;
	return stream;
}

inline QDataStream& operator>>( QDataStream& stream, PlayerTile& tile )
{
	stream >> tile.FileName >> tile.Name >> tile.Size >> tile.Pixmap >> tile.DeadPixmap;
	return stream;
}

#endif // PLAYERTILE_H
