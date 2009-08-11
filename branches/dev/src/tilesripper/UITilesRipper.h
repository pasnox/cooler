#ifndef UITILESRIPPER_H
#define UITILESRIPPER_H

#include "ui_UITilesRipper.h"

class UITilesRipper : public QMainWindow, public Ui::UITilesRipper
{
	Q_OBJECT

public:
	UITilesRipper( QWidget* parent = 0 );
	virtual ~UITilesRipper();

protected slots:
	void on_pbPreview_clicked();
	void on_pbExport_clicked();
	void on_pbOpen_clicked();
};

#endif // UITILESRIPPER_H
