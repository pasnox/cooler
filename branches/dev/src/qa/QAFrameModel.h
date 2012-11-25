#ifndef QAMODEL_H
#define QAMODEL_H

#include <QAbstractItemModel>

#include "QA.h"

class QtAbstractPropertyBrowser;

namespace QA {

class Container;

class FrameModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	enum CustomRoles
	{
		NameRole = Qt::UserRole,
		FrameIdRole,
		DataRole,
		OriginRole,
		HitRectRole,
		SourceRole
	};
	
	FrameModel( QA::Container* handle, QObject* parent = 0 );
	virtual ~FrameModel();
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
	
	bool addFrame( int row, const QA::Frame& frame );
	bool removeFrame( int row );

protected:
	QA::Container* mHandle;
	int mColumnCount;
};

};

#endif // QAMODEL_H
