#ifndef BOOLEANEXPREDITOR_H
#define BOOLEANEXPREDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/TriggerCreator/BooleanExprLineEdit/BooleanExprLineEdit.h>
class BooleanExprEditor: public QWidget
{
	Q_OBJECT
	private: BooleanExprLineEdit *fileNameEdit;
    private: QToolButton *helpButton;
    public: BooleanExprEditor(QWidget* _parent = 0);
	private slots: void ShowDoc(void);
	public: QString GetBooleanExprTable(void);
	public: void SetText(QString _text);
	public: QString GetText(void)const;
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	private: virtual void resizeEvent(QResizeEvent* _event);
};



#endif 

