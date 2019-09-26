#include "PlayerControlEventListWidget.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>





PlayerControlEventListWidget::PlayerControlEventListWidget(QWidget* _parent): QTreeWidget(_parent)
{
	setAcceptDrops(true);
	setDragEnabled(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
	

	header()->setMovable(false);
	setColumnCount(4);
	headerItem()->setText(0, "");
	headerItem()->setText(1, "Device");
	headerItem()->setText(2, "Type");
	headerItem()->setText(3, "Key");
	QFont font("Verdana", 9, QFont::Normal, false);
	headerItem()->setFont(1, font);
	headerItem()->setFont(2, font);
	headerItem()->setFont(3, font);
	headerItem()->setTextAlignment(1, Qt::AlignHCenter);
	headerItem()->setTextAlignment(2, Qt::AlignHCenter);
	headerItem()->setTextAlignment(3, Qt::AlignHCenter);
	setColumnWidth(0, 50);
	setColumnWidth(1, 100);
	setColumnWidth(2, 140);
	setColumnWidth(3, 140);
	header()->setResizeMode(0, QHeaderView::Fixed);
	header()->setResizeMode(1, QHeaderView::Fixed);
	header()->setResizeMode(2, QHeaderView::Fixed);
	header()->setResizeMode(3, QHeaderView::Fixed);
	QPalette p(palette());
	p.setColor(QPalette::Base, QColor(255, 255, 255));
	setPalette(p);

	setStyleSheet("QTreeView::item { height: 24px; }"
		          "QTreeWidget::item:!selected { border: 2px solid gainsboro; "
		                                        "border-left: none; border-top: none; }"
		          "QTreeWidget::item:selected { background-color: rgb(149,199,233);"
                                               "border: 2px solid gainsboro;"
											   "border-left: none; border-top: none; }");
	setIconSize(QSize(25, 25));
	keyboardIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "PlayerControlCreator/keyboard_icon.png");
	mouseIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "PlayerControlCreator/mouse_icon.png");
	gamepadIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "PlayerControlCreator/gamepad_icon.png");
	delegate = new UniversalDelegate(this);
	setItemDelegate(delegate);
	delegate->SetColumnDelegate(ItemInputEvent::ICON_PROPERTY, UniversalDelegate::NO_EDIT);
	delegate->SetColumnDelegate(ItemInputEvent::DEVICE_PROPERTY, UniversalDelegate::COMBO_BOX_EDITOR);
	deviceList.push_back("KEYBOARD");
	deviceList.push_back("MOUSE");

	delegate->SetComboValuesForColumn(ItemInputEvent::DEVICE_PROPERTY, deviceList);
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(UpdateRow(QTreeWidgetItem*, int)));


	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(ItemClicked(QTreeWidgetItem*, int)));
	keyboardEventTypeList.push_back("KEY_DOWN");
	keyboardEventTypeList.push_back("KEY_UP");
	mouseEventTypeList.push_back("KEY_DOWN");
	mouseEventTypeList.push_back("KEY_UP");
	mouseEventTypeList.push_back("DOUBLE_CLICK");
	mouseEventTypeList.push_back("MOVE");
	mouseEventTypeList.push_back("WHEEL");
	keyMouseEventTypeList.push_back("LEFT_BUTTON");
	keyMouseEventTypeList.push_back("MIDDLE_BUTTON");
	keyMouseEventTypeList.push_back("RIGHT_BUTTON");

}


PlayerControlEventListWidget::~PlayerControlEventListWidget(void)
{
	delete delegate;
}



  
void PlayerControlEventListWidget::mousePressEvent(QMouseEvent* _event)
{
	QTreeWidget::mousePressEvent(_event);
	
	if(_event->button() == Qt::LeftButton)
	{
		cursorPoint = _event->pos();
		 
		QTreeWidgetItem* it = itemAt(cursorPoint);

		if(it == 0)
		{
			QList<QTreeWidgetItem*> list = selectedItems();

			QTreeWidgetItem* it = list.at(0);

			if(it)
			{
				if(it->isSelected())
				{
					it->setSelected(false);
				}
			}
		}
	}
}



QTreeWidgetItem* PlayerControlEventListWidget::GetTopItem(QList<QTreeWidgetItem*> &_selectedList)
{
	QList<QTreeWidgetItem*> selectedList = _selectedList;

	QTreeWidgetItem* parent = 0;

	if(selectedList.count() > 1)
	{
		parent = selectedList.at(0);
		for(int i = 1; i < selectedList.count(); ++i)
		{
			if(indexFromItem(selectedList.at(i)).row() < indexFromItem(parent).row())
			{
				parent = selectedList.at(i);
			}
		}
	}
	else if(selectedList.count() == 1)
	{
		parent = selectedList.at(0);
	}

	return parent;
}




void PlayerControlEventListWidget::BackDrop(QList<QTreeWidgetItem*> &_selectedList, QTreeWidgetItem* _parent, int _indicator)
{
		
	int targetIndex;
		
	if(_parent->parent())
	{
		targetIndex = _parent->parent()->indexOfChild(_parent);
	}
	else
	{
		targetIndex = indexOfTopLevelItem(_parent);
	}

	if(_indicator == QAbstractItemView::BelowItem)
	{
		targetIndex++;
	}
	else if(_indicator == QAbstractItemView::AboveItem)
	{
		targetIndex--;
	}


	int offset = 0;
	while(!_selectedList.empty())
	{
		QTreeWidgetItem* it = GetTopItem(_selectedList);
				
		if(it && it->parent() == _parent)
		{
			int index = _parent->indexOfChild(it);
			
			_parent->takeChild(index);
				
			if(_parent->parent() == 0)
			{
				insertTopLevelItem(targetIndex + offset, it);
			}
			else
			{
				_parent->parent()->insertChild(targetIndex + offset, it);
			}	

			if(_indicator == QAbstractItemView::BelowItem)
			{
				++offset;
			}
			else
			{}
		}
			
		_selectedList.removeAt(_selectedList.indexOf(it));
	}
}


void PlayerControlEventListWidget::dropEvent(QDropEvent * _event)
{ 


	int dropIndicatorPos = dropIndicatorPosition();
	QList<QTreeWidgetItem*> selectedList;

	if(_event->dropAction() != Qt::IgnoreAction)
	{
		selectedList = selectedItems();
		for(int i = selectedList.size() - 1; i >= 0; --i)
		{
			selectedList.at(i)->setSelected(true);
		}

	}

	QTreeWidget::dropEvent(_event);
	
	if(_event->dropAction() != Qt::IgnoreAction)
	{
		if(!selectedList.empty())
		{
			QTreeWidgetItem* parent = GetTopItem(selectedList);
				
			if(parent)
			{
				if(parent->parent())
				{
					BackDrop(selectedList, parent->parent(), dropIndicatorPos);
				}
			}
		}
	}
}

 


void PlayerControlEventListWidget::SetDefaultEventValid(ItemInputEvent* _it)
{
	if(_it)
	{
		QString status = "<li><b>Status:</b></li>";
		QString property1Valid = "<li>Device: <font color = \"#00b200\"><b>not defined</b></font></li>";
		QString property2Valid = "<li>Type: <font color = \"#ff0000\"><b>not defined</b></font></li>";
		QString property3Valid = "<li>Key: <font color = \"#ff0000\"><b>not defined</b></font></li>";

		_it->SetPropertyValid(ItemInputEvent::DEVICE_PROPERTY, false);
		_it->SetPropertyValid(ItemInputEvent::TYPE_PROPERTY, false);
		_it->SetPropertyValid(ItemInputEvent::KEY_PROPERTY, false);
		_it->toolTipList.replace(ItemInputEvent::ICON_PROPERTY, status + property1Valid + property2Valid + property3Valid); 
	}
}


void PlayerControlEventListWidget::UpdateEventValid(ItemInputEvent* _it)
{ 
	if(_it)
	{
		QString status = "<li><b>Status:</b></li>";
		QString property1Valid;
		QString property2Valid;
		QString property3Valid;
		if(_it->text(ItemInputEvent::DEVICE_PROPERTY).length() > 0)
		{
			_it->SetPropertyValid(ItemInputEvent::DEVICE_PROPERTY, true);
			property1Valid = "<li>Device: <font color = \"#00b200\"><b>defined</b></font></li>";
		}
		else 
		{ 
			_it->SetPropertyValid(ItemInputEvent::DEVICE_PROPERTY, false);
			property1Valid = "<li>Device: <font color = \"#ff0000\"><b>not defined</b></font></li>";
		}

		if(_it->text(ItemInputEvent::TYPE_PROPERTY).length() > 0)
		{
			_it->SetPropertyValid(ItemInputEvent::TYPE_PROPERTY, true);
			property2Valid = "<li>Type: <font color = \"#00b200\"><b>defined</b></font></li>";
		}
		else 
		{ 
			_it->SetPropertyValid(ItemInputEvent::TYPE_PROPERTY, false);
			property2Valid = "<li>Type: <font color = \"#ff0000\"><b>not defined</b></font></li>";
		}

		if(_it->text(ItemInputEvent::KEY_PROPERTY).length() > 0)
		{
			_it->SetPropertyValid(ItemInputEvent::KEY_PROPERTY, true);
			property3Valid = "<li>Key: <font color = \"#00b200\"><b>defined</b></font></li>";
		}
		else 
		{ 
			_it->SetPropertyValid(ItemInputEvent::KEY_PROPERTY, false);
			property3Valid = "<li>Key: <font color = \"#ff0000\"><b>not defined</b></font></li>";
		}
		_it->toolTipList.replace(ItemInputEvent::ICON_PROPERTY, status + property1Valid + property2Valid + property3Valid); 
	} 
}


void PlayerControlEventListWidget::drawBranches(QPainter* _painter, const QRect& _rect, const QModelIndex& _index)const
{}


void PlayerControlEventListWidget::UpdateRow(QTreeWidgetItem* _it, int _column)
{
	if(_column == 0) { return; }

	ItemInputEvent* item = static_cast<ItemInputEvent*>(_it);

	if(item->text(ItemInputEvent::DEVICE_PROPERTY) == "KEYBOARD") 
	{
		if(_column == ItemInputEvent::DEVICE_PROPERTY)
		{
			item->setIcon(ItemInputEvent::ICON_PROPERTY, keyboardIcon);

			item->SetDefaultState();

			item->setText(ItemInputEvent::TYPE_PROPERTY, "");
			item->setText(ItemInputEvent::KEY_PROPERTY, "");
		}
		else if(_column == ItemInputEvent::TYPE_PROPERTY)
		{
			item->setText(ItemInputEvent::KEY_PROPERTY, ""); 
		}
	}
	else if(item->text(ItemInputEvent::DEVICE_PROPERTY) == "MOUSE") 
	{
		if(_column == ItemInputEvent::DEVICE_PROPERTY)
		{
			item->setIcon(ItemInputEvent::ICON_PROPERTY, mouseIcon);

			item->SetDefaultState();

			item->setText(ItemInputEvent::TYPE_PROPERTY, "");
			item->setText(ItemInputEvent::KEY_PROPERTY, "");
		}
		else if(_column == ItemInputEvent::TYPE_PROPERTY)
		{
			if(_it->text(ItemInputEvent::TYPE_PROPERTY) == "MOVE" ||
		       _it->text(ItemInputEvent::TYPE_PROPERTY) == "WHEEL")
			{
				item->setText(ItemInputEvent::KEY_PROPERTY, "NONE"); 
				item->SetPropertyValid(ItemInputEvent::KEY_PROPERTY, true);
			}
			else
			{
				item->SetPropertyValid(ItemInputEvent::KEY_PROPERTY, false);
				item->setText(ItemInputEvent::KEY_PROPERTY, ""); 
			}
		}
	}
	UpdateEventValid(item);
}  
  

void PlayerControlEventListWidget::ItemClicked(QTreeWidgetItem* _it, int _column)
{
	if(_column == 0) { return; }

	ItemInputEvent* item = static_cast<ItemInputEvent*>(_it);

	if(item->text(ItemInputEvent::DEVICE_PROPERTY) == "KEYBOARD") 
	{
		if(_column == ItemInputEvent::TYPE_PROPERTY)
		{
			delegate->SetComboValuesForColumn(ItemInputEvent::TYPE_PROPERTY, keyboardEventTypeList);
		}
	}
	else if(item->text(ItemInputEvent::DEVICE_PROPERTY) == "MOUSE") 
	{
		if(_column == ItemInputEvent::TYPE_PROPERTY)
		{
			delegate->SetComboValuesForColumn(ItemInputEvent::TYPE_PROPERTY, mouseEventTypeList);
		}

		if(item->text(ItemInputEvent::TYPE_PROPERTY) == "KEY_DOWN" ||
		   item->text(ItemInputEvent::TYPE_PROPERTY) == "KEY_UP" ||
		   item->text(ItemInputEvent::TYPE_PROPERTY) == "DOUBLE_CLICK")
		{
			delegate->SetComboValuesForColumn(ItemInputEvent::KEY_PROPERTY, keyMouseEventTypeList);
		}
	}
}  
 

void PlayerControlEventListWidget::mouseDoubleClickEvent(QMouseEvent* _event)
{
	if(_event->button() == Qt::LeftButton)
	{
		QTreeWidgetItem* it = currentItem();

		if(it)
		{
			int column = currentColumn();

			UpdateItem(it->text(ItemInputEvent::DEVICE_PROPERTY), it, column);
				
			editItem(it, column);
		}
	}
}


void PlayerControlEventListWidget::UpdateItem(QString _eventType, QTreeWidgetItem* _it, int _column)
{
	if(_it)
	{
		if(_column == ItemInputEvent::TYPE_PROPERTY || _column == ItemInputEvent::KEY_PROPERTY)
		{
			delegate->SetColumnDelegate(_column, GetEditorTypeForColumn(_eventType, static_cast<ItemInputEvent*>(_it), _column));
		}
	}
}


int PlayerControlEventListWidget::GetEditorTypeForColumn(QString _eventType, ItemInputEvent* _it, int _column)
{
	if(_eventType == "KEYBOARD")
	{
		switch(_column)
		{
			case ItemInputEvent::TYPE_PROPERTY:
			{
				return UniversalDelegate::COMBO_BOX_EDITOR;
			}

			case ItemInputEvent::KEY_PROPERTY:
			{ 
				return UniversalDelegate::VIRUAL_KEY_EDITOR;
			}
		}
	} 
	else if(_eventType == "MOUSE")
	{
		switch(_column)
		{
			case ItemInputEvent::TYPE_PROPERTY:
			{
				return UniversalDelegate::COMBO_BOX_EDITOR;
			}

			case ItemInputEvent::KEY_PROPERTY:
			{
				if(_it->text(ItemInputEvent::TYPE_PROPERTY) == "MOVE" ||
		           _it->text(ItemInputEvent::TYPE_PROPERTY) == "WHEEL")
				{
					UniversalDelegate::NO_EDIT;
				}
				
				return UniversalDelegate::COMBO_BOX_EDITOR;
			}
		}
	} 
	return UniversalDelegate::NO_EDIT;
}


void PlayerControlEventListWidget::keyPressEvent(QKeyEvent* _event)
{
	if(_event->key() == Qt::Key_R)
	{
		RemoveSelectedItems();
	}
	if(_event->key() == Qt::Key_A)
	{
		NewEvent();
	}
	else if(_event->key() == Qt::Key_Return)
	{
		QTreeWidgetItem* it = currentItem();

		if(it)
		{
			int column = currentColumn();

			UpdateItem(it->text(ItemInputEvent::DEVICE_PROPERTY), it, column);
			
			editItem(it, column);
		}
	}
}


void PlayerControlEventListWidget::RemoveSelectedItems(void) 
{
	QList<QTreeWidgetItem*> list = selectedItems();

	while(!list.isEmpty())
	{
		QTreeWidgetItem* it = list.at(0);

		if(it)
		{
			list.removeAt(0);
			delete it;
		}
	}
	emit UpdateEvents();
}




void PlayerControlEventListWidget::GetEventList(QList<QTreeWidgetItem*>& _list)
{
	GetItemList(invisibleRootItem(), _list);
}


void PlayerControlEventListWidget::GetItemList(QTreeWidgetItem* _it, QList<QTreeWidgetItem*>& _list)
{
	if(_it)
	{
		int child = 0;

		if(_it != invisibleRootItem())
		{ 
			_list.push_back(_it);
		}
		
		while(_it->child(child))
		{
			GetItemList(_it->child(child), _list);
			child++;
		}
	}
}


void PlayerControlEventListWidget::NewEvent(void)
{
	ItemInputEvent* it = new ItemInputEvent(invisibleRootItem());
	
	SetDefaultEventValid(it);
 
	it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
	it->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);
	it->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
	it->setTextAlignment(2, Qt::AlignHCenter | Qt::AlignVCenter);
	it->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);
	QFont font("Verdana", 8, QFont::Normal, false);
	it->setFont(1, font);
	it->setFont(2, font);
	it->setFont(3, font);
	
	emit UpdateEvents();
}



QString PlayerControlEventListWidget::GetKeyboardEventType(int32 _eventType)
{
	if(_eventType == Keyboard::KEY_DOWN)
	{
		return "KEY_DOWN";
	}
	{
		return "KEY_UP";
	}
}
 
QString PlayerControlEventListWidget::GetMouseEventType(int32 _eventType)
{
	if(_eventType == Mouse::KEY_DOWN)
	{
		return "KEY_DOWN";
	}
	else if(_eventType == Mouse::KEY_UP)
	{
		return "KEY_UP";
	}
	else if(_eventType == Mouse::DBL_CLICK)
	{
		return "DOUBLE_CLICK";
	}
	else if(_eventType == Mouse::MOVE)
	{
		return "MOVE";
	}
	else if(_eventType == Mouse::WHEEL)
	{
		return "WHEEL";
	}
	return "";
}


QString PlayerControlEventListWidget::GetMouseEventKey(int32 _eventKey)
{
	if(_eventKey == Mouse::LEFT_BUTTON)
	{
		return "LEFT_BUTTON";
	}
	else if(_eventKey == Mouse::RIGHT_BUTTON)
	{
		return "MIDDLE_BUTTON";
	}
	{
		return "RIGHT_BUTTON";
	}
}

void PlayerControlEventListWidget::NewEvent(InputEvent* _event)
{
	if(_event) 
	{
		ItemInputEvent* it = new ItemInputEvent(invisibleRootItem());
		
		SetDefaultEventValid(it);
	 
		it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
		it->setTextAlignment(0, Qt::AlignHCenter);
		it->setTextAlignment(1, Qt::AlignHCenter);
		it->setTextAlignment(2, Qt::AlignHCenter);
		it->setTextAlignment(3, Qt::AlignHCenter);
		QFont font("Verdana", 10, QFont::Normal, false);
		it->setFont(1, font);
		it->setFont(2, font);
		it->setFont(3, font);
		
		if(_event->GetDeviceType() == Keyboard::KEYBOARD_DEVICE)
		{
			it->setIcon(ItemInputEvent::ICON_PROPERTY, keyboardIcon);

			it->setText(ItemInputEvent::DEVICE_PROPERTY, "KEYBOARD");
			it->setText(ItemInputEvent::TYPE_PROPERTY, GetKeyboardEventType(_event->GetEventType()));
			it->setText(ItemInputEvent::KEY_PROPERTY, Keyboard::_GetKeyName(dynamic_cast<Keyboard::Event*>(_event)->GetKey()).c_str());
		}
		else if(_event->GetDeviceType() == Mouse::MOUSE_DEVICE)
		{
			it->setIcon(ItemInputEvent::ICON_PROPERTY, mouseIcon);

			it->setText(ItemInputEvent::DEVICE_PROPERTY, "MOUSE");
			it->setText(ItemInputEvent::TYPE_PROPERTY, GetMouseEventType(_event->GetEventType()));
			
			if(_event->GetEventType() == Mouse::KEY_DOWN |
			   _event->GetEventType() == Mouse::KEY_UP |
			   _event->GetEventType() == Mouse::DBL_CLICK)
			{
				it->setText(ItemInputEvent::KEY_PROPERTY, GetMouseEventKey(dynamic_cast<Mouse::Event*>(_event)->GetKey()));
			}
		}

		emit UpdateEvents();
	}
}

