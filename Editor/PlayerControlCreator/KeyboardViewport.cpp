#include "KeyboardViewport.h"
#include <Engine/Output/File/File.h>
#include <Editor/ErrorDialog/ErrorDialog.h>




KeyboardViewport::KeyboardViewport(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	intervalTimer = new QTimer(this);
	CreateViewport();
	CreateEngine();

	setWindowTitle("Virtual Keyboard");
    resize(viewport->width(), viewport->height());
}



void KeyboardViewport::keyPressEvent(QKeyEvent* _event)
{
}



void KeyboardViewport::closeEvent(QCloseEvent *_event)
{
	EnginePause();
}



KeyboardViewport::~KeyboardViewport(void)
{
	EnginePause();
	delete engine;
}


int32 KeyboardViewport::GetKeyId(void)const
{
	return engine->GetSelectedKey();
}



void KeyboardViewport::CreateViewport(void)
{
	viewport = new Viewport(1024, 700, 0, 0, this);
}


void KeyboardViewport::CallFrame(void)
{
	engine->Frame();
}


void KeyboardViewport::EnginePlay(void)
{
	intervalTimer->start(1.0f/60.0f);

	connect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}


void KeyboardViewport::EnginePause(void)
{
	intervalTimer->stop();

	disconnect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}


void KeyboardViewport::CreateEngine(void)
{
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.x = viewport->width();
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.y = viewport->height();
	def.rendererDef.realTimeWorkerDef.windowDef.position.x = viewport->x();
	def.rendererDef.realTimeWorkerDef.windowDef.position.y = viewport->y();
	def.rendererDef.realTimeWorkerDef.windowDef.clientWindow = viewport->GetHWND();
	def.rendererDef.realTimeWorkerDef.windowDef.title = "virtual keyboard";

	engine = new KeyboardViewer();
	
	if(engine->Create(def))
	{
		 
		EnginePlay();
	}
}



void KeyboardViewport::resizeEvent(QResizeEvent* _event)
{
	engine->SetClientAreaSize(Vector2(_event->size().width(), _event->size().height()));
	QDialog::resizeEvent(_event);
}


void KeyboardViewport::SetKeyId(int32 _key)
{
	engine->SetSelectedKey(_key);
}