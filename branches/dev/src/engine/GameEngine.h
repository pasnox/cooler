#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QGraphicsView>

#include <QString>
#include <QSize>
#include <QVector>

#include "PadSettings.h"
#include "Player.h"

class pXmlSettings;
class AbstractGameState;
class Map;
class QGraphicsScene;

class GameEngine : public QGraphicsView
{
	Q_OBJECT
	
public:
	virtual void init( const QString& title, const QSize& size = QSize( 1024, 768 ), int bpp = 0, bool fullscreen = false );
	virtual void cleanup();

	virtual void changeState( AbstractGameState* state );
	virtual void pushState( AbstractGameState* state );
	virtual void popState();

	virtual void handleEvents();
	virtual void update();
	virtual void draw();

	virtual bool running() { return mRunning; }
	virtual void quit() { mRunning = false; }
	
	void setPadSettings( const PadSettingsList& pads );
	const PadSettingsList& padsSettings() const;
	
	void setPlayers( const PlayerList& players );
	const PlayerList& players() const;
	
	const QFileInfoList& maps() const;
	
	void setMap( Map* map );
	Map* map() const;

protected:
	pXmlSettings* mSettings;
	PadSettingsList mPads;
	PlayerList mPlayers;
	QFileInfoList mMaps;
	
	QGraphicsScene* mScene;
	QVector<AbstractGameState*> mStates;
	int mIDLETimer;
	bool mRunning;
	bool mFullScreen;
	QSize mSize;
	
	Map* mMap;
	
	virtual void closeEvent( QCloseEvent* event );
	virtual void timerEvent( QTimerEvent* event );

public slots:
	void start();
	void stop();
};

#endif // GAMEENGINE_H
