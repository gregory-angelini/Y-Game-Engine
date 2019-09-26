#ifndef EQUALITYEXPRLINEEDIT_H
#define EQUALITYEXPRLINEEDIT_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Parser/EqualityExprParser/EqualityExprParser.h>
class EqualityExprLineEdit: public QLineEdit
{
	Q_OBJECT
	private: QTextCharFormat andFormat, orFormat, xorFormat, notFormat, 
			                 multiplyFormat, divideFormat, remainderFormat, plusFormat, minusFormat, 
			                 bracketFormat, 
			                 equalFormat, notEqualFormat, greaterFormat, greaterOrEqualFormat, lessFormat, lessOrEqualFormat, 
							 unknownArgFormat, standardFormat;
    private: int errorArgIndex;
	
	private: SimpleList<EqualityExprParser::Token> tokens;
    private: bool unknownArgsBacklightEnable;
	public: EqualityExprLineEdit(QWidget* _parent);
	private slots: void NameIsChanged(QString _name);
	public: void ParseString(void);
	public: void UpdateArgsList(void);
	private: static void _SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats);
	private: static void _ClearLineEditTextFormat(QLineEdit* _lineEdit);
    protected: virtual void focusInEvent(QFocusEvent* _event);
    protected: virtual void focusOutEvent(QFocusEvent* _event);
	signals: void FocusIn(void);
	signals: void FocusOut(void);
	public: static QString _GetMULTIPLY(void);
	public: static QString _GetDIVIDE(void);
	public: static QString _GetREMAINDER(void);
	public: static QString _GetPLUS(void);
	public: static QString _GetMINUS(void);
	public: static QString _GetAND(void);
	public: static QString _GetOR(void);
	public: static QString _GetXOR(void);
	public: static QString _GetNOT(void);
	public: static QString _GetEQUAL(void);
	public: static QString _GetNOT_EQUAL(void);
	public: static QString _GetGREATER(void);
	public: static QString _GetLESS(void);
	public: static QString _GetGREATER_OR_EQUAL(void);
	public: static QString _GetLESS_OR_EQUAL(void);
	public: static QString _GetLEFT_BRACKET(void);
	public: static QString _GetRIGHT_BRACKET(void);
	public: void SetUnknownArgsBacklightEnable(bool _enable);
	public: void ClearErrorArgIndex(void);
	public: void SetErrorArgIndex(int _index);
	public: int GetErrorArgIndex(void)const;
};




#endif 

