#include "EnumerationExprLineEdit.h"
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



EnumerationExprLineEdit::EnumerationExprLineEdit(QWidget* _parent): QLineEdit(_parent), errorArgIndex(-1), unknownArgsBacklightEnable(true)
{
	standardFormat.setForeground(Qt::black);
	standardFormat.setFontPointSize(10);
	nextFormat.setForeground(Qt::black);
	nextFormat.setBackground(QColor(180,180,180));
	nextFormat.setFontPointSize(10);
	unknownArgFormat.setForeground(Qt::black);
	unknownArgFormat.setBackground(QColor(231,186,188));
	unknownArgFormat.setFontPointSize(10);
	selectedArgFormat.setForeground(Qt::black);
	selectedArgFormat.setBackground(QColor(255,255,35));
	selectedArgFormat.setFontPointSize(10);


	connect(this, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
}


void EnumerationExprLineEdit::NameIsChanged(QString _name)
{ 
	ClearErrorArgIndex();
	EnumerationExprParser parser; 
	if(parser.Parse(_name.toStdString()) != EnumerationExprParser::NO_ERRORS)
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


void EnumerationExprLineEdit::focusInEvent(QFocusEvent* _event)
{
	QLineEdit::focusInEvent(_event);
	emit FocusIn();
}


void EnumerationExprLineEdit::focusOutEvent(QFocusEvent* _event)
{
	QLineEdit::focusOutEvent(_event);
	emit FocusOut();
}


void EnumerationExprLineEdit::SetUnknownArgsBacklightEnable(bool _enable)
{
	unknownArgsBacklightEnable = _enable;
}


void EnumerationExprLineEdit::ParseString(void)
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
	while((index = string.indexOf(_GetNEXT(), index)) != -1)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = _GetNEXT().length();
		format.format = nextFormat;
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

				SimpleList<EnumerationExprParser::Token>::Node* token = tokens.Get(i);

				if(token->key.GetType() == EnumerationExprParser::Token::VARIABLE)
				{
					if(AssetLibrary::_IsAssetExist(token->key.GetValue(), AssetLibrary::ACTION_ASSET))
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
			}
		}
	}

	if(GetSelectedArg().length() > 0)
	{
		if(tokens.IsNotEmpty())
		{
			for(int j = 0; j < tokens.GetSize(); j++)
			{
				SimpleList<EnumerationExprParser::Token>::Node* it = tokens.Get(j);
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



void EnumerationExprLineEdit::FindSelectedArg(void)
{
	if(GetSelectedArg().length() > 0)
	{
		if(tokens.IsNotEmpty())
		{
			for(int j = 0; j < tokens.GetSize(); j++)
			{
				SimpleList<EnumerationExprParser::Token>::Node* it = tokens.Get(j);
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



void EnumerationExprLineEdit::UpdateArgsList(void)
{
	EnumerationExprParser parser;
	
	if(parser.Parse(text().toStdString()) == EnumerationExprParser::NO_ERRORS)
	{
		tokens = parser.GetTokens();
	}
}



void EnumerationExprLineEdit::ClearErrorArgIndex(void)
{
	errorArgIndex = -1;
}


void EnumerationExprLineEdit::SetErrorArgIndex(int _index)
{
	errorArgIndex = _index;
}


int EnumerationExprLineEdit::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}




QString EnumerationExprLineEdit::GetSelectedArg(void)const
{
	return selectedArg;
}



void EnumerationExprLineEdit::SetSelectedArg(QString _name)
{
	selectedArg = _name;
}



QString EnumerationExprLineEdit::_GetNEXT(void)
{
	return ",";
}



void EnumerationExprLineEdit::_SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats)
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



void EnumerationExprLineEdit::_ClearLineEditTextFormat(QLineEdit* _lineEdit)
{
    _SetLineEditTextFormat(_lineEdit, QList<QTextLayout::FormatRange>());
}
