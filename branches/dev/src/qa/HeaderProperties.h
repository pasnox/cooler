#ifndef HEADERPROPERTIES_H
#define HEADERPROPERTIES_H

#include <QObject>
#include <QMap>

class QtAbstractPropertyBrowser;
class QtProperty;
class QtVariantProperty;

namespace QA {

class PropertyManager;
class EditorFactory;
class Container;

class HeaderProperties : public QObject
{
	Q_OBJECT
	
public:
	HeaderProperties( QA::Container* handle, QtAbstractPropertyBrowser* editor, QObject* parent = 0 );
	virtual ~HeaderProperties();
	
	void addProperty( QtProperty* property, const QString& id );

protected:
	QA::PropertyManager* mPropertyManager;
	QA::EditorFactory* mEditorFactory;
	bool mReading;
	QtAbstractPropertyBrowser* mPropertyEditor;
	QMap<QtProperty*, QString> propertyToId;
	QMap<QString, QtProperty*> idToProperty;
	QMap<QString, bool> idToExpanded;
	QA::Container* mHandle;
	
	// header
	QtVariantProperty* mVersion;
	QtVariantProperty* mFormat;
	// ex header
	QtVariantProperty* mName;
	QtVariantProperty* mFrameSize;
	QtVariantProperty* mFrameCount;
	
	void beginUpdate();
	void endUpdate();

public slots:
	void readValues();
	void writeValues( QtProperty* property = 0 );

protected slots:
	void _q_propertyChanged( QtProperty* property );
};

};

#endif // HEADERPROPERTIES_H
