#include "StringExprLineEdit.h"
#include <Engine/Core/Numerical/Numerical.h>



StringExprLineEdit::StringExprLineEdit(QWidget* _parent): QLineEdit(_parent), errorArgIndex(-1), unknownArgsBacklightEnable(true)
{
	standardFormat.setForeground(Qt::black);
	plusFormat.setForeground(Qt::black);
	plusFormat.setBackground(QColor(210,210,210));
	constFormat.setForeground(Qt::black);
	constFormat.setBackground(QColor(210,210,210));
	unknownArgFormat.setForeground(Qt::black);
	unknownArgFormat.setBackground(QColor(231,186,188));


	connect(this, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
}



void StringExprLineEdit::focusInEvent(QFocusEvent* _event)
{
	QLineEdit::focusInEvent(_event);
	emit FocusIn();
}


void StringExprLineEdit::focusOutEvent(QFocusEvent* _event)
{
	QLineEdit::focusOutEvent(_event);
	emit FocusOut();
}



void StringExprLineEdit::NameIsChanged(QString _name)
{
	ClearErrorArgIndex();
	StringExprParser parser; 
	if(parser.Parse(_name.toStdString()) != StringExprParser::NO_ERRORS)
	{
		SetErrorArgIndex(parser.GetErrorArgIndex());
		
		QString text = "Error(" + QString::number(parser.GetErrorArgIndex() + 1) + "): " + QString(parser.GetCurrentErrorString().c_str()) + " '" + QString(parser.GetErrorArg().c_str()) + "'";
		setToolTip(text);
	}
	else 
	{ 
		setToolTip(""); 
	} 
	ParseString();
}



void StringExprLineEdit::UpdateArgsList(void)
{
	StringExprParser parser;
	
	if(parser.Parse(text().toStdString()) == StringExprParser::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
}


void StringExprLineEdit::SetUnknownArgsBacklightEnable(bool _enable)
{
	unknownArgsBacklightEnable = _enable;
}



void StringExprLineEdit::ParseString(void)
{
	_ClearLineEditTextFormat(this);

	QList<QTextLayout::FormatRange> formats;
	QString string = text();
	int index = 0;

	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = string.length();
		format.format = standardFormat;
		formats.append(format);
	}
	while((index = string.indexOf(_GetPLUS(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetPLUS().length();
		format.format = plusFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetCONST(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetCONST().length();
		format.format = constFormat;
		formats.append(format);
		++index;
	}
	if(errorArgIndex != -1)
	{
		QTextLayout::FormatRange format;
		format.start = errorArgIndex;
		format.length = 1;
		format.format = unknownArgFormat;
		formats.append(format);
	}
	if(errorArgIndex == -1 && unknownArgsBacklightEnable) 
	{ 
		UpdateArgsList();

		if(tokens.IsNotEmpty())
		{
			for(int i = 0; i < tokens.GetSize(); i++)
			{  
				bool exist = false;
 
				SimpleList<StringExprParser::Token>::Node* token = tokens.Get(i);

				if(token->key.GetType() == StringExprParser::Token::STRING)
				{
					if(AssetLibrary::_IsAssetExist(token->key.GetValue()) != AssetLibrary::UNKNOWN_ASSET)
					{
						exist = true;
					}
					
					if(!exist)
					{
						QTextLayout::FormatRange format;
						format.start = token->key.GetPosition();
						format.length = token->key.GetValue().length();
						format.format = unknownArgFormat;
						formats.append(format);
						
						QString text = "One or more args does not exist";
						setToolTip(text);
					}
				}
				else if(token->key.GetType() == StringExprParser::Token::VARIABLE)
				{
					AbstractVariable* variable = AbstractVariable::_LoadFromFile(token->key.GetValue());

					if(variable)
					{
						if(variable->GetValueType() == Variable<int32>::INTEGER_TYPE ||
						   variable->GetValueType() == Variable<StringANSI>::STRING_TYPE)
						{
							exist = true;
						}
						delete variable;
					}
					
					if(!exist)
					{
						QTextLayout::FormatRange format;
						format.start = token->key.GetPosition();
						format.length = token->key.GetValue().length();
						format.format = unknownArgFormat;
						formats.append(format);
						
						QString text = "One or more args does not exist";
						setToolTip(text);
					}
				}
			}
		}
	}

	_SetLineEditTextFormat(this, formats);
}


	

void StringExprLineEdit::ClearErrorArgIndex(void)
{
	errorArgIndex = -1;
}


void StringExprLineEdit::SetErrorArgIndex(int _index)
{
	errorArgIndex = _index;
}



int StringExprLineEdit::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}



QString StringExprLineEdit::_GetPLUS(void)
{
	return "+";
}



QString StringExprLineEdit::_GetCONST(void)
{
	return "$";
}



void StringExprLineEdit::_SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats)
{
    if(_lineEdit)
	{
		QList<QInputMethodEvent::Attribute> attributes;

		foreach(const QTextLayout::FormatRange& formatRange, _formats)
		{
			QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
			int start = formatRange.start - _lineEdit->cursorPosition();
			int length = formatRange.length;
			QVariant value = formatRange.format;
			attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
		}

		QInputMethodEvent event(QString(), attributes);
		QCoreApplication::sendEvent(_lineEdit, &event);
	}
}



void StringExprLineEdit::_ClearLineEditTextFormat(QLineEdit* _lineEdit)
{
    _SetLineEditTextFormat(_lineEdit, QList<QTextLayout::FormatRange>());
}
