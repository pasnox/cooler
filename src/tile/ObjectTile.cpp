#include "ObjectTile.h"

ObjectTile::ObjectTile()
{
	Type = Globals::InvalidObject;
}

ObjectTile::ObjectTile( Globals::TypeObject type, const QFileInfo& fn )
{
	Type = type;
	FileName = fn.absoluteFilePath();
	Pixmap = QPixmap( FileName );
}

bool ObjectTile::operator==( const ObjectTile& other ) const
{
	return Type == other.Type && FileName == other.FileName;
}

bool ObjectTile::operator!=( const ObjectTile& other ) const
{
	return !operator==( other );
}
