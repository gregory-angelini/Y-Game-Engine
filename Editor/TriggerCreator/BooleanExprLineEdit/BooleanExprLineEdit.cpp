#include "BooleanExprLineEdit.h"
#include <Engine/Core/Numerical/Numerical.h>




BooleanExprLineEdit::BooleanExprLineEdit(QWidget* _parent): QLineEdit(_parent), errorArgIndex(-1), unknownArgsBacklightEnable(true)
{
	standardFormat.setForeground(Qt::black);
	standardFormat.setFontPointSize(10);
	andFormat.setForeground(Qt::black);
	andFormat.setBackground(QColor(210,210,210));
	andFormat.setFontPointSize(10);
	orFormat.setForeground(Qt::black);
	orFormat.setBackground(QColor(210,210,210));
	orFormat.setFontPointSize(10);
	xorFormat.setForeground(Qt::black);
	xorFormat.setBackground(QColor(210,210,210));
	xorFormat.setFontPointSize(10);
	notFormat.setForeground(Qt::black);
	notFormat.setBackground(QColor(210,210,210));
	notFormat.setFontPointSize(10);
	bracketFormat.setForeground(Qt::black);
	bracketFormat.setBackground(QColor(210,210,210));
	bracketFormat.setFontPointSize(10);
	unknownArgFormat.setForeground(Qt::black);
	unknownArgFormat.setBackground(QColor(231,186,188));
	unknownArgFormat.setFontPointSize(10);
	selectedArgFormat.setForeground(Qt::black);
	selectedArgFormat.setBackground(QColor(255,255,35));
	selectedArgFormat.setFontPointSize(10);


	connect(this, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
}


void BooleanExprLineEdit::NameIsChanged(QString _name)
{
	ClearErrorArgIndex();
	BooleanExprParser parser; 
	if(parser.Parse(_name.toStdString()) != BooleanExprParser::NO_ERRORS)
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



void BooleanExprLineEdit::focusInEvent(QFocusEvent* _event)
{
	QLineEdit::focusInEvent(_event);
	emit FocusIn();
}


void BooleanExprLineEdit::focusOutEvent(QFocusEvent* _event)
{
	QLineEdit::focusOutEvent(_event);
	emit FocusOut();
}



void BooleanExprLineEdit::SetUnknownArgsBacklightEnable(bool _enable)
{
	unknownArgsBacklightEnable = _enable;
}



void BooleanExprLineEdit::ParseString(void)
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

				SimpleList<BooleanExprParser::Token>::Node* token = tokens.Get(i);

				if(token->key.GetType() == BooleanExprParser::Token::VARIABLE)
				{
					if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::EVENT_ASSET) ||
					   AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::TRIGGER_ASSET))
					{
						exist = true;
					}
					else if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::VARIABLE_ASSET))
					{
						AbstractVariable* variable = AbstractVariable::_LoadFromFile(token->key.GetValue());
							
						if(variable)
						{
							if(variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
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

	if(GetSelectedArg().length() > 0)
	{
		if(tokens.IsNotEmpty())
		{
			for(int j = 0; j < tokens.GetSize(); j++)
			{
				SimpleList<BooleanExprParser::Token>::Node* it = tokens.Get(j);
				if(it->key.GetValue() == GetSelectedArg().toStdString())
				{
					QTextLayout::FormatRange format;
					format.start = it->key.GetPosition();
					format.length = it->key.GetValue().length();
					format.format = selectedArgFormat;
					formats.append(format);
				}
			} 
		}
	}
	_SetLineEditTextFormat(this, formats);
}



void BooleanExprLineEdit::FindSelectedArg(void)
{
	if(GetSelectedArg().length() > 0)
	{
		if(tokens.IsNotEmpty())
		{
			for(int j = 0; j < tokens.GetSize(); j++)
			{
				SimpleList<BooleanExprParser::Token>::Node* it = tokens.Get(j);
				if(it->key.GetValue() == GetSelectedArg().toStdString())
				{	
					if(cursorPosition() >= it->key.GetPosition())
					{
						setCursorPosition(it->key.GetPosition());
					}
					else
					{
						setCursorPosition(it->key.GetPosition() + it->key.GetValue().length() - 1);
					}
					break;
				}
			} 
		}
	}
}


void BooleanExprLineEdit::UpdateArgsList(void)
{
	BooleanExprParser parser;
	
	if(parser.Parse(text().toStdString()) == BooleanExprParser::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
}


void BooleanExprLineEdit::ClearErrorArgIndex(void)
{
	errorArgIndex = -1;
}



void BooleanExprLineEdit::SetErrorArgIndex(int _index)
{
	errorArgIndex = _index;
}


int BooleanExprLineEdit::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}



QString BooleanExprLineEdit::GetSelectedArg(void)const
{
	return selectedArg;
}



void BooleanExprLineEdit::SetSelectedArg(QString _name)
{
	selectedArg = _name;
}




QString BooleanExprLineEdit::_GetLEFT_BRACKET(void)
{
	return "(";
}


QString BooleanExprLineEdit::_GetRIGHT_BRACKET(void)
{
	return ")";
}



QString BooleanExprLineEdit::_GetAND(void)
{
	return "&";
}


QString BooleanExprLineEdit::_GetOR(void)
{
	return "|";
}


QString BooleanExprLineEdit::_GetXOR(void)
{
	return "^";
}


QString BooleanExprLineEdit::_GetNOT(void)
{
	return "~";
}




void BooleanExprLineEdit::_SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& formats)
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



void BooleanExprLineEdit::_ClearLineEditTextFormat(QLineEdit* _lineEdit)
{
    _SetLineEditTextFormat(_lineEdit, QList<QTextLayout::FormatRange>());
}
