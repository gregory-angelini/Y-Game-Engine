#ifndef BOOLEANEXPRLINEEDIT_H
#define BOOLEANEXPRLINEEDIT_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Parser/BooleanExprParser/BooleanExprParser.h>
class BooleanExprLineEdit: public QLineEdit
{
	Q_OBJECT
	private: QTextCharFormat andFormat, orFormat, xorFormat, notFormat, 
			                 bracketFormat, 
							 unknownArgFormat, selectedArgFormat, standardFormat;
	private: int errorArgIndex;

	private: SimpleList<BooleanExprParser::Token> tokens;
	private: bool unknownArgsBacklightEnable;

    private: QString selectedArg;
	public: BooleanExprLineEdit(QWidget* _parent);
	private slots: void NameIsChanged(QString _name);
	public: void ParseString(void);
	public: void UpdateArgsList(void);
	private: static void _SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats);
	private: static void _ClearLineEditTextFormat(QLineEdit* _lineEdit);
	public: void FindSelectedArg(void);
    protected: virtual void focusInEvent(QFocusEvent* _event);
    protected: virtual void focusOutEvent(QFocusEvent* _event);
	signals: void FocusIn(void);
	signals: void FocusOut(void);
	public: static QString _GetAND(void);
	public: static QString _GetOR(void);
	public: static QString _GetXOR(void);
	public: static QString _GetNOT(void);
	public: static QString _GetLEFT_BRACKET(void);
	public: static QString _GetRIGHT_BRACKET(void);
	public: void SetUnknownArgsBacklightEnable(bool _enable);
	public: void SetSelectedArg(QString _name);
	public: QString GetSelectedArg(void)const;
	public: void ClearErrorArgIndex(void);
	public: void SetErrorArgIndex(int _index);
	public: int GetErrorArgIndex(void)const;
};




#endif 

