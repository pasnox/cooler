#ifndef UIMAIN_H
#define UIMAIN_H

#include "ui_UIMain.h"

#include <PadSettings.h>

class UIMain : public QMainWindow, public Ui::UIMain
{
	Q_OBJECT

public:
	UIMain( QWidget* parent = 0 );
	virtual ~UIMain();
	
	void initialize();

protected:
	PadSettingsList mPads;

protected slots:
	void on_pbWelcome_clicked();
	void on_pbMultiPlayerMode_clicked();
	void on_pbSettingsMode_clicked();
};

#endif // UIMAIN_H
