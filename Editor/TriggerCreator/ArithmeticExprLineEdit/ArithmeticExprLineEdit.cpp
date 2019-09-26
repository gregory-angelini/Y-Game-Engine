#include "ArithmeticExprLineEdit.h"
#include <Engine/Core/Numerical/Numerical.h>


ArithmeticExprLineEdit::ArithmeticExprLineEdit(QWidget* _parent): QLineEdit(_parent), errorArgIndex(-1), unknownArgsBacklightEnable(true), variableType(0)
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
	bracketFormat.setForeground(Qt::black);
	bracketFormat.setBackground(QColor(210,210,210));
	unknownArgFormat.setForeground(Qt::black);
	unknownArgFormat.setBackground(QColor(231,186,188));


	connect(this, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
}


void ArithmeticExprLineEdit::focusInEvent(QFocusEvent* _event)
{
	QLineEdit::focusInEvent(_event);
	emit FocusIn();
}


void ArithmeticExprLineEdit::focusOutEvent(QFocusEvent* _event)
{
	QLineEdit::focusOutEvent(_event);
	emit FocusOut();
}


void ArithmeticExprLineEdit::NameIsChanged(QString _name)
{
	ClearErrorArgIndex();
	ArithmeticExprParser<int32> parser; 
	if(parser.Parse(_name.toStdString()) != ArithmeticExprParser<int32>::NO_ERRORS)
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




void ArithmeticExprLineEdit::SetUnknownArgsBacklightEnable(bool _enable)
{
	unknownArgsBacklightEnable = _enable;
}



void ArithmeticExprLineEdit::SetVariableType(int32 _variableType)
{
	variableType = _variableType;
}
		


void ArithmeticExprLineEdit::ParseString(void)
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
		SimpleList<ArithmeticExprParser<int32>::Token> tokens;
		
		ArithmeticExprParser<int32> parser;
		
		if(parser.Parse(text().toStdString()) == ArithmeticExprParser<int32>::NO_ERRORS)
		{
			tokens = parser.GetTokens();
		}

		if(tokens.IsNotEmpty())
		{
			for(int i = 0; i < tokens.GetSize(); i++)
			{  
				bool exist = false;
 
				SimpleList<ArithmeticExprParser<int32>::Token>::Node* token = tokens.Get(i);

				if(token->key.GetType() == ArithmeticExprParser<int32>::Token::VARIABLE)
				{
					if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::VARIABLE_ASSET))
					{
						AbstractVariable* variable = AbstractVariable::_LoadFromFile(token->key.GetValue());
							
						if(variable)
						{
							if(variable->GetValueType() == variableType)
							{
								exist = true;
							}
							else if(variableType == Variable<int32>::INTEGER_TYPE)
							{
								if(variable->GetValueType() == Variable<float>::FLOAT_TYPE)
								{
									exist = true;
								}
							}
							else if(variableType == Variable<float>::FLOAT_TYPE)
							{
								if(variable->GetValueType() == Variable<int32>::INTEGER_TYPE)
								{
									exist = true;
								}
							}
							else if(variableType == Variable<Vector2>::VECTOR_2_TYPE)
							{
								if(variable->GetValueType() == Variable<int32>::INTEGER_TYPE ||
								   variable->GetValueType() == Variable<float>::FLOAT_TYPE)
								{
									exist = true;
								}
							}
							else if(variableType == Variable<Vector3>::VECTOR_3_TYPE)
							{
								if(variable->GetValueType() == Variable<int32>::INTEGER_TYPE ||
								   variable->GetValueType() == Variable<float>::FLOAT_TYPE)
								{
									exist = true;
								}
							}
							else if(variableType == Variable<Quaternion>::QUATERNION_TYPE)
							{
								if(variable->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
								{
									exist = true;
								}
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


	

void ArithmeticExprLineEdit::ClearErrorArgIndex(void)
{
	errorArgIndex = -1;
}


void ArithmeticExprLineEdit::SetErrorArgIndex(int _index)
{
	errorArgIndex = _index;
}



int ArithmeticExprLineEdit::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}



QString ArithmeticExprLineEdit::_GetMULTIPLY(void)
{
	return "*";
}


QString ArithmeticExprLineEdit::_GetDIVIDE(void)
{
	return "/";
}


QString ArithmeticExprLineEdit::_GetREMAINDER(void)
{
	return "%";
}



QString ArithmeticExprLineEdit::_GetPLUS(void)
{
	return "+";
}


QString ArithmeticExprLineEdit::_GetMINUS(void)
{
	return "-";
}


QString ArithmeticExprLineEdit::_GetLEFT_BRACKET(void)
{
	return "(";
}


QString ArithmeticExprLineEdit::_GetRIGHT_BRACKET(void)
{
	return ")";
}




void ArithmeticExprLineEdit::_SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& formats)
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



void ArithmeticExprLineEdit::_ClearLineEditTextFormat(QLineEdit* _lineEdit)
{
    _SetLineEditTextFormat(_lineEdit, QList<QTextLayout::FormatRange>());
}
