#ifndef UIMAIN_H
#define UIMAIN_H

#include "ui_UIMain.h"

class UIMain : public QMainWindow, public Ui::UIMain
{
	Q_OBJECT

public:
	UIMain( QWidget* parent = 0 );
	virtual ~UIMain();
	
	void initialize();
};

#endif // UIMAIN_H
