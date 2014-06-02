#ifndef QABSTRACTVARIANTPROPERTYPOPUPEDITOR_H
#define QABSTRACTVARIANTPROPERTYPOPUPEDITOR_H

#include <qabstractpropertyeditor.h>

class QAbstractVariantPropertyPopUpEditor : public QAbstractPropertyEditor
{
	Q_OBJECT

public:
	QAbstractVariantPropertyPopUpEditor(QWidget *parent);
	virtual ~QAbstractVariantPropertyPopUpEditor();

protected:
	QPushButton* openEditorButton;
};

#endif // QABSTRACTVARIANTPROPERTYPOPUPEDITOR_H
