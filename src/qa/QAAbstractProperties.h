#ifndef QAABSTRACTPROPERTIES_H
#define QAABSTRACTPROPERTIES_H

#include <QtAbstractPropertyBrowser>

#include <QPersistentModelIndex>

namespace QA {

class PropertyManager;
class EditorFactory;
class Container;

class AbstractProperties : public QObject
{
	Q_OBJECT

public:
	AbstractProperties( QA::Container* handle, QtAbstractPropertyBrowser* editor, QObject* parent );
	virtual ~AbstractProperties();
	
	void addProperty( QtProperty* property, const QString& id );
	virtual void writeValues( const QModelIndex& index, QtProperty* property = 0 ) = 0;
	
	static QA::PropertyManager* propertyManager();
	static QA::EditorFactory* editorFactory();

protected:
	static QA::PropertyManager* mPropertyManager;
	static QA::EditorFactory* mEditorFactory;
	QA::Container* mHandle;
	QPersistentModelIndex mIndex;
	bool mReading;
	QtAbstractPropertyBrowser* mPropertyEditor;
	QMap<QtProperty*, QString> propertyToId;
	QMap<QString, QtProperty*> idToProperty;
	QMap<QString, bool> idToExpanded;
	
	void beginUpdate();
	void endUpdate();

public slots:
	virtual void readValues( const QModelIndex& index = QModelIndex() );

protected slots:
	void _q_propertyChanged( QtProperty* property );
};

};

#endif // QAABSTRACTPROPERTIES_H
