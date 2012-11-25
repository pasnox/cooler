#include "QAFrameModel.h"
#include "QAEditable.h"

#include <QDebug>

namespace QA {

FrameModel::FrameModel( QA::Container* handle, QObject* parent )
	: QAbstractItemModel( parent )
{
	Q_ASSERT( handle );
	mHandle = handle;
	mColumnCount = 6;
	
	connect( mHandle, SIGNAL( contentChanged() ), this, SIGNAL( layoutChanged() ) );
}

FrameModel::~FrameModel()
{
}

int FrameModel::columnCount( const QModelIndex& parent ) const
{
	return parent.isValid() ? 0 : mColumnCount;
}

QVariant FrameModel::data( const QModelIndex& index, int role ) const
{
	Q_ASSERT( mHandle );
	
	if ( !index.isValid() )
	{
		return QVariant();
	}
	
	switch ( role )
	{
		case Qt::DecorationRole:
			return index.column() == 0 ? mHandle->mFrames[ index.row() ].pixmap() : QVariant();
		case Qt::SizeHintRole:
			return index.column() == 0 ? mHandle->extendedHeader()->FrameSize : QVariant();
		case Qt::DisplayRole:
		{
			switch ( index.column() )
			{
				case 1: // name
					return data( index, FrameModel::NameRole );
				case 2: // frame id
					return data( index, FrameModel::FrameIdRole );
				case 3: // data
					return data( index, FrameModel::DataRole );
				case 4: // origin
					return data( index, FrameModel::OriginRole );
				case 5: // hit rect
					return data( index, FrameModel::HitRectRole );
				case 6: // source
					return data( index, FrameModel::SourceRole );
			}
			
			break;
		}
		case FrameModel::NameRole:
			return mHandle->mFrames[ index.row() ].Name;
		case FrameModel::FrameIdRole:
			return mHandle->mFrames[ index.row() ].FrameId;
		case FrameModel::DataRole:
			return mHandle->mFrames[ index.row() ].Data;
		case FrameModel::OriginRole:
			return mHandle->mFrames[ index.row() ].Origin;
		case FrameModel::HitRectRole:
			return mHandle->mFrames[ index.row() ].HitRect;
		case FrameModel::SourceRole:
			return mHandle->mFrames[ index.row() ].Source;
	}
	
	return QVariant();
}

QModelIndex FrameModel::index( int row, int column, const QModelIndex& parent ) const
{
	Q_ASSERT( mHandle );
	
	if ( parent.isValid() || row < 0 || row >= mHandle->extendedHeader()->FrameCount || column < 0 || column >= mColumnCount )
	{
		return QModelIndex();
	}
	
	return createIndex( row, column, row );
}

QModelIndex FrameModel::parent( const QModelIndex& index ) const
{
	Q_UNUSED( index );
	return QModelIndex();
}

int FrameModel::rowCount( const QModelIndex& parent ) const
{
	Q_ASSERT( mHandle );
	return !parent.isValid() ? mHandle->extendedHeader()->FrameCount : 0;
}

bool FrameModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	Q_ASSERT( mHandle );
	
	switch ( role )
	{
		case FrameModel::NameRole:
			mHandle->mFrames[ index.row() ].Name = value.toString();
			break;
		case FrameModel::FrameIdRole:
			mHandle->mFrames[ index.row() ].FrameId = value.value<qint32>();
			break;
		case FrameModel::DataRole:
			if ( value.type() == QVariant::Pixmap )
			{
				const QPixmap pixmap = value.value<QPixmap>();
				bool ok;
				const QByteArray data = QA::toByteArray( pixmap, mHandle->header().Format, -1, &ok );
				
				if ( !ok )
				{
					return false;
				}
				
				mHandle->mFrames[ index.row() ].Data = data;
			}
			else
			{
				mHandle->mFrames[ index.row() ].Data = value.toByteArray();
			}
			break;
		case FrameModel::OriginRole:
			mHandle->mFrames[ index.row() ].Origin = value.toPoint();
			break;
		case FrameModel::HitRectRole:
			mHandle->mFrames[ index.row() ].HitRect = value.toRect();
			break;
		case FrameModel::SourceRole:
			mHandle->mFrames[ index.row() ].Source = value.toString();
			break;
		default:
			return false;
	}
	
	emit dataChanged( index, index );
	emit mHandle->contentChanged();
	
	return true;
}

bool FrameModel::addFrame( int row, const QA::Frame& frame )
{
	Q_ASSERT( mHandle );
	beginInsertRows( QModelIndex(), row, row );
	mHandle->addFrame( row, frame );
	endInsertRows();
	
	return true;
}

bool FrameModel::removeFrame( int row )
{
	Q_ASSERT( mHandle );
	beginRemoveRows( QModelIndex(), row, row );
	mHandle->removeFrame( row );
	endRemoveRows();
	
	return true;
}

};
