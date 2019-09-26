#include "TextureShapeListWidget.h"
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>




TextureShapeListWidget::TextureShapeListWidget(QWidget* _parent): QTreeWidget(_parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
	

	header()->setMovable(false);
	setColumnCount(5);


	CreateContextMenu();
	boundingVolumeComboList.push_back("QUAD");
	boundingVolumeComboList.push_back("CIRCLE");
	boundingVolumeComboList.push_back("BOX");
	boundingVolumeComboList.push_back("SPHERE");
	headerItem()->setText(ItemShape::NAME_PROPERTY, "Name");
	headerItem()->setText(ItemShape::COLLISION_GROUP_PROPERTY, "Collision group");
	headerItem()->setText(ItemShape::COLLISION_ENABLE_PROPERTY, "Collision enable");
	headerItem()->setText(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, "Collision polygons info");
	headerItem()->setText(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, "Bounding volume type");
	QFont font("Verdana", 9, QFont::Normal, false);

	headerItem()->setFont(ItemShape::NAME_PROPERTY, font);
	headerItem()->setFont(ItemShape::COLLISION_GROUP_PROPERTY, font);
	headerItem()->setFont(ItemShape::COLLISION_ENABLE_PROPERTY, font);
	headerItem()->setFont(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, font);
	headerItem()->setFont(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, font);
	headerItem()->setTextAlignment(ItemShape::NAME_PROPERTY, Qt::AlignHCenter);
	headerItem()->setTextAlignment(ItemShape::COLLISION_GROUP_PROPERTY, Qt::AlignHCenter);
	headerItem()->setTextAlignment(ItemShape::COLLISION_ENABLE_PROPERTY, Qt::AlignHCenter);
	headerItem()->setTextAlignment(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, Qt::AlignHCenter);
	headerItem()->setTextAlignment(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, Qt::AlignHCenter);
	setColumnWidth(ItemShape::NAME_PROPERTY, 150);
	setColumnWidth(ItemShape::COLLISION_GROUP_PROPERTY, 110);
	setColumnWidth(ItemShape::COLLISION_ENABLE_PROPERTY, 110);
	setColumnWidth(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, 150);
	setColumnWidth(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, 150);
	header()->setResizeMode(ItemShape::COLLISION_GROUP_PROPERTY, QHeaderView::Fixed);
	header()->setResizeMode(ItemShape::COLLISION_ENABLE_PROPERTY, QHeaderView::Fixed);
	header()->setResizeMode(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, QHeaderView::Fixed);
	header()->setResizeMode(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, QHeaderView::Fixed);
	
	
	hideColumn(0);
	QPalette p(palette());
	p.setColor(QPalette::Base, QColor(255, 255, 255));
	setPalette(p);

	setStyleSheet("QTreeView::item { height: 24px; }"
		          "QTreeWidget::item:!selected { border: 2px solid gainsboro; "
		                                        "border-left: none; border-top: none; }"
		          "QTreeWidget::item:selected { background-color: rgb(149,199,233);"
                                               "border: 2px solid gainsboro;"
											   "border-left: none; border-top: none; }");
	delegate = new UniversalDelegate(this);
	setItemDelegate(delegate);
	delegate->SetColumnDelegate(ItemShape::NAME_PROPERTY, UniversalDelegate::NAME_EDITOR);
	delegate->SetColumnDelegate(ItemShape::COLLISION_GROUP_PROPERTY, UniversalDelegate::INT_EDITOR);
	delegate->SetColumnDelegate(ItemShape::COLLISION_ENABLE_PROPERTY, UniversalDelegate::BOOL_EDITOR);
	delegate->SetColumnDelegate(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, UniversalDelegate::BOOL_EDITOR);
	delegate->SetColumnDelegate(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, UniversalDelegate::COMBO_BOX_EDITOR);
	
	delegate->SetComboValuesForColumn(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, boundingVolumeComboList);
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(UpdateRow(QTreeWidgetItem*, int)));

	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(ItemClicked(QTreeWidgetItem*, int)));
}


QString TextureShapeListWidget::GetStringBoundingVolumeType(int _shapeType)
{
	switch(_shapeType)
	{
		case Mesh::BOUNDING_QUAD:
		{
			return "QUAD";
		}

		case Mesh::BOUNDING_CIRCLE:
		{
			return "CIRCLE";
		}

		case Mesh::BOUNDING_BOX:
		{
			return "BOX";
		}

		case Mesh::BOUNDING_SPHERE:
		{
			return "SPHERE";
		}
	}
	return "CIRCLE";
}



TextureShapeListWidget::~TextureShapeListWidget(void)
{
	delete delegate;
}


	
void TextureShapeListWidget::CreateContextMenu(void)
{
	contextMenu = new QMenu(this);
	importShapeAct = new QAction("Import", contextMenu);
    connect(importShapeAct, SIGNAL(triggered()), SIGNAL(ImportShape()));
	contextMenu->addAction(importShapeAct);
	saveShapeAsAct = new QAction("Save as...", contextMenu);
    connect(saveShapeAsAct, SIGNAL(triggered()), SIGNAL(SaveShapeAs()));
	contextMenu->addAction(saveShapeAsAct);
	removeShapeAct = new QAction("Remove", contextMenu);
    connect(removeShapeAct, SIGNAL(triggered()), SIGNAL(RemoveShape()));
	contextMenu->addAction(removeShapeAct);
}



void TextureShapeListWidget::UpdateContextMenu(void)
{
	contextMenu->removeAction(saveShapeAsAct);
	contextMenu->removeAction(removeShapeAct);



	QTreeWidgetItem* it = itemAt(cursorPoint);
	
	if(it)
	{
		if(selectedItems().indexOf(it) != -1)
		{
			contextMenu->addAction(saveShapeAsAct);
			contextMenu->addAction(removeShapeAct);
		} 
		else
		{}
	} 
}
  


TextureShapeListWidget::ItemShape* TextureShapeListWidget::GetSelectedShape(void)const
{
	QList<QTreeWidgetItem*> list = selectedItems();
		
	if(!list.isEmpty())
	{
		return dynamic_cast<ItemShape*>(list.at(0));
	}
	return NIL;
}



void TextureShapeListWidget::ResetSelectedShape(void)
{
	QList<QTreeWidgetItem*> list = selectedItems();
		
	if(!list.isEmpty())
	{ 
		dynamic_cast<ItemShape*>(list.at(0))->setSelected(false);
	}
}



int32 TextureShapeListWidget::GetSelectedShapeIndex(void)const
{
	QList<QTreeWidgetItem*> list = selectedItems();
		
	while(!list.isEmpty())
	{
		QTreeWidgetItem* it = list.at(0);

		if(it)
		{
			return indexFromItem(it).row();
		}
	}
	return -1;
}


QString TextureShapeListWidget::GetSelectedShapeName(void)const
{
	QList<QTreeWidgetItem*> list = selectedItems();
		
	while(!list.isEmpty())
	{
		QTreeWidgetItem* it = list.at(0);

		if(it)
		{
			return it->text(ItemShape::NAME_PROPERTY);
		}
	}
	return "";
}



void TextureShapeListWidget::AddShape(QString _name, int _collisionGroup, bool _collisionCheck, bool _collisionPolygonsInfo, int _shapeType)
{
	ItemShape* it = new ItemShape(invisibleRootItem());

	it->setText(ItemShape::NAME_PROPERTY, _name);
	it->setText(ItemShape::COLLISION_GROUP_PROPERTY, QString::number(_collisionGroup));
	it->setText(ItemShape::COLLISION_ENABLE_PROPERTY, _collisionCheck ? "true" : "false");
	it->setText(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, _collisionPolygonsInfo ? "true" : "false");
	it->setText(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, GetStringBoundingVolumeType(_shapeType));

	it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
	it->setTextAlignment(ItemShape::NAME_PROPERTY, Qt::AlignHCenter | Qt::AlignVCenter);
	it->setTextAlignment(ItemShape::COLLISION_GROUP_PROPERTY, Qt::AlignHCenter | Qt::AlignVCenter);
	it->setTextAlignment(ItemShape::COLLISION_ENABLE_PROPERTY, Qt::AlignHCenter | Qt::AlignVCenter);
	it->setTextAlignment(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, Qt::AlignHCenter | Qt::AlignVCenter);
	it->setTextAlignment(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, Qt::AlignHCenter | Qt::AlignVCenter);
	QFont font("Verdana", 8, QFont::Normal, false);

	it->setFont(ItemShape::NAME_PROPERTY, font);
	it->setFont(ItemShape::COLLISION_GROUP_PROPERTY, font);
	it->setFont(ItemShape::COLLISION_ENABLE_PROPERTY, font);
	it->setFont(ItemShape::COLLISION_POLYGONS_INFO_PROPERTY, font);
	it->setFont(ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY, font);


	QList<QTreeWidgetItem*> list = selectedItems();
	for(int i = 0; i < list.size(); i++)
	{
		list.at(i)->setSelected(false);
	}

	it->setSelected(true);
}



void TextureShapeListWidget::mousePressEvent(QMouseEvent* _event)
{ 
	cursorPoint = _event->pos();

	if(_event->button() == Qt::LeftButton)
	{
		QTreeWidget::mousePressEvent(_event);
	}
	else if(_event->button() == Qt::RightButton)
	{
		QTreeWidget::mousePressEvent(_event);
	
		UpdateContextMenu();
		QPoint mousePos(QCursor::pos().x(), QCursor::pos().y()); 
		contextMenu->move(mousePos);
		contextMenu->show();
	}
}


void TextureShapeListWidget::SelectShape(int32 _index)
{
	ItemShape* it = dynamic_cast<ItemShape*>(topLevelItem(_index));

	QList<QTreeWidgetItem*> list = selectedItems();
	for(int i = 0; i < list.size(); i++)
	{
		list.at(i)->setSelected(false);
	}

	if(it)
	{
		it->setSelected(true);
	}
}


void TextureShapeListWidget::RemoveSelectedShape(void) 
{
	QList<QTreeWidgetItem*> list = selectedItems();

	while(!list.isEmpty())
	{
		QTreeWidgetItem* it = list.at(0);

		if(it)
		{
			list.removeAt(0);

			if(it->isSelected())
			{
				it->setSelected(false);
			}

			delete it;
		}
	}
}

