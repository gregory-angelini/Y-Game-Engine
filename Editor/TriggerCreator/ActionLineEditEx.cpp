#include "TriggerCreator.h"


ActionLineEditEx::ActionLineEditEx(QWidget* _parent): EnumerationExprLineEdit(_parent)
{}



void ActionLineEditEx::dragEnterEvent(QDragEnterEvent* _event)
{
	if(_event->mimeData()->hasFormat("text/uri-list")) 
	{ 	
		_event->acceptProposedAction();
	}
}
	

void ActionLineEditEx::dropEvent(QDropEvent* _event)
{
	QList<QUrl> urlList;
	QString fileName;
	QFileInfo info;

	if(_event->mimeData()->hasUrls())
	{
		urlList = _event->mimeData()->urls();
		
		if(!urlList.isEmpty())
		{
			for(int i = 0; i < urlList.size(); i++)
			{
				fileName = urlList[i].toLocalFile();
				
				bool exist = true;
					
				info.setFile(fileName);

				if(info.isFile())
				{
					exist = false;

					if(info.suffix() == AbstractAction::_GetFileSuffix().c_str())
					{
						fileName = info.completeBaseName();
						exist = true;
					}
				}
				if(exist)
				{
					if(IsEmpty(text()))
					{ 
						setText(fileName);
					} 
					else
					{
						if(OperationIsLast(text()))
						{
							while(QString(text().at(text().length() - 1)) == QString(" "))
							{
								QString str = text();
								str.chop(1);
								setText(str);
							}

							setText(text() + QString(" ") + fileName);
						}
						else
						{
							while(QString(text().at(text().length() - 1)) == QString(" "))
							{
								QString str = text();
								str.chop(1);
								setText(str);
							}

							QString operation = EnumerationExprLineEdit::_GetNEXT() + QString(" ");

							setText(text() + operation + fileName);
						}
					}
				}
			}
		}
	}	
	_event->acceptProposedAction();
}



bool ActionLineEditEx::OperationIsLast(QString _expr)
{
	if(!IsEmpty(_expr))
	{
		for(int i = _expr.length() - 1; i >= 0; i--)
		{
			if(QString(_expr.at(i)) != QString(" "))
			{
				if(QString(_expr.at(i)) == EnumerationExprLineEdit::_GetNEXT())
				{
					return true;
				}
				else
				{ 
					return false; 
				} 
			}
		}
	}
	return false;
}



bool ActionLineEditEx::IsEmpty(QString _expr)
{
	for(int i = 0; i < _expr.length(); i++)
	{
		if(QString(_expr.at(i)) != QString(" "))
		{
			return false;
		}
	}
	return true;
}