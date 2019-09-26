#ifndef STRINGEXPREDITOR_H
#define STRINGEXPREDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/TriggerCreator/StringExprLineEdit/StringExprLineEdit.h>
class StringExprEditor: public QWidget
{
	Q_OBJECT
	private: StringExprLineEdit *fileNameEdit;
    private: QToolButton *helpButton;
    public: StringExprEditor(QWidget* _parent = 0);
	private slots: void ShowDoc(void);
	public: QString GetStringExprTable(void);
	public: void SetText(QString _text);
	public: QString GetText(void)const;
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	private: virtual void resizeEvent(QResizeEvent* _event);
};



#endif 

