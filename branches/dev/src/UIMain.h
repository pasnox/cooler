#ifndef UIMAIN_H
#define UIMAIN_H

#include "ui_UIMain.h"

#include <PadSettings.h>

class pXmlSettings;

class UIMain : public QMainWindow, public Ui::UIMain
{
	Q_OBJECT

public:
	UIMain( QWidget* parent = 0 );
	virtual ~UIMain();
	
	void initialize();
	
	void loadSettings();
	void saveSettings();

protected:
	pXmlSettings* mSettings;
	QMap<QString, int> mPlayersNameMapping;
	PadSettingsList mPads;

protected slots:
	void on_pbWelcome_clicked();
	void on_pbMultiPlayerMode_clicked();
	void on_sbPlayerNumber_valueChanged( int value );
	void on_cbPlayers_currentIndexChanged( int index );
	void on_lwPlayers_itemSelectionChanged();
	void on_pbLetsGo_clicked();
};

#endif // UIMAIN_H
