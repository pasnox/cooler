#ifndef QAEDITORFACTORY_H
#define QAEDITORFACTORY_H

#include <QtVariantEditorFactory>

namespace QA {

class EditorFactory : public QtVariantEditorFactory
{
	Q_OBJECT
	
public:
	EditorFactory( QObject* parent = 0 );
	virtual ~EditorFactory();
};

};

#endif // QAEDITORFACTORY_H
