#ifndef ARITHMETICEXPREDITOR_H
#define ARITHMETICEXPREDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/TriggerCreator/ArithmeticExprLineEdit/ArithmeticExprLineEdit.h>
class ArithmeticExprEditor: public QWidget
{
	Q_OBJECT
	private: ArithmeticExprLineEdit *fileNameEdit;
    private: QToolButton *helpButton;
    public: ArithmeticExprEditor(QWidget* _parent, int32 _variableType);
	private slots: void ShowDoc(void);
	public: QString GetIntegerArithmeticExprTable(void);
	public: QString GetFloatArithmeticExprTable(void);
	public: QString GetVector2ArithmeticExprTable(void);
	public: QString GetVector3ArithmeticExprTable(void);
	public: QString GetQuaternionArithmeticExprTable(void);
	public: void SetText(QString _text);
	public: QString GetText(void)const;
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	private: virtual void resizeEvent(QResizeEvent* _event);
};



#endif 

