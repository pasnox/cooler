#include "PadSettings.h"

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

Globals::PadStroke PadSettings::keyStroke( int key ) const
{
	return mStrokeMapping.value( key, Globals::PadStrokeNo );
}

Globals::PadAction PadSettings::keyAction( int key ) const
{
	return mActionMapping.value( key, Globals::PadActionNo );
}

QList<int> PadSettings::keysStroke() const
{
	return mStrokeMapping.keys();
}

QList<int> PadSettings::keysAction() const
{
	return mActionMapping.keys();
}

void PadSettings::debug() const
{
	qWarning() << mStrokeMapping << mActionMapping;
}
