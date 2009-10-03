#ifndef GSGENERICSTATEITEM_H
#define GSGENERICSTATEITEM_H

#include "GSMenuItem.h"

class GSGenericStateItem : public GSMenuItem
{
public:
	GSGenericStateItem( int min, int max, int state, int pixelSize = 20 );
	GSGenericStateItem( int min, int max );
	
	void setWrapEnabled( bool enabled );
	const bool& wrapEnabled() const;
	
	virtual void setState( int state );
	const int& state() const;
	
	void previousState();
	void nextState();

protected:
	int mMin;
	int mMax;
	int mState;
	bool mWrapEnabled;
	
	void init();
};

#endif // GSGENERICSTATEITEM_H
