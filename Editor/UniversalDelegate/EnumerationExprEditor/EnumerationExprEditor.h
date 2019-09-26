#ifndef ENUMERATIONEXPREDITOR_H
#define ENUMERATIONEXPREDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/TriggerCreator/EnumerationExprLineEdit/EnumerationExprLineEdit.h>
class EnumerationExprEditor: public QWidget
{
	Q_OBJECT
	private: EnumerationExprLineEdit *fileNameEdit;
    private: QToolButton *helpButton;
    public: EnumerationExprEditor(QWidget* _parent = 0);
	private slots: void ShowDoc(void);
	public: void SetText(QString _text);
	public: QString GetText(void)const;
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	private: virtual void resizeEvent(QResizeEvent* _event);
};



#endif 

