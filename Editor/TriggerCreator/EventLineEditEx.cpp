#include "TriggerCreator.h"


EventLineEditEx::EventLineEditEx(QWidget* _parent): BooleanExprLineEdit(_parent)
{}



void EventLineEditEx::dragEnterEvent(QDragEnterEvent* _event)
{
	if(_event->mimeData()->hasFormat("text/uri-list")) 
	{ 	
		_event->acceptProposedAction();
	}
}
	

void EventLineEditEx::dropEvent(QDropEvent* _event)
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

					if(info.suffix() == AbstractEvent::_GetFileSuffix().c_str() ||
					   info.suffix() == Trigger::_GetFileSuffix().c_str())
					{
						fileName = info.completeBaseName();
						exist = true;
					}
					else if(info.suffix() == AbstractVariable::_GetFileSuffix().c_str())
					{
						fileName = info.completeBaseName();

						AbstractVariable* variable = AbstractVariable::_LoadFromFile(fileName.toAscii().data());
						
						if(variable)
						{
							if(variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
							{
								exist = true;
							}
							delete variable;
						}
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

							QString operation = QString(" ") + _GetAND() + QString(" ");

							setText(text() + operation + fileName);
						}
					}
				}
			}
		}
	}	
	_event->acceptProposedAction();
}



bool EventLineEditEx::IsEmpty(QString _expr)
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



bool EventLineEditEx::OperationIsLast(QString _expr)
{
	if(!IsEmpty(_expr))
	{
		for(int i = _expr.length() - 1; i >= 0; i--)
		{
			if(QString(_expr.at(i)) != QString(" "))
			{ 
				if(QString(_expr.at(i)) == _GetAND() ||
				   QString(_expr.at(i)) == _GetOR() ||
				   QString(_expr.at(i)) == _GetXOR() ||
				   QString(_expr.at(i)) == _GetNOT() ||
				   QString(_expr.at(i)) == _GetLEFT_BRACKET())
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
