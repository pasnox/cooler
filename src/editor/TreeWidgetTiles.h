#ifndef TREEWIDGETTILES_H
#define TREEWIDGETTILES_H

#include <QTreeWidget>

class TilesManager;

class TreeWidgetTiles : public QTreeWidget
{
	Q_OBJECT
	
public:
	TreeWidgetTiles( QWidget* parent = 0 );
	virtual ~TreeWidgetTiles();
	
	TilesManager* tiles() const;
	void setTiles( TilesManager* tiles );
	
	void setCurrentLayer( int layer );

protected:
	TilesManager* mTiles;
	int mCurrentLayer;
	
	virtual void mousePressEvent( QMouseEvent* event );

protected slots:
	void datasLoaded( bool success );
};

#endif // TREEWIDGETTILES_H
