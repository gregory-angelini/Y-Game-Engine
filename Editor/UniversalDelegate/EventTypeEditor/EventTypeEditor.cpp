#include "EventTypeEditor.h"
#include <Editor/Editor/Editor.h>



EventTypeEditor::EventTypeEditor(QWidget* _widget): QComboBox(_widget), selectedCategory(-1), selectedProperty(-1), selectedEventType(-1)
{
	model = new QStandardItemModel(MAX_ROWS, 1, this);
	QStringList columnLabels;
	columnLabels.push_back("CATEGORY");
	columnLabels.push_back("PROPERTY");
	columnLabels.push_back("EVENT_TYPE");
	model->setHorizontalHeaderLabels(columnLabels);
	QStringList rowLabels;
	for(int i = 0; i < MAX_ROWS; i++)
	{
		rowLabels.push_back("");
	}
	model->setVerticalHeaderLabels(rowLabels);

	model->verticalHeaderItem(BASIC)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/basic.png"));
	
	model->verticalHeaderItem(SHAPE)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/shape.png"));
	model->verticalHeaderItem(TRANSFORMATION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/transformation.png"));

	model->verticalHeaderItem(MATERIAL)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/material.png"));
	model->verticalHeaderItem(VIEW)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/view.png"));

	model->verticalHeaderItem(INTERPOLATION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/interpolation.png"));
	model->verticalHeaderItem(PLAYBACK)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/playback.png"));

	model->verticalHeaderItem(RANDOMIZATION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/randomization.png"));
	model->verticalHeaderItem(COMBINATION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/combination.png"));

	model->verticalHeaderItem(INPUT)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/input.png"));
	model->verticalHeaderItem(TIME)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/time.png"));
	model->verticalHeaderItem(TEXT)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/text.png"));
	model->verticalHeaderItem(FILE)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/file.png"));

	model->verticalHeaderItem(CONTAINER)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/container.png"));
	
	model->verticalHeaderItem(COLLISION)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/collision.png"));
	
	model->verticalHeaderItem(SCRIPT)->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "EventCreator/script.png"));
	categoryLabels.push_back("BASIC");

	categoryLabels.push_back("SHAPE");
	categoryLabels.push_back("TRANSFORMATION");

	categoryLabels.push_back("MATERIAL");
	categoryLabels.push_back("VIEW");

	categoryLabels.push_back("INTERPOLATION");
	categoryLabels.push_back("PLAYBACK");

	categoryLabels.push_back("RANDOMIZATION");
	categoryLabels.push_back("COMBINATION");

	categoryLabels.push_back("INPUT");
	categoryLabels.push_back("TIME");
	categoryLabels.push_back("TEXT");
	categoryLabels.push_back("FILE");

	categoryLabels.push_back("CONTAINER");

	categoryLabels.push_back("COLLISION");

	categoryLabels.push_back("SCRIPT");
	for(int i = 0; i < categoryLabels.size(); i++)
	{
		AddCategory(i, categoryLabels.at(i));
	}
	propertyTree[BASIC].AddToTail("ABSTRACT_OBJECT");
	propertyTree[TIME].AddToTail("TIMER");
	propertyTree[INTERPOLATION].AddToTail("FLOAT_LERP");
	propertyTree[CONTAINER].AddToTail("VARIABLE");
	propertyTree[CONTAINER].AddToTail("LAYER");
	propertyTree[CONTAINER].AddToTail("LOGIC_SCENE");
	propertyTree[CONTAINER].AddToTail("GRAPHICS_SCENE");
	propertyTree[CONTAINER].AddToTail("SCENE");
	propertyTree[VIEW].AddToTail("CAMERA");
	propertyTree[VIEW].AddToTail("GRAPHICS_CONTAINER");
	propertyTree[TRANSFORMATION].AddToTail("ABSTRACT_ANIMATION");
	propertyTree[TRANSFORMATION].AddToTail("TEXTURED_ANIMATION");
	propertyTree[TRANSFORMATION].AddToTail("LERPING_OBJECT");
	propertyTree[MATERIAL].AddToTail("TEXTURED_ANIMATION");
	propertyTree[MATERIAL].AddToTail("TEXTURING_SURFACE");
	propertyTree[INPUT].AddToTail("PLAYER_CONTROL");
	propertyTree[SCRIPT].AddToTail("TRIGGER");
	propertyTree[SHAPE].AddToTail("POLYGONAL_SURFACE");
	propertyTree[COLLISION].AddToTail("POLYGONAL_SURFACE");
	propertyTree[PLAYBACK].AddToTail("PLAYING_OBJECT");
	eventTree[BASIC][ABSTRACT_OBJECT].AddToTail("CHANGE_NAME");
	eventTree[BASIC][ABSTRACT_OBJECT].AddToTail("SEND_MESSAGE");
	eventTree[BASIC][ABSTRACT_OBJECT].AddToTail("CREATE_OBJECT");
	eventTree[BASIC][ABSTRACT_OBJECT].AddToTail("DESTROY_OBJECT");
	eventTree[BASIC][ABSTRACT_OBJECT].AddToTail("ENABLE_PROPERTY");
	eventTree[TIME][TIME_TIMER].AddToTail("TIMER_FINISH");
	eventTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("FLOAT_LERP_FINISH");
	eventTree[INTERPOLATION][INTERPOLATION_FLOAT_LERP].AddToTail("FLOAT_LERP_START");
	eventTree[CONTAINER][CONTAINER_VARIABLE].AddToTail("CHANGE_VALUE");

	eventTree[CONTAINER][CONTAINER_LAYER].AddToTail("ADD_TO_LAYER");
	eventTree[CONTAINER][CONTAINER_LAYER].AddToTail("REMOVE_FROM_LAYER");

	eventTree[CONTAINER][CONTAINER_LOGIC_SCENE].AddToTail("ADD_TO_UPDATE_LIST");
	eventTree[CONTAINER][CONTAINER_LOGIC_SCENE].AddToTail("REMOVE_FROM_UPDATE_LIST");

	eventTree[CONTAINER][CONTAINER_GRAPHICS_SCENE].AddToTail("ADD_TO_RENDER_LIST");
	eventTree[CONTAINER][CONTAINER_GRAPHICS_SCENE].AddToTail("REMOVE_FROM_RENDER_LIST");

	eventTree[CONTAINER][CONTAINER_SCENE].AddToTail("PICK_SCENE");
	eventTree[VIEW][VIEW_CAMERA].AddToTail("PICK_CAMERA");

	eventTree[VIEW][VIEW_GRAPHICS_CONTAINER].AddToTail("CHANGE_CAMERA");
	eventTree[TRANSFORMATION][TRANSFORMATION_ABSTRACT_ANIMATION].AddToTail("CHANGE_ANIMATION_FRAME");

	eventTree[TRANSFORMATION][TRANSFORMATION_TEXTURED_ANIMATION].AddToTail("ENABLE_POSITION_OFFSET");
	
	eventTree[TRANSFORMATION][TRANSFORMATION_LERPING_OBJECT].AddToTail("CHANGE_FLOAT_LERP");
	eventTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("CHANGE_FRONT_TEXTURE");
	eventTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("CHANGE_BACK_TEXTURE");

	eventTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("CHANGE_FRONT_TEXTURED_ANIMATION");
	eventTree[MATERIAL][MATERIAL_TEXTURING_SURFACE].AddToTail("CHANGE_BACK_TEXTURED_ANIMATION");

	eventTree[MATERIAL][MATERIAL_TEXTURED_ANIMATION].AddToTail("CHANGE_ATLAS");
	eventTree[INPUT][INPUT_PLAYER_CONTROL].AddToTail("PLAYER_CONTROL_FINISH");
	eventTree[SCRIPT][SCRIPT_TRIGGER].AddToTail("TRIGGER_FINISH");
	eventTree[SHAPE][SHAPE_POLYGONAL_SURFACE].AddToTail("CHANGE_MESH");
	eventTree[COLLISION][COLLISION_POLYGONAL_SURFACE].AddToTail("COLLISION_BETWEEN_OBJECTS");
	eventTree[COLLISION][COLLISION_POLYGONAL_SURFACE].AddToTail("CHANGE_HITBOX");
	eventTree[PLAYBACK][PLAYBACK_PLAYING_OBJECT].AddToTail("CHANGE_PLAYING_STATE");
	eventTree[PLAYBACK][PLAYBACK_PLAYING_OBJECT].AddToTail("CHANGE_PLAYING_MODE");
	for(int i = 0; i < MAX_COLUMNS; i++)
	{
		for(int j = 0; j < MAX_ROWS; j++)
		{
			AddItem(j, i);
		}
	}

	view = new QTableView(this);
	view->setSelectionBehavior(QAbstractItemView::SelectItems);
	view->setSelectionMode(QAbstractItemView::SingleSelection);
	
	view->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");
	view->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: black; color: white; }");


	view->setFixedSize(MAX_COLUMNS * 210 + 100 - (20 * 5)+157+45, (MAX_ROWS) * 32 - 11);

	setView(view);
	setModel(model);

	view->setColumnWidth(CATEGORY, 255);
	view->setColumnWidth(PROPERTY, 255);
	view->setColumnWidth(EVENT_TYPE, 300);
	view->verticalHeader()->setResizeMode(QHeaderView::Fixed);


	connect(view, SIGNAL(pressed(QModelIndex)), this, SLOT(ItemPressed(QModelIndex)));
	connect(view, SIGNAL(entered(QModelIndex)), this, SLOT(ItemEntered(QModelIndex)));
}


int EventTypeEditor::GetSelectedCategory(void)const
{
	return selectedCategory;
}


int EventTypeEditor::GetSelectedProperty(void)const
{
	return selectedProperty;
}


int EventTypeEditor::GetSelectedEventType(void)const
{
	return selectedEventType;
}


bool EventTypeEditor::SelectCategory(QString _eventType)
{
	CategorySelectionReset();
	for(int i = 0; i < MAX_ROWS; i++)
	{
		for(int j = 0; j < PROPERTIES_IN_CATEGORY; j++)
		{
			if(eventTree[i][j].Find(_eventType.toStdString()))
			{
				selectedCategory = i;
			
				UpdateCategorySelection();
				return true;
			}
		}
	}
	return false;
}



bool EventTypeEditor::SelectProperty(QString _eventType)
{
	if(SelectCategory(_eventType))
	{
		PropertySelectionReset();
		for(int j = 0; j < PROPERTIES_IN_CATEGORY; j++)
		{
			if(eventTree[selectedCategory][j].Find(_eventType.toStdString()))
			{
				selectedProperty = j;
				
				UpdatePropertySelection();
				return true;
			}
		}
	}
	return false;
}



void EventTypeEditor::ItemPressed(QModelIndex _index)
{
	setModelColumn(_index.column());
}


void EventTypeEditor::CategorySelectionReset(void)
{
	selectedCategory = -1;
	selectedProperty = -1;
	selectedEventType = -1;

	for(int i = 0; i < MAX_ROWS; i++)
	{
		QStandardItem* it = model->item(i, CATEGORY);

		if(it)
		{
			QFont font("Verdana", 9, QFont::Normal, false);
			it->setFont(font);

			it->setForeground(QColor(0,0,0));
			it->setBackground(QColor(255,255,255));
		}
		for(int i = 0; i < MAX_ROWS; i++)
		{
			QStandardItem* it = model->item(i, PROPERTY);
			if(it) { it->setText(""); }
		}
	}
	PropertySelectionReset();
}



void EventTypeEditor::PropertySelectionReset(void)
{
	selectedProperty = -1;
	selectedEventType = -1;

	for(int i = 0; i < MAX_ROWS; i++)
	{
		QStandardItem* it = model->item(i, PROPERTY);

		if(it)
		{
			QFont font("Verdana", 9, QFont::Normal, false);
			it->setFont(font);

			it->setForeground(QColor(0,0,0));
			it->setBackground(QColor(255,255,255));
		}
		for(int i = 0; i < MAX_ROWS; i++)
		{
			QStandardItem* it = model->item(i, EVENT_TYPE);
			if(it) { it->setText(""); }
		}
	}    
}




void EventTypeEditor::EventTypeSelectionReset(void)
{
	selectedEventType = -1;

	for(int i = 0; i < MAX_ROWS; i++)
	{
		QStandardItem* it = model->item(i, EVENT_TYPE);

		if(it)
		{
			QFont font("Verdana", 9, QFont::Normal, false);
			it->setFont(font);

			it->setForeground(QColor(0,0,0));
			it->setBackground(QColor(255,255,255));
		}
	}
}



bool EventTypeEditor::IsEvent(QString _type)
{
	for(int x = 0; x < MAX_ROWS; x++)
	{
		for(int y = 0; y < PROPERTIES_IN_CATEGORY; y++)
		{
			for(int i = 0; i < eventTree[x][y].GetSize(); i++)
			{
				if(eventTree[x][y].Get(i)->key == _type.toStdString())
				{
					return true;
				}
			}
		}
	}
	return false;
}



void EventTypeEditor::UpdatePropertySelection(void)
{
	if(selectedProperty != -1)
	{
		QStandardItem* it = model->item(selectedProperty, PROPERTY);
		
		if(it)
		{
			QFont font("Verdana", 10, QFont::Bold, false);
			it->setFont(font);

			it->setForeground(QColor(255,255,255));
			it->setBackground(QColor(51,94,168));
			for(int i = 0; i < eventTree[selectedCategory][selectedProperty].GetSize(); i++)
			{
				ChangeItem(i, EVENT_TYPE, eventTree[selectedCategory][selectedProperty].Get(i)->key.c_str());
			}
		}
	}
}



void EventTypeEditor::UpdateEventTypeSelection(void)
{
	if(selectedEventType != -1)
	{
		QStandardItem* it = model->item(selectedEventType, EVENT_TYPE);
		
		if(it)
		{
			QFont font("Verdana", 10, QFont::Bold, false);
			it->setFont(font);

			it->setForeground(QColor(255,255,255));
			it->setBackground(QColor(51,94,168));
		}
	}
}


void EventTypeEditor::UpdateCategorySelection(void)
{
	if(selectedCategory != -1)
	{
		QStandardItem* it = model->item(selectedCategory, CATEGORY);
		
		if(it)
		{
			QFont font("Verdana", 10, QFont::Bold, false);
			it->setFont(font);

			it->setForeground(QColor(255,255,255));
			it->setBackground(QColor(51,94,168));
			for(int i = 0; i < propertyTree[selectedCategory].GetSize(); i++)
			{
				ChangeItem(i, PROPERTY, propertyTree[selectedCategory].Get(i)->key.c_str());
			}
		}
	}
}



void EventTypeEditor::ItemEntered(QModelIndex _index)
{
	if(_index.column() == CATEGORY)
	{
		CategorySelectionReset();

		selectedCategory = _index.row();

		UpdateCategorySelection();
	}
	else if(_index.column() == PROPERTY)
	{ 
		if(selectedCategory != -1)
		{ 
			PropertySelectionReset();

			selectedProperty = _index.row();
			
			UpdatePropertySelection();
		}
	}
	else if(_index.column() == EVENT_TYPE)
	{
		if(selectedProperty != -1)
		{
			EventTypeSelectionReset();

			selectedEventType = _index.row();

			UpdateEventTypeSelection();
		}
	}
}


void EventTypeEditor::ChangeItem(int _row, int _column, QString _name)
{
	QStandardItem* it = model->item(_row, _column);

	if(it)
	{	
		it->setText(_name);
	}
}


void EventTypeEditor::AddItem(int _row, int _column)
{
	if(model->item(_row, _column) == 0)
	{
		QStandardItem* it = new QStandardItem("");
		it->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		QFont font("Verdana", 9, QFont::Normal, false);
		it->setFont(font);

		model->setItem(_row, _column, it);
	}
}


void EventTypeEditor::AddCategory(int _row, QString _name)
{
	QStandardItem* it = new QStandardItem(_name);
	it->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	QFont font("Verdana", 9, QFont::Normal, false);
	it->setFont(font);

	model->setItem(_row, CATEGORY, it);
}