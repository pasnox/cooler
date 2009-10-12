#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMap>

class QGraphicsScene;
class TilesManager;
class MapItem;
class PlayerItem;

typedef QMap<QString, PlayerItem*> PlayersMap;

class GraphicsView : public QGraphicsView
{
	Q_OBJECT
	
public:
	GraphicsView( QWidget* parent = 0 );
	virtual ~GraphicsView();
	
	TilesManager* tiles() const;
	MapItem* map() const;
	PlayersMap players() const;
	
	void addPlayer( const QString& name, PlayerItem* player );

protected:
	QGraphicsScene* mScene;
	TilesManager* mTiles;
	MapItem* mMap;
	PlayersMap mPlayers;
	
	virtual void keyPressEvent( QKeyEvent* event );
	virtual void keyReleaseEvent( QKeyEvent* event );
	virtual void drawForeground( QPainter* painter, const QRectF& rect );
};

#endif // GRAPHICSVIEW_H
