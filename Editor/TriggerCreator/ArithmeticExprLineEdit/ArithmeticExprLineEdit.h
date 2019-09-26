#ifndef ARITHMETICEXPRLINEEDIT_H
#define ARITHMETICEXPRLINEEDIT_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Parser/ArithmeticExprParser/ArithmeticExprParser.h>
class ArithmeticExprLineEdit: public QLineEdit
{
	Q_OBJECT
	private: QTextCharFormat multiplyFormat, divideFormat, remainderFormat, plusFormat, minusFormat, 
			                 bracketFormat, 
							 unknownArgFormat, standardFormat;
    private: int errorArgIndex;
    private: bool unknownArgsBacklightEnable;
	private: int32 variableType;
	public: ArithmeticExprLineEdit(QWidget* _parent);
	private slots: void NameIsChanged(QString _name);
	public: void ParseString(void);
	public: void SetVariableType(int32 _variableType);
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
	public: static QString _GetLEFT_BRACKET(void);
	public: static QString _GetRIGHT_BRACKET(void);
	public: void SetUnknownArgsBacklightEnable(bool _enable);
	public: void ClearErrorArgIndex(void);
	public: void SetErrorArgIndex(int _index);
	public: int GetErrorArgIndex(void)const;
};




#endif 

