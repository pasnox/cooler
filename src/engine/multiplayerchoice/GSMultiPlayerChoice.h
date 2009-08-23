#ifndef GSMULTIPLAYERCHOICE_H
#define GSMULTIPLAYERCHOICE_H

#include <AbstractGameState.h>

#include "GSMenu.h"

class QGraphicsPixmapItem;

class GSMultiPlayerChoice : public AbstractGameState
{
public:
	static GSMultiPlayerChoice* instance();
	
	virtual void Init( const QSizeF& size );
	virtual void Cleanup();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents( GameEngine* game );
	virtual void Update( GameEngine* game );

protected:
	static GSMultiPlayerChoice* mInstance;
	TilesMap mPlayerTiles;
	
	int mBackgroundValue;
	QPixmap mBackground;
	
	GSMenu* mFacesMenu;
	
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
};

#endif // GSMULTIPLAYERCHOICE_H
