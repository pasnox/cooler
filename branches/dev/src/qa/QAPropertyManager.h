#ifndef QAPROPERTYMANAGER_H
#define QAPROPERTYMANAGER_H

#include <QtVariantPropertyManager>

namespace QA {

class PropertyManager : public QtVariantPropertyManager
{
	Q_OBJECT
	
public:
	PropertyManager( QObject* parent = 0 );
	virtual ~PropertyManager();
	
	virtual QtVariantProperty* addProperty( int propertyType, const QString& name = QString() );
	virtual int attributeType( int propertyType, const QString& attribute ) const;
	virtual QVariant attributeValue( const QtProperty* property, const QString& attribute ) const;
	virtual QStringList attributes( int propertyType ) const;
	virtual bool isPropertyTypeSupported( int propertyType ) const;
	virtual QVariant value( const QtProperty* property ) const;
	virtual int valueType( int propertyType ) const;

protected:
	QMap<const QtProperty*, QPixmap> mPixmapValues;
	
	virtual void initializeProperty( QtProperty* property );
	virtual void uninitializeProperty( QtProperty* property );
	virtual QIcon valueIcon( const QtProperty* property ) const;
	virtual QString valueText( const QtProperty* property ) const;

public slots:
	virtual void setAttribute( QtProperty* property, const QString& attribute, const QVariant& value );
	virtual void setValue( QtProperty* property, const QVariant& value );
};

};

#endif // QAPROPERTYMANAGER_H
