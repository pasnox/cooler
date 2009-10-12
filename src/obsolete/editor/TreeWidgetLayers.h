#ifndef TREEWIDGETLAYERS_H
#define TREEWIDGETLAYERS_H

#include <QTreeWidget>

class TreeWidgetLayersItem : public QTreeWidgetItem
{
public:
	TreeWidgetLayersItem( QTreeWidget* parent, int type = Type )
		: QTreeWidgetItem( parent, type )
	{
	}
	
	virtual bool operator<( const QTreeWidgetItem& other ) const
	{
		return data( 0, Qt::UserRole ).toInt() < other.data( 0, Qt::UserRole ).toInt();
	}
};

class TreeWidgetLayers : public QTreeWidget
{
	Q_OBJECT
	
public:
	TreeWidgetLayers( QWidget* parent = 0 );
	virtual ~TreeWidgetLayers();
	
	QList<uint> layers() const;
	void setLayers( const QList<uint> layers );
	
	void addLayer();
	void addSkyLayer();
	void removeLayer();
	void clearLayers();
	
	int currentLayer() const;
	void setCurrentLayer( int layer );

protected:
	void updateItems();

protected slots:
	void _q_itemSelectionChanged();

signals:
	void currentLayerChanged( int layer );
	void layerRemoved( int layer );
	void layersCleared();
};

#endif // TREEWIDGETLAYERS_H
