#ifndef GSMULTIPLAYGROUND_H
#define GSMULTIPLAYGROUND_H

#include "AbstractGameState.h"
#include "GSMenuItem.h"

class QGraphicsLinearLayout;
class MapItem;
class PlayerItem;

class GSMultiPlayGround : public AbstractGameState
{
public:
	static GSMultiPlayGround* instance();
	
	virtual void init( GameEngine* engine, const QSizeF& size );
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void handleEvents( GameEngine* engine );
	virtual void update( GameEngine* engine );
	virtual bool validateState( GameEngine* engine ) const;

protected:
	static GSMultiPlayGround* mInstance;
	
	QGraphicsLinearLayout* mMainLayout;
	GSMenuItem* mTitle;
	MapItem* mMap;
	QMap<uint, PlayerItem*> mPlayers;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	
	bool handlePlayersEvent( KeyEvent* event, const PadSettingsList& pads );
};

#endif // GSMULTIPLAYGROUND_H
