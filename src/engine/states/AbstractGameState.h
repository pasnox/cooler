#ifndef ABSTRACTGAMESTATE_H
#define ABSTRACTGAMESTATE_H

#include <QGraphicsWidget>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "GameEngine.h"
#include "TilesManager.h"

struct Event
{
	enum Type
	{
		Invalid = -1,
		KeyPress,
		KeyRelease,
		MouseDoubleClick,
		MousePress,
		MouseRelease,
		MouseMove
	};
	
	Event( Event::Type _type = Event::Invalid )
	{ type = _type; }
	
	Event::Type type;
};

struct KeyEvent : Event
{
	KeyEvent( QKeyEvent* event )
		: Event()
	{
		switch ( event->type() )
		{
			case QEvent::KeyPress:
				type = Event::KeyPress;
				break;
			case QEvent::KeyRelease:
				type = Event::KeyRelease;
				break;
			default:
				Q_ASSERT( 0 );
				break;
		}
		
		key = event->key();
		modifiers = event->modifiers();
		text = event->text();
		autoRepeat = event->isAutoRepeat();
		count = event->count();
	}

	int key;
	Qt::KeyboardModifiers modifiers;
	QString text;
	bool autoRepeat;
	ushort count;
};

struct MouseEvent : Event
{
	MouseEvent( QGraphicsSceneMouseEvent* event )
		: Event()
	{
		switch ( event->type() )
		{
			case QEvent::GraphicsSceneMouseDoubleClick:
				type = Event::MouseDoubleClick;
				break;
			case QEvent::GraphicsSceneMousePress:
				type = Event::MousePress;
				break;
			case QEvent::GraphicsSceneMouseRelease:
				type = Event::MouseRelease;
				break;
			case QEvent::GraphicsSceneMouseMove:
				type = Event::MouseMove;
				break;
			default:
				Q_ASSERT( 0 );
				break;
		}
		
		button = event->button();
		buttons = event->buttons();
		lastPos = event->lastPos();
		modifiers = event->modifiers();
		pos = event->pos();
		
		if ( buttons != Qt::NoButton )
		{
			if ( buttons & Qt::LeftButton )
			{
				buttonDownPos[ Qt::LeftButton ] = event->buttonDownPos( Qt::LeftButton );
			}
			
			if ( buttons & Qt::RightButton )
			{
				buttonDownPos[ Qt::RightButton ] = event->buttonDownPos( Qt::RightButton );
			}
			
			if ( buttons & Qt::MidButton )
			{
				buttonDownPos[ Qt::MidButton ] = event->buttonDownPos( Qt::MidButton );
			}
			
			if ( buttons & Qt::XButton1 )
			{
				buttonDownPos[ Qt::XButton1 ] = event->buttonDownPos( Qt::XButton1 );
			}
			
			if ( buttons & Qt::XButton2 )
			{
				buttonDownPos[ Qt::XButton2 ] = event->buttonDownPos( Qt::XButton2 );
			}
		}
	}
	
	Qt::MouseButton button;
	Qt::MouseButtons buttons;
	QPointF lastPos;
	Qt::KeyboardModifiers modifiers;
	QPointF pos;
	QMap<Qt::MouseButton, QPointF> buttonDownPos;
};

class AbstractGameState : public QGraphicsWidget
{
	Q_OBJECT
	
public:
	virtual void Init( GameEngine* engine, const QSizeF& size )
	{
		Q_UNUSED( engine );
		mSize = size;
		setGeometry( QRectF( QPointF( 0, 0 ), size ) );
	}
	
	virtual void Cleanup()
	{
		FlushEvents();
		setGeometry( QRectF() );
		mTiles.clear();
	}
	
	virtual void FlushEvents()
	{
		qDeleteAll( mEvents );
		mEvents.clear();
	}

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents( GameEngine* game ) = 0;
	virtual void Update( GameEngine* game ) = 0;
	virtual bool validateState( GameEngine* game ) const = 0;

	virtual void ChangeState( GameEngine* game, AbstractGameState* state )
	{
		game->ChangeState( state );
	}

protected:
	QSizeF mSize;
	QList<Event*> mEvents;
	TilesMap mTiles;
	
	virtual void keyPressEvent( QKeyEvent* event )
	{ mEvents << new KeyEvent( event ); }
	
	virtual void keyReleaseEvent( QKeyEvent* event )
	{ mEvents << new KeyEvent( event ); }
	
	virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event )
	{ mEvents << new MouseEvent( event ); }
	
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* event )
	{ mEvents << new MouseEvent( event ); }
	
	virtual void mousePressEvent( QGraphicsSceneMouseEvent* event )
	{ mEvents << new MouseEvent( event ); }
	
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
	{ mEvents << new MouseEvent( event ); }
	
	template <class T>
	static T* qCleanup( T* pointer )
	{
		delete pointer;
		return 0;
	}
};

#define Q_CLEANUP( pointer ) pointer = qCleanup( pointer )

#endif // ABSTRACTGAMESTATE_H