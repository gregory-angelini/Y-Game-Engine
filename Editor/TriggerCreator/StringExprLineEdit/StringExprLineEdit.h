#ifndef STRINGEXPRLINEEDIT_H
#define STRINGEXPRLINEEDIT_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Trigger/StringExprParser/StringExprParser.h>
class StringExprLineEdit: public QLineEdit
{
	Q_OBJECT
	private: QTextCharFormat plusFormat, constFormat, 
			                 unknownArgFormat, standardFormat;
	private: int errorArgIndex;
	
	private: SimpleList<StringExprParser::Token> tokens;
    private: bool unknownArgsBacklightEnable;
	public: StringExprLineEdit(QWidget* _parent);
	private slots: void NameIsChanged(QString _name);
	public: void ParseString(void);
	public: void UpdateArgsList(void);
	private: static void _SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats);
	private: static void _ClearLineEditTextFormat(QLineEdit* _lineEdit);
    protected: virtual void focusInEvent(QFocusEvent* _event);
    protected: virtual void focusOutEvent(QFocusEvent* _event);
	signals: void FocusIn(void);
	signals: void FocusOut(void);
	public: static QString _GetPLUS(void);
	public: static QString _GetCONST(void);
	public: void SetUnknownArgsBacklightEnable(bool _enable);
	public: void ClearErrorArgIndex(void);
	public: void SetErrorArgIndex(int _index);
	public: int GetErrorArgIndex(void)const;
};




#endif 

