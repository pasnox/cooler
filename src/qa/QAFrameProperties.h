#ifndef QAFRAMEPROPERTIES_H
#define QAFRAMEPROPERTIES_H

#include "QAAbstractProperties.h"

class QtVariantProperty;

namespace QA {

class Container;

class FrameProperties : public QA::AbstractProperties
{
	Q_OBJECT

public:
	FrameProperties( QA::Container* handle, QtAbstractPropertyBrowser* editor, QObject* parent );
	virtual ~FrameProperties();
	
	virtual void writeValues( const QModelIndex& index, QtProperty* property );

protected:
	QtVariantProperty* mName;
	QtVariantProperty* mFrameId;
	QtVariantProperty* mData;
	QtVariantProperty* mOrigin;
	QtVariantProperty* mHitRect;
	QtVariantProperty* mSource;

public slots:
	virtual void readValues( const QModelIndex& index = QModelIndex() );
};

};

#endif // QAFRAMEPROPERTIES_H
