#include "HeaderProperties.h"
#include "QAEditable.h"
#include "QAAbstractProperties.h"
#include "QAPropertyManager.h"
#include "QAEditorFactory.h"

#include <QtTreePropertyBrowser>

#include <QDebug>

namespace QA {

HeaderProperties::HeaderProperties( QA::Container* handle, QtAbstractPropertyBrowser* editor, QObject* parent )
	: QObject( parent )
{
	Q_ASSERT( editor );
	Q_ASSERT( handle );
	
	mPropertyManager = QA::AbstractProperties::propertyManager();
	mEditorFactory = QA::AbstractProperties::editorFactory();
	mReading = false;
	mPropertyEditor = editor;
	mHandle = handle;
	
	mVersion = mPropertyManager->addProperty( QVariant::Int, tr( "Version" ) );
	addProperty( mVersion, "version" );
	
	QStringList list = QStringList( tr( "PNG Format" ) ) << tr( "JPG Format" );
	
	mFormat = mPropertyManager->addProperty( QA::PropertyManager::enumTypeId(), tr( "Format" ) );
	mFormat->setAttribute( "enumNames", list );
	addProperty( mFormat, "format" );
	
	mName = mPropertyManager->addProperty( QVariant::String, tr( "Name" ) );
	addProperty( mName, "name" );
	
	mFrameSize = mPropertyManager->addProperty( QVariant::Size, tr( "Frame Size" ) );
	addProperty( mFrameSize, "frameSize" );
	
	mFrameCount = mPropertyManager->addProperty( QVariant::Int, tr( "Frame Count" ) );
	addProperty( mFrameCount, "frameCount" );
	
	mPropertyEditor->setFactoryForManager( (QtAbstractPropertyManager*)mPropertyManager, (QtAbstractEditorFactory<QtAbstractPropertyManager>*)mEditorFactory );
	
	connect( mPropertyManager, SIGNAL( propertyChanged( QtProperty* ) ), this, SLOT( _q_propertyChanged( QtProperty* ) ) );
}

HeaderProperties::~HeaderProperties()
{
}

void HeaderProperties::addProperty( QtProperty* property, const QString& id )
{
	propertyToId[ property ] = id;
	idToProperty[ id ] = property;
	QtBrowserItem* item = mPropertyEditor->addProperty( property );
	
	QtTreePropertyBrowser* editor = qobject_cast<QtTreePropertyBrowser*>( mPropertyEditor );
	
	if ( editor && idToExpanded.contains( id ) )
	{
		editor->setExpanded( item, idToExpanded[ id ] );
	}
}

void HeaderProperties::readValues()
{
	beginUpdate();
	
	// header
	mVersion->setValue( mHandle->header().Version );
	mVersion->setModified( false );
	mVersion->setEnabled( false );
	
	mFormat->setValue( mHandle->header().Format );
	mFormat->setModified( false );
	mFormat->setEnabled( mHandle->header().isValid() );
	
	// ex header
	mName->setValue( mHandle->extendedHeader()->Name );
	mName->setModified( false );
	mName->setEnabled( mHandle->extendedHeader()->isValid() );
	
	mFrameSize->setValue( mHandle->extendedHeader()->FrameSize );
	mFrameSize->setModified( false );
	mFrameSize->setEnabled( mHandle->extendedHeader()->isValid() );
	
	mFrameCount->setValue( mHandle->extendedHeader()->FrameCount );
	mFrameCount->setModified( false );
	mFrameCount->setEnabled( false );
	
	endUpdate();
}

void HeaderProperties::writeValues( QtProperty* property )
{
	if ( !property || property == mVersion )
		mHandle->header().Version = QA::QAVersion( mVersion->value().toInt() );
	if ( !property || property == mFormat )
		mHandle->header().Format = QA::QAFormat( mFormat->value().toInt() );
	if ( !property || property == mName )
		mHandle->extendedHeader()->Name = mName->value().toString();
	if ( !property || property == mFrameSize )
		mHandle->extendedHeader()->FrameSize = mFrameSize->value().toSize();
	if ( !property || property == mFrameCount )
		mHandle->extendedHeader()->FrameCount = mFrameCount->value().toInt();
	
	emit mHandle->contentChanged();
}

void HeaderProperties::beginUpdate()
{
	mReading = true;;
}

void HeaderProperties::endUpdate()
{
	mReading = false;
}

void HeaderProperties::_q_propertyChanged( QtProperty* property )
{
	Q_UNUSED( property );
	
	if ( !mReading && propertyToId.contains( property ) )
	{
		writeValues( property );
		
		const bool wasReading = mReading;
		mReading = true;
		property->setModified( true );
		mReading = wasReading;
	}
}

};
