#include "QAAbstractProperties.h"
#include "QAPropertyManager.h"
#include "QAEditorFactory.h"

#include <QtTreePropertyBrowser>

#include <QApplication>
#include <QDebug>

namespace QA {

QA::PropertyManager* AbstractProperties::mPropertyManager = 0;
QA::EditorFactory* AbstractProperties::mEditorFactory = 0;

AbstractProperties::AbstractProperties( QA::Container* handle, QtAbstractPropertyBrowser* editor, QObject* parent )
	: QObject( parent )
{
	Q_ASSERT( handle );
	Q_ASSERT( editor );
	
	mReading = false;
	mHandle = handle;
	mPropertyEditor = editor;
	mPropertyManager = propertyManager();
	mEditorFactory = editorFactory();
	
	mPropertyEditor->setFactoryForManager( (QtAbstractPropertyManager*)mPropertyManager, (QtAbstractEditorFactory<QtAbstractPropertyManager>*)mEditorFactory );
	
	connect( mPropertyManager, SIGNAL( propertyChanged( QtProperty* ) ), this, SLOT( _q_propertyChanged( QtProperty* ) ) );
}

AbstractProperties::~AbstractProperties()
{
	QMap<QtProperty*, QString>::ConstIterator itProp = propertyToId.constBegin();
	
	while ( itProp != propertyToId.constEnd() )
	{
		delete itProp.key();
		itProp++;
	}
	
	propertyToId.clear();
	idToProperty.clear();
}

void AbstractProperties::addProperty( QtProperty* property, const QString& id )
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

void AbstractProperties::readValues( const QModelIndex& index )
{
	mIndex = index.isValid() ? QPersistentModelIndex( index ) : ( mIndex.isValid() ? mIndex : QPersistentModelIndex() );
}

QA::PropertyManager* AbstractProperties::propertyManager()
{
	if ( !mPropertyManager )
	{
		mPropertyManager = new QA::PropertyManager( qApp );
	}
	
	return mPropertyManager;
}

QA::EditorFactory* AbstractProperties::editorFactory()
{
	if ( !mEditorFactory )
	{
		mEditorFactory = new QA::EditorFactory( qApp );
	}
	
	return mEditorFactory;
}

void AbstractProperties::beginUpdate()
{
	mReading = true;;
}

void AbstractProperties::endUpdate()
{
	mReading = false;
}

void AbstractProperties::_q_propertyChanged( QtProperty* property )
{
	Q_UNUSED( property );
	
	if ( mIndex.isValid() && !mReading && propertyToId.contains( property ) )
	{
		writeValues( mIndex, property );
		
		const bool wasReading = mReading;
		mReading = true;
		property->setModified( true );
		mReading = wasReading;
	}
}

}
