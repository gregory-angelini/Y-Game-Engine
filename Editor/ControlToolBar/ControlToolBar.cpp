#include "ControlToolBar.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/TriggerCreator/ActionCreator/ActionCreator.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Editor/NameCreator/NameCreator.h>




ControlToolBar::ControlToolBar(QWidget* _parent): QToolBar(_parent), currentTool(-1), recordEnable(false), currentTimeSpeed(1.0f), timeShowEnable(false)
{
	setIconSize(QSize(32, 32));

	CreatePlayTool();
	CreatePauseTool();
	CreateStopTool();
	CreateNextTool();
	CreateRecordTool();

	addSeparator();

	CreateTimeTool();
	CreateTimeSpeedTool();
	ToPause(); 

	setWindowTitle("Control tools");
}



ControlToolBar::~ControlToolBar(void)
{}


bool ControlToolBar::IsRecordEnabled(void)const
{
	return recordEnable;
}



void ControlToolBar::CreatePlayTool(void)
{
	buttonPlayEnabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/play_enabled.png");
	buttonPlayDisabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/play_disabled.png");
	playAction = new QAction(buttonPlayDisabled, "Play", this);
	connect(playAction, SIGNAL(triggered()), this, SLOT(ToPlay()));
	
	playToolButton = new QToolButton(this);
	playToolButton->setDefaultAction(playAction);
	addWidget(playToolButton);
}


void ControlToolBar::CreateStopTool(void)
{
	buttonStopEnabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/stop_enabled.png");
	buttonStopDisabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/stop_disabled.png");
	stopAction = new QAction(buttonStopDisabled, "Stop", this);
	connect(stopAction, SIGNAL(triggered()), this, SLOT(ToStop()));
	
	stopToolButton = new QToolButton(this);
	stopToolButton->setDefaultAction(stopAction);
	addWidget(stopToolButton);
}



void ControlToolBar::CreatePauseTool(void)
{
	buttonPauseEnabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/pause_enabled.png");
	buttonPauseDisabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/pause_disabled.png");
	pauseAction = new QAction(buttonPauseDisabled, "Pause", this);
	connect(pauseAction, SIGNAL(triggered()), this, SLOT(ToPause()));
	
	pauseToolButton = new QToolButton(this);
	pauseToolButton->setDefaultAction(pauseAction);
	addWidget(pauseToolButton); 
}




void ControlToolBar::CreateNextTool(void)
{
	buttonNextEnabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/next_enabled.png");
	buttonNextDisabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/next_disabled.png");
	nextFrameAction = new QAction(buttonNextDisabled, "Next frame", this);
	connect(nextFrameAction, SIGNAL(triggered()), this, SLOT(ToNextFrame()));
	
	nextFrameToolButton = new QToolButton(this);
	nextFrameToolButton->setDefaultAction(nextFrameAction);
	addWidget(nextFrameToolButton); 
}



void ControlToolBar::CreateRecordTool(void)
{
	buttonRecordEnabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/record_enabled.png");
	buttonRecordDisabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/record_disabled.png");
	recordAction = new QAction(buttonRecordDisabled, "Record", this);
	connect(recordAction, SIGNAL(triggered()), this, SLOT(ToRecord()));
	
	recordToolButton = new QToolButton(this);
	recordToolButton->setDefaultAction(recordAction);
	addWidget(recordToolButton); 
}



void ControlToolBar::CreateTimeSpeedTool(void)
{
	buttonTimeX1 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x1.png");
	buttonTimeX2 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x2.png");
	buttonTimeX4 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x4.png");
	buttonTimeX8 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x8.png");
	buttonTimeX16 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x16.png");
	buttonTimeX05 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x05.png");
	buttonTimeX025 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x025.png");
	buttonTimeX0125 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x0125.png");
	buttonTimeX00625 = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_x00625.png");
	timeSpeedAction = new QAction(buttonTimeX1, "Time speed", this);
	connect(timeSpeedAction, SIGNAL(triggered()), this, SLOT(NextTimeSpeed()));
	
	timeSpeedToolButton = new QToolButton(this);
	timeSpeedToolButton->setDefaultAction(timeSpeedAction);
	timeSpeedToolButton->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(timeSpeedToolButton, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(UpdateTimeSpeedContextMenu(const QPoint&)));
	addWidget(timeSpeedToolButton);

	CreateTimeSpeedContextMenu();
}


void ControlToolBar::UpdateTimeSpeedContextMenu(const QPoint& _point)
{
	UpdateTimeSpeedContextMenu();

	timeSpeedContextMenu->exec(timeSpeedToolButton->mapToGlobal(_point));
}


void ControlToolBar::UpdateTimeSpeedContextMenu(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX2Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);

	if(currentTimeSpeed == 0.5f)
	{ 
		timeSpeedX05Act->setChecked(true);
	}
	else if(currentTimeSpeed == 0.25f)
	{
		timeSpeedX025Act->setChecked(true);
	}
	else if(currentTimeSpeed == 0.125f)
	{
		timeSpeedX0125Act->setChecked(true);
	}
	else if(currentTimeSpeed == 0.0625f)
	{
		timeSpeedX00625Act->setChecked(true);
	}
	else if(currentTimeSpeed == 1.0f)
	{
		timeSpeedX1Act->setChecked(true);
	}
	else if(currentTimeSpeed == 2.0f)
	{
		timeSpeedX2Act->setChecked(true);
	}
	else if(currentTimeSpeed == 4.0f)
	{
		timeSpeedX4Act->setChecked(true);
	}
	else if(currentTimeSpeed == 8.0f)
	{
		timeSpeedX8Act->setChecked(true);
	}
	else if(currentTimeSpeed == 16.0f)
	{
		timeSpeedX16Act->setChecked(true);
	}
}


void ControlToolBar::TimeSpeedX1IsChecked(void)
{
	timeSpeedX2Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX1);
	currentTimeSpeed = 1.0f;

	emit TimeSpeedToolIsChanged();
}



void ControlToolBar::TimeSpeedX2IsChecked(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX2);
	currentTimeSpeed = 2.0f;

	emit TimeSpeedToolIsChanged();
}



void ControlToolBar::TimeSpeedX4IsChecked(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX2Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX4);
	currentTimeSpeed = 4.0f;

	emit TimeSpeedToolIsChanged();
}


void ControlToolBar::TimeSpeedX8IsChecked(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX2Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX8);
	currentTimeSpeed = 8.0f;

	emit TimeSpeedToolIsChanged();
}



void ControlToolBar::TimeSpeedX16IsChecked(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX2Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX16);
	currentTimeSpeed = 16.0f;

	emit TimeSpeedToolIsChanged();
}



void ControlToolBar::TimeSpeedX05IsChecked(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX2Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX05);
	currentTimeSpeed = 0.5f;

	emit TimeSpeedToolIsChanged();
}



void ControlToolBar::TimeSpeedX025IsChecked(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX2Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX025);
	currentTimeSpeed = 0.25f;

	emit TimeSpeedToolIsChanged();
}


void ControlToolBar::TimeSpeedX0125IsChecked(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX2Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX00625Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX0125);
	currentTimeSpeed = 0.125f;

	emit TimeSpeedToolIsChanged();
}


 
void ControlToolBar::TimeSpeedX00625IsChecked(void)
{
	timeSpeedX1Act->setChecked(false);
	timeSpeedX2Act->setChecked(false);
	timeSpeedX4Act->setChecked(false);
	timeSpeedX8Act->setChecked(false);
	timeSpeedX16Act->setChecked(false);

	timeSpeedX05Act->setChecked(false);
	timeSpeedX025Act->setChecked(false);
	timeSpeedX0125Act->setChecked(false);


	timeSpeedAction->setIcon(buttonTimeX00625);
	currentTimeSpeed = 0.0625f;

	emit TimeSpeedToolIsChanged();
}


void ControlToolBar::CreateTimeSpeedContextMenu(void)
{
	timeSpeedContextMenu = new QMenu(this);
	timeSpeedX00625Act = new QAction("X1/16", timeSpeedContextMenu);
	timeSpeedX00625Act->setCheckable(true);
	connect(timeSpeedX00625Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX00625IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX00625Act);
	timeSpeedX0125Act = new QAction("X1/8", timeSpeedContextMenu);
	timeSpeedX0125Act->setCheckable(true);
	connect(timeSpeedX0125Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX0125IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX0125Act);
	timeSpeedX025Act = new QAction("X1/4", timeSpeedContextMenu);
	timeSpeedX025Act->setCheckable(true);
	connect(timeSpeedX025Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX025IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX025Act);
	timeSpeedX05Act = new QAction("X1/2", timeSpeedContextMenu);
	timeSpeedX05Act->setCheckable(true);
	connect(timeSpeedX05Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX05IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX05Act);

	timeSpeedContextMenu->addSeparator();
	timeSpeedX1Act = new QAction("X1", timeSpeedContextMenu);
	timeSpeedX1Act->setCheckable(true);
	timeSpeedX1Act->setChecked(true); 
	connect(timeSpeedX1Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX1IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX1Act);
	timeSpeedX2Act = new QAction("X2", timeSpeedContextMenu);
	timeSpeedX2Act->setCheckable(true);
	connect(timeSpeedX2Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX2IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX2Act);
	timeSpeedX4Act = new QAction("X4", timeSpeedContextMenu);
	timeSpeedX4Act->setCheckable(true);
	connect(timeSpeedX4Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX4IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX4Act);
	timeSpeedX8Act = new QAction("X8", timeSpeedContextMenu);
	timeSpeedX8Act->setCheckable(true);
	connect(timeSpeedX8Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX8IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX8Act);
	timeSpeedX16Act = new QAction("X16", timeSpeedContextMenu);
	timeSpeedX16Act->setCheckable(true);
	connect(timeSpeedX16Act, SIGNAL(triggered()), this, SLOT(TimeSpeedX16IsChecked()));
	timeSpeedContextMenu->addAction(timeSpeedX16Act);
} 



void ControlToolBar::CreateTimeTool(void)
{
	buttonTimeDisabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_disabled.png");
	buttonTimeEnabled = QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "ControlToolBar/time_enabled.png");
	timeAction = new QAction(buttonTimeDisabled, "Time", this);
	connect(timeAction, SIGNAL(triggered()), this, SLOT(ToTime()));
	
	timeToolButton = new QToolButton(this);
	timeToolButton->setDefaultAction(timeAction);
	addWidget(timeToolButton); 
}




int ControlToolBar::GetCurrentTool(void)
{
	return currentTool;
}



void ControlToolBar::ToPlay(void)
{
	if(currentTool == PLAY)
	{
		return;
	}

	stopAction->setIcon(buttonStopDisabled);
	pauseAction->setIcon(buttonPauseDisabled);
	playAction->setIcon(buttonPlayEnabled);


	currentTool = PLAY;
	emit PickPlayTool();
}


float ControlToolBar::GetCurrentTimeSpeed(void)const
{
	return currentTimeSpeed;
}


void ControlToolBar::NextTimeSpeed(void)
{
	if(currentTimeSpeed == 0.0625f)
	{
		timeSpeedAction->setIcon(buttonTimeX0125);
		currentTimeSpeed = 0.125f;

		emit TimeSpeedToolIsChanged();
	}
	else if(currentTimeSpeed == 0.125f)
	{
		timeSpeedAction->setIcon(buttonTimeX025);
		currentTimeSpeed = 0.25f;

		emit TimeSpeedToolIsChanged();
	}
	else if(currentTimeSpeed == 0.25f)
	{
		timeSpeedAction->setIcon(buttonTimeX05);
		currentTimeSpeed = 0.5f;

		emit TimeSpeedToolIsChanged();
	}
	else if(currentTimeSpeed == 0.5f)
	{
		timeSpeedAction->setIcon(buttonTimeX1);
		currentTimeSpeed = 1.0f;

		emit TimeSpeedToolIsChanged();
	}
	else if(currentTimeSpeed == 1.0f)
	{
		timeSpeedAction->setIcon(buttonTimeX2);
		currentTimeSpeed = 2.0f;

		emit TimeSpeedToolIsChanged();
	}
	else if(currentTimeSpeed == 2.0f)
	{
		timeSpeedAction->setIcon(buttonTimeX4);
		currentTimeSpeed = 4.0f;

		emit TimeSpeedToolIsChanged();
	}
	else if(currentTimeSpeed == 4.0f)
	{
		timeSpeedAction->setIcon(buttonTimeX8);
		currentTimeSpeed = 8.0f;

		emit TimeSpeedToolIsChanged();
	}
	else if(currentTimeSpeed == 8.0f)
	{
		timeSpeedAction->setIcon(buttonTimeX16);
		currentTimeSpeed = 16.0f;

		emit TimeSpeedToolIsChanged();
	}
	else if(currentTimeSpeed == 16.0f)
	{
		timeSpeedAction->setIcon(buttonTimeX00625);
		currentTimeSpeed = 0.0625f;

		emit TimeSpeedToolIsChanged();
	}
}



void ControlToolBar::ToTime(void)
{
	if(timeShowEnable)
	{
		timeAction->setIcon(buttonTimeDisabled);
		timeShowEnable = false;
	}
	else
	{
		timeAction->setIcon(buttonTimeEnabled);
		timeShowEnable = true;
	}
	emit ShowTimeToolIsChanged();
}




bool ControlToolBar::IsTimeShowEnabled(void)const
{
	return timeShowEnable;
}



void ControlToolBar::DisableRecord(void)
{
	if(recordEnable)
	{
		recordAction->setIcon(buttonRecordDisabled);
		recordEnable = false;
		Trigger::_SetEventLogEnable(false);
	}
}


void ControlToolBar::ToRecord(void)
{ 
	if(recordEnable)
	{
		recordAction->setIcon(buttonRecordDisabled);
		recordEnable = false;
		Trigger::_SetEventLogEnable(false);

		emit RecordToolIsDisabled();
	}
	else
	{
		recordAction->setIcon(buttonRecordEnabled);
		recordEnable = true;
		Trigger::_SetEventLogEnable(true);
	} 
}




void ControlToolBar::ToPause(void)
{ 
	if(currentTool == PAUSE)
	{
		return;
	}

	stopAction->setIcon(buttonStopDisabled);
	playAction->setIcon(buttonPlayDisabled);
	pauseAction->setIcon(buttonPauseEnabled);

	currentTool = PAUSE;
	emit PickPauseTool();
}



void ControlToolBar::ToNextFrame(void)
{ 
	if(currentTool == PAUSE)
	{  
		nextFrameAction->setIcon(buttonNextEnabled);

		emit PickNextTool();

		QTimer::singleShot(100, this, SLOT(NextButtonTimeout()));
	}
}


void ControlToolBar::NextButtonTimeout(void)
{
	nextFrameAction->setIcon(buttonNextDisabled);
}


void ControlToolBar::ToStop(void)
{ 
	if(currentTool == STOP)
	{
		return;
	}

	pauseAction->setIcon(buttonPauseDisabled);
	playAction->setIcon(buttonPlayDisabled);
	stopAction->setIcon(buttonStopEnabled);

	currentTool = STOP; 
	emit PickStopTool();
	if(timeShowEnable)
	{
		timeAction->setIcon(buttonTimeDisabled);
		timeShowEnable = false;
		emit ShowTimeToolIsChanged();
	} 
}



