#ifndef LISTEDITOR_H
#define LISTEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/StringLineEdit/StringLineEdit.h>
class ListEditor: public QWidget
{
	Q_OBJECT
	private: QTextCharFormat unknownArgFormat, standardFormat;
	private: StringLineEdit *fileNameEdit;
    private: QToolButton *openFileButton;
    private: QToolButton *editAssetButton;
	private: int32 listType;
    public: ListEditor(QWidget* _parent = 0);
	private slots: void NameIsChanged(QString _name);
	public: void SetFileName(QString _fileName);
	public: QString GetFileName(void)const;
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	public slots: void OpenFinder(void);
	private: virtual void resizeEvent(QResizeEvent* _event);
	public slots: void CreateList(void);
	public slots: void EditList(void);
	private: void CheckArg(void);
	public: void SetListType(int32 _type);
	private: static void _SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats);
	private: static void _ClearLineEditTextFormat(QLineEdit* _lineEdit);
};




#endif 

