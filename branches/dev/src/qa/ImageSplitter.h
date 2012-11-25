#ifndef IMAGESPLITTER_H
#define IMAGESPLITTER_H

#include "ui_ImageSplitter.h"

#include <QAbstractItemModel>

class QtVariantProperty;

namespace QA {

class PropertyManager;
class EditorFactory;

class ImageSplitterModel : public QAbstractItemModel
{
	Q_OBJECT
	
	friend class ImageSplitter;
	
public:
	ImageSplitterModel( const QImage& image, const QMap<QString, QtVariantProperty*>& properties, QObject* parent = 0 );
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

protected:
	int mRowCount;
	QMap<QPersistentModelIndex, QMap<int, QVariant> > mData;
	const QImage* mImage;
	const QMap<QString, QtVariantProperty*>* mProperties;

protected slots:
	void propertiesChanged();
};

class ImageSplitter : public QWidget, public Ui::ImageSplitter
{
	Q_OBJECT

public:
	ImageSplitter( const QImage& image, QWidget* parent = 0 );
	virtual ~ImageSplitter();
	
	QList<QImage> checkedFrames() const;
	
	static QList<QImage> getCheckedFrames( const QImage& image, QWidget* parent = 0 );

protected:
	QImage mImage;
	QA::PropertyManager* mPropertyManager;
	QA::EditorFactory* mEditorFactory;
	QMap<QString, QtVariantProperty*> mProperties;
	ImageSplitterModel* mModel;
	
	void addProperty( const QString& name, QtVariantProperty* property );
};

};

#endif // IMAGESPLITTER_H
