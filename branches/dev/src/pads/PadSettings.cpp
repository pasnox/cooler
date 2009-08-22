#include "PadSettings.h"

#include <pXmlSettings.h>

#include <QDebug>

PadSettings::PadSettings()
{
}

PadSettings::~PadSettings()
{
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
PadSettings::PadSettings( const PadSettings& other )
{
	operator=( other );
}

PadSettings& PadSettings::operator=( const PadSettings& other )
{
	if( *this != other )
	{
		mStrokeMapping = other.mStrokeMapping;
		mActionMapping = other.mActionMapping;
	}

	return *this;
}

bool PadSettings::operator==( const PadSettings& other ) const
{
	return mStrokeMapping == other.mStrokeMapping &&
		mActionMapping == other.mActionMapping;
}

bool PadSettings::operator!=( const PadSettings& other ) const
{
	return !operator==( other );
}
/*** End of dynamic memory management ***/

void PadSettings::setStrokeKeys( const PadSettings::StrokeKeys& keys )
{
	mStrokeMapping = keys;
}

void PadSettings::setActionKeys( const PadSettings::ActionKeys& keys )
{
	mActionMapping = keys;
}

void PadSettings::setStrokeKey( Globals::PadStroke stroke, int key )
{
	mStrokeMapping.remove( mStrokeMapping.key( stroke ) );
	mStrokeMapping[ key ] = stroke;
}

void PadSettings::setActionKey( Globals::PadAction action, int key )
{
	mActionMapping.remove( mActionMapping.key( action ) );
	mActionMapping[ key ] = action;
}

void PadSettings::clear()
{
	mStrokeMapping.clear();
	mActionMapping.clear();
}

bool PadSettings::isStrokeKeyUsed( int key ) const
{
	return mStrokeMapping.contains( key );
}

bool PadSettings::isActionKeyUsed( int key ) const
{
	return mActionMapping.contains( key );
}

bool PadSettings::isKeyUsed( int key ) const
{
	return mStrokeMapping.contains( key ) || mActionMapping.contains( key );
}

bool PadSettings::isValid() const
{
	return mStrokeMapping.count() == 4 && mActionMapping.count() == 4;
}

Globals::PadStroke PadSettings::keyStroke( int key ) const
{
	return mStrokeMapping.value( key, Globals::PadStrokeNo );
}

Globals::PadAction PadSettings::keyAction( int key ) const
{
	return mActionMapping.value( key, Globals::PadActionNo );
}

int PadSettings::strokeKey( Globals::PadStroke stroke ) const
{
	return mStrokeMapping.key( stroke, Globals::PadStrokeNo );
}

int PadSettings::actionKey( Globals::PadAction action ) const
{
	return mActionMapping.key( action, Globals::PadActionNo );
}

QList<int> PadSettings::keysStroke() const
{
	return mStrokeMapping.keys();
}

QList<int> PadSettings::keysAction() const
{
	return mActionMapping.keys();
}

void PadSettings::loadMapping( pXmlSettings* settings )
{
	const QStringList groups = settings->childGroups();
	
	if ( !groups.contains( "Stroke" ) || !groups.contains( "Action" ) )
	{
		return;
	}
	
	clear();
	
	mStrokeMapping[ settings->value( QString( "Stroke/%1" ).arg( Globals::PadStrokeUp ) ).toInt() ] = Globals::PadStrokeUp;
	mStrokeMapping[ settings->value( QString( "Stroke/%1" ).arg( Globals::PadStrokeDown ) ).toInt() ] = Globals::PadStrokeDown;
	mStrokeMapping[ settings->value( QString( "Stroke/%1" ).arg( Globals::PadStrokeLeft ) ).toInt() ] = Globals::PadStrokeLeft;
	mStrokeMapping[ settings->value( QString( "Stroke/%1" ).arg( Globals::PadStrokeRight ) ).toInt() ] = Globals::PadStrokeRight;
	
	mActionMapping[ settings->value( QString( "Action/%1" ).arg( Globals::PadAction1 ) ).toInt() ] = Globals::PadAction1;
	mActionMapping[ settings->value( QString( "Action/%1" ).arg( Globals::PadAction2 ) ).toInt() ] = Globals::PadAction2;
	mActionMapping[ settings->value( QString( "Action/%1" ).arg( Globals::PadAction3 ) ).toInt() ] = Globals::PadAction3;
	mActionMapping[ settings->value( QString( "Action/%1" ).arg( Globals::PadAction4 ) ).toInt() ] = Globals::PadAction4;
}

void PadSettings::saveMapping( pXmlSettings* settings ) const
{
	if ( !isValid() )
	{
		return;
	}
	
	settings->setValue( QString( "Stroke/%1" ).arg( Globals::PadStrokeUp ), QString::number( mStrokeMapping.key( Globals::PadStrokeUp ) ) );
	settings->setValue( QString( "Stroke/%1" ).arg( Globals::PadStrokeDown ), QString::number( mStrokeMapping.key( Globals::PadStrokeDown ) ) );
	settings->setValue( QString( "Stroke/%1" ).arg( Globals::PadStrokeLeft ), QString::number( mStrokeMapping.key( Globals::PadStrokeLeft ) ) );
	settings->setValue( QString( "Stroke/%1" ).arg( Globals::PadStrokeRight ), QString::number( mStrokeMapping.key( Globals::PadStrokeRight ) ) );
	
	settings->setValue( QString( "Action/%1" ).arg( Globals::PadAction1 ), QString::number( mActionMapping.key( Globals::PadAction1 ) ) );
	settings->setValue( QString( "Action/%1" ).arg( Globals::PadAction2 ), QString::number( mActionMapping.key( Globals::PadAction2 ) ) );
	settings->setValue( QString( "Action/%1" ).arg( Globals::PadAction3 ), QString::number( mActionMapping.key( Globals::PadAction3 ) ) );
	settings->setValue( QString( "Action/%1" ).arg( Globals::PadAction4 ), QString::number( mActionMapping.key( Globals::PadAction4 ) ) );
}

void PadSettings::debug() const
{
	qWarning() << mStrokeMapping << mActionMapping;
}

PadSettings PadSettings::defaultConfiguration( int index )
{
	PadSettings pad;
	PadSettings::StrokeKeys strokeKeys;
	PadSettings::ActionKeys actionKeys;
	
	switch ( index )
	{
		case 0:
			strokeKeys[ Qt::Key_Up ] = Globals::PadStrokeUp;
			strokeKeys[ Qt::Key_Down ] = Globals::PadStrokeDown;
			strokeKeys[ Qt::Key_Left ] = Globals::PadStrokeLeft;
			strokeKeys[ Qt::Key_Right ] = Globals::PadStrokeRight;
			
			actionKeys[ Qt::Key_5 ] = Globals::PadAction1;
			actionKeys[ Qt::Key_6 ] = Globals::PadAction2;
			actionKeys[ Qt::Key_8 ] = Globals::PadAction3;
			actionKeys[ Qt::Key_9 ] = Globals::PadAction4;
			break;
		case 1:
			strokeKeys[ Qt::Key_E ] = Globals::PadStrokeUp;
			strokeKeys[ Qt::Key_D ] = Globals::PadStrokeDown;
			strokeKeys[ Qt::Key_S ] = Globals::PadStrokeLeft;
			strokeKeys[ Qt::Key_F ] = Globals::PadStrokeRight;
			
			actionKeys[ Qt::Key_K ] = Globals::PadAction1;
			actionKeys[ Qt::Key_L ] = Globals::PadAction2;
			actionKeys[ Qt::Key_I ] = Globals::PadAction3;
			actionKeys[ Qt::Key_O ] = Globals::PadAction4;
			break;
	}
	
	pad.setStrokeKeys( strokeKeys );
	pad.setActionKeys( actionKeys );
	return pad;
}
