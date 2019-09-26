#include "PlayerControlEventListWidget.h"


PlayerControlEventListWidget::ItemInputEvent::ItemInputEvent(QTreeWidgetItem* _parent): QTreeWidgetItem(_parent)
{
	SetDefaultState();
}


void PlayerControlEventListWidget::ItemInputEvent::SetDefaultState(void)
{
	toolTipList.push_back("status");
	toolTipList.push_back("device type");
	toolTipList.push_back("event type");
	toolTipList.push_back("event key");

	for(int i = 0; i < SIZE; i++)
	{
		propertyValid[i] = false;
	}
}


void PlayerControlEventListWidget::ItemInputEvent::SetPropertyValid(int _index, bool _valid)
{
	if(_index >= 1 && _index < SIZE)
	{
		propertyValid[_index] = _valid;
	}
}


bool PlayerControlEventListWidget::ItemInputEvent::IsKeyValid(void)const
{
	if(propertyValid[KEY_PROPERTY]) { return true; }
	return false;
}


bool PlayerControlEventListWidget::ItemInputEvent::IsValid(void)const
{
	for(int i = 1; i < SIZE; i++)
	{
		if(!propertyValid[i]) { return false; }
	}
	return true;
}


QVariant PlayerControlEventListWidget::ItemInputEvent::data(int _column, int _role)const
{
	if(_role == Qt::ToolTipRole)
	{
		if(_column < toolTipList.size())
		{
			return QVariant(toolTipList.at(_column));
		}
	}
	else if(_role == Qt::BackgroundRole)
	{
		return QColor(255,0,0);
	}
	
	return QTreeWidgetItem::data(_column, _role);
}

