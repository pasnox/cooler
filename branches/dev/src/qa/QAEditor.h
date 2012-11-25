#ifndef QAEDITOR_H
#define QAEDITOR_H

#include "ui_QAEditor.h"
#include "QAEditable.h"

namespace QA {
	class HeaderProperties;
	class FrameProperties;
	class FrameModel;
};

class QAEditor : public QMainWindow, public Ui::QAEditor
{
	Q_OBJECT

public:
	QAEditor( QWidget* parent = 0 );
	virtual ~QAEditor();

protected:
	QMenuBar* mbFrame;
	QString mFilePath;
	QA::Container mHandle;
	QA::HeaderProperties* mHeaderProperties;
	QA::FrameProperties* mFrameProperties;
	QA::FrameModel* mFrameModel;
	
	bool importFrame( int index, const QImage& image, const QString& name = QString::null, const QString& source = QString::null );
	bool importFrames( int index, const QImage& image, const QString& source = QString::null );

public slots:
	void on_aNew_triggered();
	bool on_aOpen_triggered();
	bool on_aSave_triggered();
	
	void addFrame();
	void removeFrame();
	void setTransparentPixel();

protected slots:
	void lvFrames_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
};

#endif // QAEDITOR_H
