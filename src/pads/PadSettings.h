#ifndef PADSETTINGS_H
#define PADSETTINGS_H

#include <QMap>

#include <Globals.h>

class QSettings;

class PadSettings
{
public:

	typedef QMap<int, Globals::PadStroke> StrokeKeys;
	typedef QMap<int, Globals::PadAction> ActionKeys;
	
	PadSettings();
	virtual ~PadSettings();

	/*** For dynamic memory management (can be removed if it isn't necessary) ***/
	PadSettings( const PadSettings& other );
	PadSettings& operator=( const PadSettings& other );
	bool operator==( const PadSettings& other ) const;
	bool operator!=( const PadSettings& other ) const;
	/*** End of dynamic memory management ***/
	
	void setStrokeKeys( const PadSettings::StrokeKeys& keys );
	void setActionKeys( const PadSettings::ActionKeys& keys );
	
	void setStrokeKey( Globals::PadStroke stroke, int key );
	void setActionKey( Globals::PadAction action, int key );
	
	void clear();
	
	bool isStrokeKeyUsed( int key ) const;
	bool isActionKeyUsed( int key ) const;
	bool isKeyUsed( int key ) const;
	bool isValid() const;
	
	Globals::PadStroke keyStroke( int key ) const;
	Globals::PadAction keyAction( int key ) const;
	
	QList<int> keysStroke() const;
	QList<int> keysAction() const;
	
	void loadMapping( QSettings* settings );
	void saveMapping( QSettings* settings ) const;
	
	void debug() const;
	
	static PadSettings defaultConfiguration( int index );

protected:
	StrokeKeys mStrokeMapping;
	ActionKeys mActionMapping;
};

typedef QList<PadSettings*> PadSettingsList;

Q_DECLARE_METATYPE( PadSettings* );

#endif // PADSETTINGS_H
