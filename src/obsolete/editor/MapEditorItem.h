#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "MapItem.h"

class MapEditorItem : public QObject, public MapItem
{
	Q_OBJECT
	
public:
	MapEditorItem( QObject* parentObject = 0, QGraphicsItem* parent = 0 );
	virtual ~MapEditorItem();
	
	void setName( const QString& name );
	void setSize( const QSize& size );
	void setLayers( const LayersMap& layers );
	void setLayer( uint id, const LayerMap& layer );
	void setLayerTile( uint id, const QPoint& pos, AbstractTile* tile );
	void setCurrentLayer( int layer, bool exclusive );
	void removeLayer( int layer );
	void clearLayers();
	
	virtual bool load( const QString& fileName );
	bool save( const QString& fileName );

protected:
	ObjectTile* objectTile( const QString& name ) const;
	uint createMapping( const QString& name );
};

#endif // MAPEDITOR_H
