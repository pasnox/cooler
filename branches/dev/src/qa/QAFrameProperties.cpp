#include "QAFrameProperties.h"
#include "QAEditable.h"
#include "QAFrameModel.h"
#include "QAPropertyManager.h"
#include "QAEditorFactory.h"
#include "pIconManager.h"

#include "QDebug"

namespace QA {

FrameProperties::FrameProperties( QA::Container* handle, QtAbstractPropertyBrowser* editor, QObject* parent )
	: QA::AbstractProperties( handle, editor, parent )
{
	mName = mPropertyManager->addProperty( QVariant::String, tr( "Name" ) );
	addProperty( mName, "frame-name" );
	
	mFrameId = mPropertyManager->addProperty( QVariant::Int, tr( "Frame Id" ) );
	addProperty( mFrameId, "frame-frameId" );
	
	mData = mPropertyManager->addProperty( QVariant::Pixmap, tr( "Data" ) );
	addProperty( mData, "frame-data" );
	
	mOrigin = mPropertyManager->addProperty( QVariant::Point, tr( "Origin" ) );
	addProperty( mOrigin, "frame-origin" );
	
	mHitRect = mPropertyManager->addProperty( QVariant::Rect, tr( "Hit Rect" ) );
	addProperty( mHitRect, "frame-hitRect" );
	
	mSource = mPropertyManager->addProperty( QVariant::String, tr( "Source" ) );
	addProperty( mSource, "frame-source" );
}

FrameProperties::~FrameProperties()
{
	delete mPropertyManager;
	delete mEditorFactory;
}

void FrameProperties::readValues( const QModelIndex& index )
{
	QA::AbstractProperties::readValues( index );
	
	beginUpdate();
	
	mName->setValue( mIndex.data( FrameModel::NameRole ) );
	mName->setModified( false );
	mName->setEnabled( mIndex.isValid() );
	
	mFrameId->setValue( mIndex.data( FrameModel::FrameIdRole ) );
	mFrameId->setModified( false );
	mFrameId->setEnabled( false );
	
	mData->setValue( mIndex.data( FrameModel::DataRole ) );
	mData->setModified( false );
	mData->setEnabled( mIndex.isValid() );
	
	mOrigin->setAttribute( "minimum", QSize() );
	mOrigin->setAttribute( "maximum", mHandle->extendedHeader()->FrameSize );
	mOrigin->setValue( mIndex.data( FrameModel::OriginRole ) );
	mOrigin->setModified( false );
	mOrigin->setEnabled( mIndex.isValid() );
	
	mHitRect->setAttribute( "constraint", QRect( QPoint(), mHandle->extendedHeader()->FrameSize ) );
	mHitRect->setValue( mIndex.data( FrameModel::HitRectRole ) );
	mHitRect->setModified( false );
	mHitRect->setEnabled( mIndex.isValid() );
	
	mSource->setValue( mIndex.data( FrameModel::SourceRole ) );
	mSource->setModified( false );
	mSource->setEnabled( mIndex.isValid() );
	
	endUpdate();
}

void FrameProperties::writeValues( const QModelIndex& index, QtProperty* property )
{
	QAbstractItemModel* model = const_cast<QAbstractItemModel*>( index.model() );
	
	if ( !model )
	{
		Q_ASSERT( 0 );
		return;
	}
	
	if ( !property || property == mName )
		model->setData( index, mName->value(), FrameModel::NameRole );
	if ( !property || property == mFrameId )
		model->setData( index, mFrameId->value(), FrameModel::FrameIdRole );
	if ( !property || property == mData )
		model->setData( index, mData->value(), FrameModel::DataRole );
	if ( !property || property == mOrigin )
		model->setData( index, mOrigin->value(), FrameModel::OriginRole );
	if ( !property || property == mHitRect )
		model->setData( index, mHitRect->value(), FrameModel::HitRectRole );
	if ( !property || property == mSource )
		model->setData( index, mSource->value(), FrameModel::SourceRole );
}

};
