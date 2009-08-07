#ifndef UIPADSETTINGS_H
#define UIPADSETTINGS_H

#include "ui_UIPadSettings.h"

#include <PadSettings.h>

class QTimer;

class UIPadSettings : public QWidget, public Ui::UIPadSettings
{
	Q_OBJECT

public:
	UIPadSettings( QWidget* parent = 0 );
	virtual ~UIPadSettings();

protected:
	QTimer* mSetupTimer;
	int mKeyConfigurationStep;
	QPalette mPalette;
	QPalette mPaletteEditing;
	QColor mGoodColor;
	QColor mBadColor;
	
	void keyPressEvent( QKeyEvent* event );
	
	QLineEdit* lineEditForStep( int step ) const;
	QString keyToString( int key ) const;

public slots:
	void setup();
	void updateGui( PadSettings* pad );

protected slots:
	void setupTimer_timeout();

signals:
	void aboutToChangeStrokeKey( Globals::PadStroke stroke, int key, bool& accept );
	void aboutToChangeActionKey( Globals::PadAction action, int key, bool& accept );
	void strokeKeyChanged( Globals::PadStroke stroke, int key );
	void actionKeyChanged( Globals::PadAction action, int key );
	void editingFinished();
};

#endif // UIPADSETTINGS_H
