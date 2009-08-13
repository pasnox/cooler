#ifndef PXMLSETTINGS_H
#define PXMLSETTINGS_H

#include <QSettings>
#include <QDomDocument>

class pXmlSettings : public QSettings
{
	Q_OBJECT
	
public:
	pXmlSettings( const QString& name = QString(), QObject* parent = 0 );
	virtual ~pXmlSettings();
	
	static QString fileName( const QString& name );
	static bool readXmlFile( QIODevice& device, QSettings::SettingsMap& map );
	static bool writeXmlFile( QIODevice& device, const QSettings::SettingsMap& map );

protected:
	static const QSettings::Format XmlFormat;
	
	static QVariant elementToVariant( const QDomElement& element );
	static bool variantToElement( const QVariant& variant, QDomElement& element );
	static bool recursiveNodeReader( const QString& path, const QDomElement& element, QSettings::SettingsMap& map );
};

#endif // PXMLSETTINGS_H
