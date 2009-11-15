#ifndef BOMBSETTINGS_H
#define BOMBSETTINGS_H

struct BombSettings
{
	BombSettings();

	BombSettings( const BombSettings& other );
	BombSettings& operator=( const BombSettings& other );
	bool operator==( const BombSettings& other ) const;
	bool operator!=( const BombSettings& other ) const;

	int TimeOut; // millisecond
	int Power; // range between 0 - 4
};

#endif // BOMBSETTINGS_H
