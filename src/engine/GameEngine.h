#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QGraphicsView>

#include <QString>
#include <QSize>
#include <QVector>

#include <PadSettings.h>
#include <Player.h>

class pXmlSettings;

class AbstractGameState;
class QGraphicsScene;

class GameEngine : public QGraphicsView
{
	Q_OBJECT
	
public:
	virtual void Init( const QString& title, const QSize& size = QSize( 1024, 768 ), int bpp = 0, bool fullscreen = false );
	virtual void Cleanup();

	virtual void ChangeState( AbstractGameState* state );
	virtual void PushState( AbstractGameState* state );
	virtual void PopState();

	virtual void HandleEvents();
	virtual void Update();
	virtual void Draw();

	virtual bool Running() { return mRunning; }
	virtual void Quit() { mRunning = false; }
	
	void setPadSettings( const PadSettingsList& pads );
	const PadSettingsList& padsSettings() const;
	
	void setPlayers( const PlayerList& players );
	const PlayerList& players() const;

protected:
	pXmlSettings* mSettings;
	PadSettingsList mPads;
	PlayerList mPlayers;
	
	QGraphicsScene* mScene;
	QVector<AbstractGameState*> mStates;
	int mIDLETimer;
	bool mRunning;
	bool mFullScreen;
	QSize mSize;
	
	virtual void closeEvent( QCloseEvent* event );
	virtual void timerEvent( QTimerEvent* event );

public slots:
	void start();
	void stop();
};

#endif // GAMEENGINE_H
