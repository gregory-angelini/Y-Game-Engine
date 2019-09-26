#include "EqualityExprLineEdit.h"
#include <Engine/Core/Numerical/Numerical.h>


EqualityExprLineEdit::EqualityExprLineEdit(QWidget* _parent): QLineEdit(_parent), errorArgIndex(-1), unknownArgsBacklightEnable(true)
{
	standardFormat.setForeground(Qt::black);
	multiplyFormat.setForeground(Qt::black);
	multiplyFormat.setBackground(QColor(210,210,210));
	divideFormat.setForeground(Qt::black);
	divideFormat.setBackground(QColor(210,210,210));
	remainderFormat.setForeground(Qt::black);
	remainderFormat.setBackground(QColor(210,210,210));
	plusFormat.setForeground(Qt::black);
	plusFormat.setBackground(QColor(210,210,210));
	minusFormat.setForeground(Qt::black);
	minusFormat.setBackground(QColor(210,210,210));
	andFormat.setForeground(Qt::black);
	andFormat.setBackground(QColor(210,210,210));
	orFormat.setForeground(Qt::black);
	orFormat.setBackground(QColor(210,210,210));
	xorFormat.setForeground(Qt::black);
	xorFormat.setBackground(QColor(210,210,210));
	notFormat.setForeground(Qt::black);
	notFormat.setBackground(QColor(210,210,210));
	bracketFormat.setForeground(Qt::black);
	bracketFormat.setBackground(QColor(210,210,210));
	equalFormat.setForeground(Qt::black);
	equalFormat.setBackground(QColor(210,210,210));
	notEqualFormat.setForeground(Qt::black);
	notEqualFormat.setBackground(QColor(210,210,210));
	greaterFormat.setForeground(Qt::black);
	greaterFormat.setBackground(QColor(210,210,210));
	greaterOrEqualFormat.setForeground(Qt::black);
	greaterOrEqualFormat.setBackground(QColor(210,210,210));
	lessFormat.setForeground(Qt::black);
	lessFormat.setBackground(QColor(210,210,210));
	lessOrEqualFormat.setForeground(Qt::black);
	lessOrEqualFormat.setBackground(QColor(210,210,210));
	unknownArgFormat.setForeground(Qt::black);
	unknownArgFormat.setBackground(QColor(231,186,188));


	connect(this, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
}


void EqualityExprLineEdit::focusInEvent(QFocusEvent* _event)
{
	QLineEdit::focusInEvent(_event);
	emit FocusIn();
}


void EqualityExprLineEdit::focusOutEvent(QFocusEvent* _event)
{
	QLineEdit::focusOutEvent(_event);
	emit FocusOut();
}



void EqualityExprLineEdit::NameIsChanged(QString _name)
{
	ClearErrorArgIndex();
	EqualityExprParser parser; 
	if(parser.Parse(_name.toStdString()) != EqualityExprParser::NO_ERRORS)
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



void EqualityExprLineEdit::UpdateArgsList(void)
{
	EqualityExprParser parser;
	
	if(parser.Parse(text().toStdString()) == EqualityExprParser::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
}


void EqualityExprLineEdit::SetUnknownArgsBacklightEnable(bool _enable)
{
	unknownArgsBacklightEnable = _enable;
}



void EqualityExprLineEdit::ParseString(void)
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
	while((index = string.indexOf(_GetMULTIPLY(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetMULTIPLY().length();
		format.format = multiplyFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetDIVIDE(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetDIVIDE().length();
		format.format = divideFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetREMAINDER(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetREMAINDER().length();
		format.format = remainderFormat;
		formats.append(format);
		++index;
	}

	index = 0;
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
	while((index = string.indexOf(_GetMINUS(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetMINUS().length();
		format.format = minusFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetAND(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetAND().length();
		format.format = andFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetOR(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetOR().length();
		format.format = orFormat;
		formats.append(format);
		++index;
	}

    index = 0;
	while((index = string.indexOf(_GetXOR(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetXOR().length();
		format.format = xorFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetNOT(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetNOT().length();
		format.format = notFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetLEFT_BRACKET(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetLEFT_BRACKET().length();
		format.format = bracketFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetRIGHT_BRACKET(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetRIGHT_BRACKET().length();
		format.format = bracketFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetEQUAL(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetEQUAL().length();
		format.format = equalFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetNOT_EQUAL(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetNOT_EQUAL().length();
		format.format = notEqualFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetGREATER(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetGREATER().length();
		format.format = greaterFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetGREATER_OR_EQUAL(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetGREATER_OR_EQUAL().length();
		format.format = greaterOrEqualFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetLESS(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetLESS().length();
		format.format = lessFormat;
		formats.append(format);
		++index;
	}

	index = 0;
	while((index = string.indexOf(_GetLESS_OR_EQUAL(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetLESS_OR_EQUAL().length();
		format.format = lessOrEqualFormat;
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
 
				SimpleList<EqualityExprParser::Token>::Node* token = tokens.Get(i);

				if(token->key.GetType() == EqualityExprParser::Token::VARIABLE)
				{
					if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::VARIABLE_ASSET))
					{
						AbstractVariable* variable = AbstractVariable::_LoadFromFile(token->key.GetValue());
							
						if(variable)
						{ 
							if(variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE ||
							   variable->GetValueType() == Variable<int32>::INTEGER_TYPE ||
							   variable->GetValueType() == Variable<float>::FLOAT_TYPE |
							   variable->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE |
							   variable->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE |
							   variable->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
							{
								exist = true;
							}
							delete variable;
						}
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


	

void EqualityExprLineEdit::ClearErrorArgIndex(void)
{
	errorArgIndex = -1;
}


void EqualityExprLineEdit::SetErrorArgIndex(int _index)
{
	errorArgIndex = _index;
}



int EqualityExprLineEdit::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}


QString EqualityExprLineEdit::_GetAND(void)
{
	return "&";
}


QString EqualityExprLineEdit::_GetOR(void)
{
	return "|";
}


QString EqualityExprLineEdit::_GetXOR(void)
{
	return "^";
}


QString EqualityExprLineEdit::_GetNOT(void)
{
	return "~";
}



QString EqualityExprLineEdit::_GetMULTIPLY(void)
{
	return "*";
}


QString EqualityExprLineEdit::_GetDIVIDE(void)
{
	return "/";
}


QString EqualityExprLineEdit::_GetREMAINDER(void)
{
	return "%";
}



QString EqualityExprLineEdit::_GetPLUS(void)
{
	return "+";
}


QString EqualityExprLineEdit::_GetMINUS(void)
{
	return "-";
}


QString EqualityExprLineEdit::_GetLEFT_BRACKET(void)
{
	return "(";
}


QString EqualityExprLineEdit::_GetRIGHT_BRACKET(void)
{
	return ")";
}



QString EqualityExprLineEdit::_GetEQUAL(void)
{
	return "=";
}


QString EqualityExprLineEdit::_GetNOT_EQUAL(void)
{
	return "!=";
}


QString EqualityExprLineEdit::_GetGREATER(void)
{
	return ">";
}


QString EqualityExprLineEdit::_GetLESS(void)
{
	return "<";
}


QString EqualityExprLineEdit::_GetGREATER_OR_EQUAL(void)
{
	return ">=";
}


QString EqualityExprLineEdit::_GetLESS_OR_EQUAL(void)
{
	return "<=";
}



void EqualityExprLineEdit::_SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& formats)
{
    if(_lineEdit)
	{
		QList<QInputMethodEvent::Attribute> attributes;

		foreach(const QTextLayout::FormatRange& formatRange, formats)
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



void EqualityExprLineEdit::_ClearLineEditTextFormat(QLineEdit* _lineEdit)
{
    _SetLineEditTextFormat(_lineEdit, QList<QTextLayout::FormatRange>());
}
