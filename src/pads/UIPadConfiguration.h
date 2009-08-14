#ifndef UIPADCONFIGURATION_H
#define UIPADCONFIGURATION_H

#include "ui_UIPadConfiguration.h"

#include <PadSettings.h>

class UIPadConfiguration : public QWidget, public Ui::UIPadConfiguration
{
	Q_OBJECT

public:
	UIPadConfiguration( QWidget* parent = 0 );
	virtual ~UIPadConfiguration();
	
	void setPads( PadSettingsList& pads, int maxPad );
	void updateGui();
	
	int currentPadIndex() const;

protected:
	PadSettingsList* mPads;
	int mMaxPad;

public slots:
	void setCurrentPadIndex( int index );

protected slots:
	void on_cbPads_currentIndexChanged( int index );
	void on_pbConfigure_clicked();
	void on_pbDefault_clicked();
	void on_pbReset_clicked();
	
	void padSettings_aboutToChangeStrokeKey( Globals::PadStroke stroke, int key, bool& accept );
	void padSettings_aboutToChangeActionKey( Globals::PadAction action, int key, bool& accept );
	void padSettings_strokeKeyChanged( Globals::PadStroke stroke, int key );
	void padSettings_actionKeyChanged( Globals::PadAction action, int key );
	void padSettings_editingFinished();
};

#endif // UIPADCONFIGURATION_H
