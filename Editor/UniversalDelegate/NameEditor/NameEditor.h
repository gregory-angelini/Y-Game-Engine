#ifndef NAMEEDITOR_H
#define NAMEEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h> 
#include <Editor/StringLineEdit/StringLineEdit.h>
class NameEditor: public QWidget
{
	Q_OBJECT
	private: QTextCharFormat unknownArgFormat, standardFormat;
	private: StringLineEdit *fileNameEdit;
    public: NameEditor(QWidget* _parent = 0);
	public: void SetFileName(QString _fileName);
	public: QString GetFileName(void)const;
	private slots: void NameIsChanged(QString _name);
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	private: virtual void resizeEvent(QResizeEvent* _event);
	private: void CheckArg(void);
	private: static void _SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats);
	private: static void _ClearLineEditTextFormat(QLineEdit* _lineEdit);
};



#endif 

