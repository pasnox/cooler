#include "UIPadSettings.h"

#include <QTimer>
#include <QKeyEvent>
#include <QDebug>

UIPadSettings::UIPadSettings( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	mSetupTimer = new QTimer( this );
	mKeyConfigurationStep = -1;
	mPalette = leUp->palette();
	mPaletteEditing = mPalette;
	mGoodColor = QColor( 117, 247, 88 );
	mBadColor = QColor( 250, 115, 118 );
	
	connect( mSetupTimer, SIGNAL( timeout() ), this, SLOT( setupTimer_timeout() ) );
}

UIPadSettings::~UIPadSettings()
{
}

PadSettings UIPadSettings::pad() const
{
	return mPad;
}

void UIPadSettings::setPad( const PadSettings& pad )
{
	mPad = pad;
	updateGui();
}

void UIPadSettings::keyPressEvent( QKeyEvent* event )
{
	QWidget::keyPressEvent( event );
	
	if ( !mSetupTimer->isActive() )
	{
		return;
	}
	
	if ( event->isAutoRepeat() )
	{
		return;
	}
	
	bool accept = false;
	
	if ( event->key() != Qt::Key_Escape )
	{
		accept = true;
		
		switch ( mKeyConfigurationStep )
		{
			case 0: // Up
				emit aboutToChangeStrokeKey( Globals::PadStrokeUp, event->key(), accept );
				if ( accept )
					emit strokeKeyChanged( Globals::PadStrokeUp, event->key() );
				break;
			case 1: // Down
				emit aboutToChangeStrokeKey( Globals::PadStrokeDown, event->key(), accept );
				if ( accept )
					emit strokeKeyChanged( Globals::PadStrokeDown, event->key() );
				break;
			case 2: // Left
				emit aboutToChangeStrokeKey( Globals::PadStrokeLeft, event->key(), accept );
				if ( accept )
					emit strokeKeyChanged( Globals::PadStrokeLeft, event->key() );
				break;
			case 3: // Right
				emit aboutToChangeStrokeKey( Globals::PadStrokeRight, event->key(), accept );
				if ( accept )
					emit strokeKeyChanged( Globals::PadStrokeRight, event->key() );
				break;
			case 4: // Action1
				emit aboutToChangeActionKey( Globals::PadAction1, event->key(), accept );
				if ( accept )
					emit actionKeyChanged( Globals::PadAction1, event->key() );
				break;
			case 5: // Action2
				emit aboutToChangeActionKey( Globals::PadAction2, event->key(), accept );
				if ( accept )
					emit actionKeyChanged( Globals::PadAction2, event->key() );
				break;
			case 6: // Action3
				emit aboutToChangeActionKey( Globals::PadAction3, event->key(), accept );
				if ( accept )
					emit actionKeyChanged( Globals::PadAction3, event->key() );
				break;
			case 7:
				emit aboutToChangeActionKey( Globals::PadAction4, event->key(), accept );
				if ( accept )
					emit actionKeyChanged( Globals::PadAction4, event->key() );
				break;
		}
	}
	
	if ( accept )
	{
		lineEditForStep( mKeyConfigurationStep )->setText( keyToString( event->key() ) );
		
		if ( mKeyConfigurationStep == 7 )
		{
			mKeyConfigurationStep = -1;
		}
		else
		{
			mKeyConfigurationStep++;
			mPaletteEditing.setColor( leUp->backgroundRole(), mGoodColor );
		}
	}
	else
	{
		mPaletteEditing.setColor( leUp->backgroundRole(), mBadColor );
		
		if ( event->key() == Qt::Key_Escape )
		{
			mKeyConfigurationStep = -1;
		}
	}
}

QLineEdit* UIPadSettings::lineEditForStep( int step ) const
{
	switch ( step )
	{
		case 0: // Up
			return leUp;
			break;
		case 1: // Down
			return leDown;
			break;
		case 2: // Left
			return leLeft;
			break;
		case 3: // Right
			return leRight;
			break;
		case 4: // Action1
			return leAction1;
			break;
		case 5: // Action2
			return leAction2;
			break;
		case 6: // Action3
			return leAction3;
			break;
		case 7: // Action4
			return leAction4;
			break;
	}
	
	return 0;
}

QString UIPadSettings::keyToString( int key ) const
{
	return QKeySequence( key ).toString( QKeySequence::NativeText );
}

void UIPadSettings::updateGui()
{
	foreach ( QLineEdit* le, findChildren<QLineEdit*>() )
	{
		le->clear();
	}
	
	foreach ( const int& key, mPad.keysStroke() )
	{
		const Globals::PadStroke stroke = mPad.keyStroke( key );
		
		switch ( stroke )
		{
			case Globals::PadStrokeUp:
				leUp->setText( keyToString( key ) );
				break;
			case Globals::PadStrokeDown:
				leDown->setText( keyToString( key ) );
				break;
			case Globals::PadStrokeLeft:
				leLeft->setText( keyToString( key ) );
				break;
			case Globals::PadStrokeRight:
				leRight->setText( keyToString( key ) );
				break;
			default:
				Q_ASSERT( 0 );
				break;
		}
	}
	
	foreach ( const int& key, mPad.keysAction() )
	{
		const Globals::PadAction action = mPad.keyAction( key );
		
		switch ( action )
		{
			case Globals::PadAction1:
				leAction1->setText( keyToString( key ) );
				break;
			case Globals::PadAction2:
				leAction2->setText( keyToString( key ) );
				break;
			case Globals::PadAction3:
				leAction3->setText( keyToString( key ) );
				break;
			case Globals::PadAction4:
				leAction4->setText( keyToString( key ) );
				break;
			default:
				Q_ASSERT( 0 );
				break;
		}
	}
}

void UIPadSettings::setup()
{
	setFocus();
	mKeyConfigurationStep = 0;
	mPaletteEditing.setColor( leUp->backgroundRole(), mGoodColor );
	mSetupTimer->start( 125 );
}

void UIPadSettings::setupTimer_timeout()
{
	foreach ( QLineEdit* le, findChildren<QLineEdit*>() )
	{
		le->setPalette( mPalette );
	}
	
	QLineEdit* le = lineEditForStep( mKeyConfigurationStep );
	
	if ( le )
	{
		le->setPalette( mPaletteEditing );
	}
	else
	{
		mSetupTimer->stop();
		emit editingFinished();
	}
}
