#ifndef QAEDITABLE_H
#define QAEDITABLE_H

#include <QObject>

#include "QA.h"

namespace QA {

class Container : public QObject
{
	Q_OBJECT
	
	friend class FrameModel;
	friend class HeaderProperties;
	friend class FrameProperties;
	
public:
	Container( QObject* parent = 0 );
	virtual ~Container();
	
	Container& operator=( const Container& other );
	bool operator==( const Container& other ) const;
	bool operator!=( const Container& other ) const;
	
	QA::Header& header() const;
	QA::ExtendedHeader* extendedHeader() const;
	QA::FrameList& frames() const;
	
	void reset();
	/*
	bool resizeChunks();
	bool setTransparentPixel( QRgb pixel );
	void clearFrames();
	QA::Frame* frame( int index ) const;
	*/
	
	void addFrame( int index, const QA::Frame& frame );
	void removeFrame( int index );
	
	bool open( const QString& filePath );
	bool saveAs( const QString& filePath );
	
	static bool createAnimation( QA::Header* header, QA::ExtendedHeader* extendedHeader, const QA::FrameList& frames, const QString& filePath );

protected:
	mutable QA::Header mHeader;
	mutable QA::ExtendedHeader* mExHeader;
	mutable QA::FrameList mFrames;
	
	void updateFrameIds();

signals:
	void contentChanged();
};

};

#endif // QAEDITABLE_H
