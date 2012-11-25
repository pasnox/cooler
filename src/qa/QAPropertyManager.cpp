#include "QAPropertyManager.h"

#include <QDebug>

namespace QA {

PropertyManager::PropertyManager( QObject* parent )
	: QtVariantPropertyManager( parent )
{
}

PropertyManager::~PropertyManager()
{
}

QtVariantProperty* PropertyManager::addProperty( int propertyType, const QString& name )
{
	return QtVariantPropertyManager::addProperty( propertyType, name );
}

int PropertyManager::attributeType( int propertyType, const QString& attribute ) const
{
	return QtVariantPropertyManager::attributeType( propertyType, attribute );
}

QVariant PropertyManager::attributeValue( const QtProperty* property, const QString& attribute ) const
{
	return QtVariantPropertyManager::attributeValue( property, attribute );
}

QStringList PropertyManager::attributes( int propertyType ) const
{
	return QtVariantPropertyManager::attributes( propertyType );
}

bool PropertyManager::isPropertyTypeSupported( int propertyType ) const
{
	switch ( propertyType )
	{
		case QVariant::Pixmap:
			return true;
		default:
			return QtVariantPropertyManager::isPropertyTypeSupported( propertyType );
	}
}

QVariant PropertyManager::value( const QtProperty* property ) const
{
	if ( mPixmapValues.contains( property ) )
	{
		return mPixmapValues[ property ];
	}
	return QtVariantPropertyManager::value(property);
}

int PropertyManager::valueType( int propertyType ) const
{
	switch ( propertyType )
	{
		case QVariant::Pixmap:
			return QVariant::Pixmap;
		default:
			return QtVariantPropertyManager::valueType( propertyType );
	}
}

void PropertyManager::initializeProperty( QtProperty* property )
{
	switch ( propertyType( property ) )
	{
		case QVariant::Pixmap:
		{
			mPixmapValues[ property ] = QPixmap();
			break;
		}
		default:
			QtVariantPropertyManager::initializeProperty( property );
	}
}

void PropertyManager::uninitializeProperty( QtProperty* property )
{
	switch ( propertyType( property ) )
	{
		case QVariant::Pixmap:
		{
			mPixmapValues.remove( property );
			break;
		}
		default:
			QtVariantPropertyManager::uninitializeProperty( property );
	}
}

QIcon PropertyManager::valueIcon( const QtProperty* property ) const
{
	switch ( propertyType( property ) )
	{
		case QVariant::Pixmap:
		{
			return QIcon( value( property ).value<QPixmap>() );
		}
		default:
			return QtVariantPropertyManager::valueIcon( property );
	}
}

QString PropertyManager::valueText( const QtProperty* property ) const
{
	switch ( propertyType( property ) )
	{
		case QVariant::Pixmap:
		{
			QPixmap pixmap = value( property ).value<QPixmap>();
			return pixmap.isNull() ? tr( "<undefined>" ) : tr( "Defined" );
		}
		default:
			return QtVariantPropertyManager::valueText( property );
	}
}

void PropertyManager::setAttribute( QtProperty* property, const QString& attribute, const QVariant& value )
{
	QtVariantPropertyManager::setAttribute( property, attribute, value );
}

void PropertyManager::setValue( QtProperty* property, const QVariant& value )
{
	if ( mPixmapValues.contains( property ) )
	{
		if ( value.type() == QVariant::Pixmap )
		{
			mPixmapValues[ property ] = value.value<QPixmap>();
		}
		else
		{
			QPixmap pixmap;
			
			if ( pixmap.loadFromData( value.toByteArray() ) )
			{
				mPixmapValues[ property ] = pixmap;
			}
		}
		
		emit propertyChanged( property );
		emit valueChanged( property, value );
	}
	else
	{
		QtVariantPropertyManager::setValue( property, value );
	}
}

}
