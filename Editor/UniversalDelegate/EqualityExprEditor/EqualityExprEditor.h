#ifndef EQUALITYEXPREDITOR_H
#define EQUALITYEXPREDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/TriggerCreator/EqualityExprLineEdit/EqualityExprLineEdit.h>
class EqualityExprEditor: public QWidget
{
	Q_OBJECT
	private: EqualityExprLineEdit *fileNameEdit;
    private: QToolButton *helpButton;
    public: EqualityExprEditor(QWidget* _parent = 0);
	private slots: void ShowDoc(void);
	public: QString GetEqualityExprTable(void);
	public: void SetText(QString _text);
	public: QString GetText(void)const;
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	private: virtual void resizeEvent(QResizeEvent* _event);
};



#endif 

