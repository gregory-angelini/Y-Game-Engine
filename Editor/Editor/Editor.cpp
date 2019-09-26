
#include "Editor.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
SimpleList<HANDLE> Editor::processes;

QString Editor::editorName = "Y";
QString Editor::workDir = "editor/";
QString Editor::docsDir = "docs/";
QString Editor::tempDir = "temp/";
QString Editor::resDir = "resources/";

QString Editor::version;
float Editor::b = 12;
float Editor::c = 0.5;
QString Editor::stability = "s";
float Editor::bf = 9;
float Editor::cf = 0.4;
float Editor::df = 0.02; 




Editor::Editor(QWidget* _parent, Qt::WFlags _flags): QMainWindow(_parent, _flags)
{
	intervalTimer = new QTimer(this); 

	version = QString::number((b + c) / (b + c + bf + cf + df)) + stability;


	QString title  = editorName + " Editor " + version;
	setWindowTitle(title);
	

	CreateAssetManager();

	CreateDesignTools();
	
    CreateMainMenu();
 
	CreateViewport();
	
	CreateEngine();
  
	CreateControlToolBar();
	connect(designToolBar, SIGNAL(CameraHomeIsChecked()), this, SLOT(ViewToolHome()));
	connect(designToolBar, SIGNAL(CameraShowSightIsChecked(bool)), this, SLOT(ViewToolShowSightEnableIsChanged(bool)));
	connect(designToolBar, SIGNAL(CameraMovementEnableIsChecked(bool)), this, SLOT(ViewToolMovementEnableIsChanged(bool)));

	connect(designToolBar, SIGNAL(CameraFirstPersonIsChecked()), this, SLOT(ViewToolFirstPersonIsChecked()));
	connect(designToolBar, SIGNAL(CameraObjectViewerIsChecked()), this, SLOT(ViewToolObjectViewerIsChecked()));

	connect(designToolBar, SIGNAL(CameraZoomEnableIsChecked()), this, SLOT(ViewToolZoomEnableIsChanged()));
	connect(designToolBar, SIGNAL(CameraRotationEnableIsChecked()), this, SLOT(ViewToolRotationEnableIsChanged()));

	connect(designToolBar, SIGNAL(CameraPerspectiveProjectionIsChecked()), this, SLOT(ViewToolPerspectiveIsChecked()));
	connect(designToolBar, SIGNAL(CameraOrthographicProjectionIsChecked()), this, SLOT(ViewToolOrthographicIsChecked()));
	connect(designToolBar, SIGNAL(CameraScreenProjectionIsChecked()), this, SLOT(ViewToolScreenIsChecked()));
	
	connect(designToolBar, SIGNAL(CameraSpectatorIsChecked()), this, SLOT(ViewToolSpectatorIsChecked()));
	connect(designToolBar, SIGNAL(CameraOverlandIsChecked()), this, SLOT(ViewToolOverlandIsChecked()));
	connect(designToolBar, SIGNAL(CameraFlightIsChecked()), this, SLOT(ViewToolFlightIsChecked()));
	
	connect(designToolBar, SIGNAL(ScaleHomeIsChecked()), this, SLOT(ScaleToolHome()));
	connect(designToolBar, SIGNAL(MoveHomeIsChecked()), this, SLOT(MoveToolHome()));
	connect(designToolBar, SIGNAL(RotateHomeIsChecked()), this, SLOT(RotateToolHome()));
	connect(controlToolBar, SIGNAL(PickPlayTool()), this, SLOT(PickPlayTool()));
	connect(controlToolBar, SIGNAL(PickPauseTool()), this, SLOT(PickPauseTool()));
	connect(controlToolBar, SIGNAL(PickStopTool()), this, SLOT(PickStopTool()));
	connect(controlToolBar, SIGNAL(PickNextTool()), this, SLOT(PickNextTool()));
    connect(controlToolBar, SIGNAL(RecordToolIsDisabled()), this, SLOT(RecordToolIsDisabled()));

	connect(controlToolBar, SIGNAL(TimeSpeedToolIsChanged()), this, SLOT(TimeSpeedToolIsChanged()));
	connect(controlToolBar, SIGNAL(ShowTimeToolIsChanged()), this, SLOT(ShowTimeToolIsChanged()));
	connect(assetManager->GetObjectTreeWidget(), SIGNAL(SelectObject(GraphicsSurface*)), this, SLOT(SelectObject(GraphicsSurface*)));
	connect(assetManager->GetObjectTreeWidget(), SIGNAL(UnselectObject(GraphicsSurface*)), this, SLOT(UnselectObject(GraphicsSurface*)));
	cursorPosLabel = new QLabel(this);
	cursorPosLabel->move(270+100+40+100+280+150, 70+600+10); 
	cursorPosLabel->resize(130, 20);
	UpdateCursorPos(Vector2());
	cameraPosLabel = new QLabel(this);
	cameraPosLabel->move(270+100+40+100+400+150, 70+600+10); 
	cameraPosLabel->resize(250, 20);
	UpdateCameraPos(Vector3());


	LoadConfigFile();
	
	showMaximized();

	setAttribute(Qt::WA_DeleteOnClose);
}


void Editor::SaveEditorConfigFile(void)
{
	File::_SetBoolKey("editor.config", true, "CAMERA", "Movement enable", engine->GetViewMovementEnable());
	File::_SetBoolKey("editor.config", true, "CAMERA", "Show sight", engine->GetViewShowSightEnable());
}



void Editor::LoadEditorConfigFile(void)
{
	if(!File::_IsExist("editor.config"))
	{
		File::_SetIntKey("editor.config", true, "SPACE", "Pixels per meter", 25);
		File::_SetFloatKey("editor.config", true, "CAMERA", "Move speed", 0.9f);
		File::_SetBoolKey("editor.config", true, "CAMERA", "Movement enable", true);
		File::_SetBoolKey("editor.config", true, "CAMERA", "Show sight", false);
		File::_SetStringKey("editor.config", true, "CAMERA", "Move up", Keyboard::_GetKeyName(Keyboard::UP_ARROW));
		File::_SetStringKey("editor.config", true, "CAMERA", "Move down", Keyboard::_GetKeyName(Keyboard::DOWN_ARROW));
		File::_SetStringKey("editor.config", true, "CAMERA", "Move left", Keyboard::_GetKeyName(Keyboard::LEFT_ARROW));
		File::_SetStringKey("editor.config", true, "CAMERA", "Move right", Keyboard::_GetKeyName(Keyboard::RIGHT_ARROW));
	}
	float cameraMoveSpeed;
	if(File::_GetFloatKey("editor.config", true, "CAMERA", "Move speed", cameraMoveSpeed))
	{
		engine->SetCamera3dMoveSpeed(cameraMoveSpeed);
	}
	bool cameraMovementEnable;
	if(File::_GetBoolKey("editor.config", true, "CAMERA", "Movement enable", cameraMovementEnable))
	{   
		designToolBar->SetViewMovementEnable(cameraMovementEnable);
	}
	bool cameraShowSightEnable;
	if(File::_GetBoolKey("editor.config", true, "CAMERA", "Show sight", cameraShowSightEnable))
	{
		designToolBar->SetViewShowSightEnable(cameraShowSightEnable);
	}
	StringANSI cameraMoveUp;
	if(File::_GetStringKey("editor.config", true, "CAMERA", "Move up", cameraMoveUp))
	{
		engine->SetCameraMoveUp(Keyboard::_GetKeyId(cameraMoveUp));
	}
	StringANSI cameraMoveDown;
	if(File::_GetStringKey("editor.config", true, "CAMERA", "Move down", cameraMoveDown))
	{
		engine->SetCameraMoveDown(Keyboard::_GetKeyId(cameraMoveDown));
	}
	StringANSI cameraMoveLeft;
	if(File::_GetStringKey("editor.config", true, "CAMERA", "Move left", cameraMoveLeft))
	{
		engine->SetCameraMoveLeft(Keyboard::_GetKeyId(cameraMoveLeft));
	}
	StringANSI cameraMoveRight;
	if(File::_GetStringKey("editor.config", true, "CAMERA", "Move right", cameraMoveRight))
	{
		engine->SetCameraMoveRight(Keyboard::_GetKeyId(cameraMoveRight));
	}
	StringANSI cameraMoveForward;
	if(File::_GetStringKey("editor.config", true, "CAMERA", "Move forward", cameraMoveForward))
	{
		engine->SetCameraMoveForward(Keyboard::_GetKeyId(cameraMoveForward));
	}
	StringANSI cameraMoveBackward;
	if(File::_GetStringKey("editor.config", true, "CAMERA", "Move backward", cameraMoveBackward))
	{
		engine->SetCameraMoveBackward(Keyboard::_GetKeyId(cameraMoveBackward));
	}
}


 
void Editor::LoadEngineConfigFile(void)
{
	if(!File::_IsExist("engine.config"))
	{
		File::_SetIntKey("engine.config", true, "WINDOW", "Width", 800);
		File::_SetIntKey("engine.config", true, "WINDOW", "Height", 600);
		File::_SetBoolKey("engine.config", true, "WINDOW", "Fullscreen", false);
		File::_SetFloatKey("engine.config", true, "POINTS", "Size", 8.0f);
		File::_SetBoolKey("engine.config", true, "POINTS", "Anti-aliasing", true);
		File::_SetFloatKey("engine.config", true, "LINES", "Width", 4.2f);
		File::_SetBoolKey("engine.config", true, "LINES", "Anti-aliasing", true);
	}
	float pointSize;
	if(File::_GetFloatKey("engine.config", true, "POINTS", "Size", pointSize))
	{
		Renderer::_SetPointSize(pointSize);
	}

	bool pointsAntiAliasing;
	if(File::_GetBoolKey("engine.config", true, "POINTS", "Anti-aliasing", pointsAntiAliasing))
	{
		engine->SetSmoothEnable(Renderer::POINT, pointsAntiAliasing);
	}
	float linesWidth;
	if(File::_GetFloatKey("engine.config", true, "LINES", "Width", linesWidth))
	{
		Renderer::_SetLineWidth(linesWidth); 
	}

	bool linesAntiAliasing;
	if(File::_GetBoolKey("engine.config", true, "LINES", "Anti-aliasing", linesAntiAliasing))
	{
		engine->SetSmoothEnable(Renderer::LINE, linesAntiAliasing);
	}
}



void Editor::LoadConfigFile(void)
{
	LoadEditorConfigFile();

	LoadEngineConfigFile();
}



Editor::~Editor(void)
{
	EnginePause();
	
	SaveEditorConfigFile();

	DestroyEngine();
	DestroyViewport(); 
	
	DestroyDesignTools();
	DestroyAssetManager();
	DestroyMainMenu();
}




void Editor::UpdateCursorPos(Vector2 _cursorPos)
{
	cursorPosLabel->setText("cursor x/y: " + QString::number((int)_cursorPos.x) + ", " + QString::number((int)_cursorPos.y));
}



void Editor::UpdateCameraPos(Vector3 _cameraPos)
{
	cameraPosLabel->setText("camera x/y/z: " + QString::number(_cameraPos.x) + ", " + QString::number(_cameraPos.y) + ", " + QString::number(_cameraPos.z));
}


 
bool Editor::IsViewportFocus(void)const
{
	return viewport->hasFocus();
}



EngineEx* Editor::GetEngine(void)const
{
	return engine;
}


void Editor::ViewportCursorEnter(void)
{
	if(designToolBar->GetCurrentTool() != DesignToolBar::NO_TOOL)
	{ 
		Mouse::_SetShowCursor(false);
	}
}


void Editor::ViewportCursorLeave(void)
{
	Mouse::_SetShowCursor(true);
}



void Editor::ViewToolShowSightEnableIsChanged(bool _enable)
{
	engine->SetViewShowSightEnable(_enable);
}


void Editor::ViewToolMovementEnableIsChanged(bool _enable)
{
	engine->SetViewMovementEnable(_enable);
}


void Editor::ViewToolZoomEnableIsChanged(void)
{
	engine->SetViewZoomEnable();
}


void Editor::ViewToolRotationEnableIsChanged(void)
{
	engine->SetViewRotationEnable();
}


void Editor::ViewToolPerspectiveIsChecked(void)
{
	engine->SetViewPerspective();
}


void Editor::ViewToolOrthographicIsChecked(void)
{
	engine->SetViewOrthographic();
}


void Editor::ViewToolScreenIsChecked(void)
{
	engine->SetViewScreen();
}



void Editor::ViewToolSpectatorIsChecked(void)
{
	engine->SetViewSpectator();
}


void Editor::ViewToolOverlandIsChecked(void)
{
	engine->SetViewOverland();
}


void Editor::ViewToolFlightIsChecked(void)
{
	engine->SetViewFlight();
}



void Editor::ViewToolFirstPersonIsChecked(void)
{
	engine->SetViewFirstPerson();
}


void Editor::ViewToolObjectViewerIsChecked(void)
{
	engine->SetViewObjectViewer();
}



void Editor::RotateToolHome(void)
{
	engine->SetRotateHome();
}



void Editor::ScaleToolHome(void)
{
	engine->SetScaleHome();
}



void Editor::MoveToolHome(void)
{
	engine->SetMoveHome();
}


void Editor::ViewToolHome(void)
{
	engine->SetViewHome();
}



void Editor::UnpickDesignEraseTool(void)
{
	eraseToolAct->setChecked(false);
	Mouse::_SetShowCursor(true);
}


void Editor::UnpickDesignScaleTool(void)
{
	scaleToolAct->setChecked(false);
	Mouse::_SetShowCursor(true);
}


void Editor::UnpickDesignRotateTool(void)
{
	rotateToolAct->setChecked(false);
	Mouse::_SetShowCursor(true);
}


void Editor::UnpickDesignMoveTool(void)
{
	moveToolAct->setChecked(false);
	Mouse::_SetShowCursor(true);
}



void Editor::UnpickDesignSelectTool(void)
{
	selectToolAct->setChecked(false);
	Mouse::_SetShowCursor(true);
}



void Editor::PickSelectTool(void)
{
	engine->ResetTool(designToolBar->GetCurrentTool());

	moveToolAct->setChecked(false);
	selectToolAct->setChecked(true);
	eraseToolAct->setChecked(false);
	scaleToolAct->setChecked(false);
	rotateToolAct->setChecked(false);
}



void Editor::PickMoveTool(void)
{
	engine->ResetTool(designToolBar->GetCurrentTool());

	moveToolAct->setChecked(true);
	selectToolAct->setChecked(false);
	eraseToolAct->setChecked(false);
	scaleToolAct->setChecked(false);
	rotateToolAct->setChecked(false);
}



void Editor::PickEraseTool(void)
{
	engine->ResetTool(designToolBar->GetCurrentTool());

	moveToolAct->setChecked(false);
	selectToolAct->setChecked(false);
	eraseToolAct->setChecked(true);
	scaleToolAct->setChecked(false);
	rotateToolAct->setChecked(false);
}


void Editor::PickRotateTool(void)
{
	engine->ResetTool(designToolBar->GetCurrentTool());

	moveToolAct->setChecked(false);
	selectToolAct->setChecked(false);
	eraseToolAct->setChecked(false);
	scaleToolAct->setChecked(false);
	rotateToolAct->setChecked(true);
}



void Editor::PickScaleTool(void)
{
	engine->ResetTool(designToolBar->GetCurrentTool());

	moveToolAct->setChecked(false);
	selectToolAct->setChecked(false);
	eraseToolAct->setChecked(false);
	scaleToolAct->setChecked(true);
	rotateToolAct->setChecked(false);
}


void Editor::SelectObject(GraphicsSurface* _object)
{ 
	if(_object)
	{
		engine->SelectObject(_object);
	}
}


void Editor::UnselectObject(GraphicsSurface* _object)
{
	if(_object)
	{
		engine->UnselectObject(_object);
	}
}



void Editor::LoadScene(QString _path)
{
	DestroyScene();
 
	controlToolBar->DisableRecord();

	Trigger::_GetEventLogList().Clear();
	assetManager->GetEventTreeWidget()->ClearLogList();

	engine->ResetTime();
	ObjectScene* scene = ObjectScene::_LoadFromFile(_path.toStdString(), false);

	if(scene)
	{
		AssetLibrary::_GetAssets().AddObject(scene);
		assetManager->ExpandRoot();
	}
}


void Editor::DestroyScene(void)
{
	engine->ResetDesignTools();
	
	bool state = Trigger::_IsBlockEnabled();
	Trigger::_SetBlockEnable(true);
	AssetLibrary::_GetAssets().Clear(MagicContainer::DELETE_NODE);
	Trigger::_SetBlockEnable(state);
}


AssetManager* Editor::GetAssetManager(void)const
{
	return assetManager;
}



void Editor::CreateAssetManager(void)
{
	assetManager = new AssetManager(this);
	assetManager->setGeometry(QRect(10, 70, 245+100+40+100+5, 250+200));
} 


void Editor::PickPlayTool(void)
{
	EnginePlay();
}



void Editor::PickStopTool(void)
{
	EnginePause();
	DestroyScene();

	controlToolBar->DisableRecord();
	
	Trigger::_GetEventLogList().Clear();
	assetManager->GetEventTreeWidget()->ClearLogList();

	engine->ResetTime();
} 

 

void Editor::PickPauseTool(void)
{
	EnginePause();
}



void Editor::LoadRootTrigger(void)
{
	if(AssetLibrary::_GetAssets().IsEmpty())
	{
		if(!AssetLibrary::_IsAssetExist("loader", AssetLibrary::TRIGGER_ASSET))
		{
			Trigger* trigger = new Trigger();
			trigger->Rename("loader");
			trigger->SaveToFile();
			delete trigger;
		}

		AssetLibrary::_LoadCommonAsset("loader", AssetLibrary::TRIGGER_ASSET);
	
		assetManager->ExpandRoot();
	}
}



void Editor::PickNextTool(void)
{
	CallFrame();
}


void Editor::RecordToolIsDisabled(void)
{
	assetManager->GetEventTreeWidget()->UpdateEventTree();
}



void Editor::TimeSpeedToolIsChanged(void)
{
	engine->SetTimeSpeed(controlToolBar->GetCurrentTimeSpeed());
}



void Editor::ShowTimeToolIsChanged(void)
{
	engine->TimeShowEnable(controlToolBar->IsTimeShowEnabled());
}




void Editor::DestroyAssetManager(void)
{
	delete assetManager;
}


DesignToolBar* Editor::GetDesignToolBar(void)const
{
	return designToolBar;
}



ControlToolBar* Editor::GetControlToolBar(void)const
{
	return controlToolBar;
}



void Editor::CreateControlToolBar(void)
{	
	controlToolBar = new ControlToolBar(this);
	QMainWindow::addToolBar(Qt::TopToolBarArea, controlToolBar);
}



void Editor::CreateDesignTools(void)
{
	designToolBar = new DesignToolBar(this);
	QMainWindow::addToolBar(Qt::RightToolBarArea, designToolBar);
	connect(designToolBar, SIGNAL(PickSelectTool()), this, SLOT(PickSelectTool()));
	connect(designToolBar, SIGNAL(PickMoveTool()), this, SLOT(PickMoveTool()));
	connect(designToolBar, SIGNAL(PickEraseTool()), this, SLOT(PickEraseTool()));
	connect(designToolBar, SIGNAL(PickScaleTool()), this, SLOT(PickScaleTool()));
	connect(designToolBar, SIGNAL(PickRotateTool()), this, SLOT(PickRotateTool()));
	 
	connect(designToolBar, SIGNAL(UnpickSelectTool()), this, SLOT(UnpickDesignSelectTool()));
	connect(designToolBar, SIGNAL(UnpickMoveTool()), this, SLOT(UnpickDesignMoveTool()));
	connect(designToolBar, SIGNAL(UnpickEraseTool()), this, SLOT(UnpickDesignEraseTool()));
	connect(designToolBar, SIGNAL(UnpickScaleTool()), this, SLOT(UnpickDesignScaleTool()));
	connect(designToolBar, SIGNAL(UnpickRotateTool()), this, SLOT(UnpickDesignRotateTool()));
}


void Editor::DestroyDesignTools(void)
{
	delete designToolBar;
}



void Editor::CreateViewport(void)
{
	viewport = new Viewport(800, 600, 270+100+40+100, 70, this);

	connect(viewport, SIGNAL(CursorEnter()), this, SLOT(ViewportCursorEnter())); 
    connect(viewport, SIGNAL(CursorLeave()), this, SLOT(ViewportCursorLeave()));  
}



void Editor::DestroyViewport(void)
{
	delete viewport;
}


void Editor::CreateEngine(void)
{
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.x = viewport->width();
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.y = viewport->height();
	def.rendererDef.realTimeWorkerDef.windowDef.position.x = viewport->x();
	def.rendererDef.realTimeWorkerDef.windowDef.position.y = viewport->y();
	def.rendererDef.realTimeWorkerDef.windowDef.clientWindow = viewport->GetHWND();
	def.rendererDef.realTimeWorkerDef.unsuspended = true;
	def.rendererDef.realTimeWorkerDef.windowDef.title = "scene viewer";
	def.rendererDef.zBufferEnable = false;

	engine = new EngineEx(this);
	
	if(engine->Create(def))
	{
	}
}



void Editor::DestroyEngine(void)
{
	delete engine;
}



void Editor::CallFrame(void)
{
	engine->Frame();
}


void Editor::EnginePlay(void)
{
	intervalTimer->start(1.0f/60.0f);

	connect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));

	engine->Pick();

	Trigger::_SetBlockEnable(false);
	LoadRootTrigger();
}


void Editor::EnginePause(void)
{
	intervalTimer->stop(); 

	disconnect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));

	Trigger::_SetBlockEnable(true);
}



SimpleList<HANDLE>& Editor::_GetProcesses(void)
{
	return processes;
}



void Editor::_DestroyProcesses(void)
{
	while(processes.IsNotEmpty())
	{
		TerminateProcess(processes.GetHead()->key, NO_ERROR);
		CloseHandle(processes.GetHead()->key);
		processes.RemoveHead();
	}
}



QString Editor::_GetWorkDir(void)
{
	return workDir;
}


QString Editor::_GetDocsDir(void)
{
	return docsDir;
}



QString Editor::_GetTempDir(void)
{
	return tempDir;
}



QString Editor::_GetResDir(void)
{
	return resDir;
}