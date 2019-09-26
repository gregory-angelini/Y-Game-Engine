#include "FloatLerpCreator.h"
#include <Engine/Output/File/File.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Editor/WarningDialog/WarningDialog.h>



FloatLerpCreator::FloatLerpCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), fastCreatingEnable(false)
{
	intervalTimer = new QTimer(this);
	viewportGroupBox = new QGroupBox(this);
    viewportGroupBox->setGeometry(QRect(10, 10, 800 + 20, 600 + 30));
	viewportGroupBox->setTitle("Viewer:");


	CreateViewport();
	CreateEngine();
	nameLabel = new QLabel(this); 
	nameLabel->setGeometry(QRect(880-20-5, 10+10, 41, 20));
	nameLabel->setText("Name:");

	nameMaxLength = 256;
    
	nameLineEdit = new QLineEdit(this);
    nameLineEdit->setGeometry(QRect(960-20, 10+10, 171-11, 20));
	nameLineEdit->setMaxLength(nameMaxLength);
	nameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	targetLabel = new QLabel(this);
    targetLabel->setGeometry(QRect(880-20-5, 50+10, 71, 21));
	targetLabel->setText("Target:");

	float minTarget = 0.1f;
	float maxTarget = Numerical<float>::_UpperValue();

	targetSpinBox = new FloatSpinBox(this);
    targetSpinBox->setGeometry(QRect(960-20, 50+10, 81, 22)); 
	targetSpinBox->setValue(1.0f);
	targetSpinBox->setRange(minTarget, maxTarget);
	targetSpinBox->setToolTip(QString::number(minTarget) + " - " + QString::number(maxTarget));

	connect(targetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(TargetIsChanged(double)));
	currentLabel = new QLabel(this);
    currentLabel->setGeometry(QRect(880-20-5, 90+10, 71, 21));
	currentLabel->setText("Current:");

	currentSpinBox = new FloatSpinBox(this);
    currentSpinBox->setGeometry(QRect(960-20, 90+10, 81, 22));
	currentSpinBox->setDisabled(true);
	tLabel = new QLabel(this);
    tLabel->setGeometry(QRect(880-20-5, 130+10, 71, 21));
	tLabel->setText("T:");

	tSpinBox = new FloatSpinBox(this);
    tSpinBox->setGeometry(QRect(960-20, 130+10, 81, 22));
	tSpinBox->setDisabled(true);
	deltaLabel = new QLabel(this);
    deltaLabel->setGeometry(QRect(880-20-5, 170+10, 71, 21));
    deltaLabel->setText("Delta:");
	
	deltaSpinBox = new FloatSpinBox(this);
    deltaSpinBox->setGeometry(QRect(960-20, 170+10, 81, 22));
	deltaSpinBox->setDisabled(true);
	lerpModeLabel = new QLabel(this);
    lerpModeLabel->setGeometry(QRect(880-20-5, 210+10, 81, 21));
	lerpModeLabel->setText("Lerp mode:");

	lerpModeComboBox = new QComboBox(this);
    lerpModeComboBox->setGeometry(QRect(960-20, 210+10, 141, 22));
	lerpModeComboBox->addItem("UNITS_PER_SECOND");
	lerpModeComboBox->addItem("FIXED_TIME");
	lerpModeComboBox->addItem("PARABOLA");
	lerpModeComboBox->addItem("DAMPING");
	lerpModeComboBox->addItem("MAGNETIC");
	connect(lerpModeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(LerpModeIsChanged(QString)));
	playingSpeedLabel = new QLabel(this);
    playingSpeedLabel->setGeometry(QRect(880-20-5, 250+10, 71, 21));
	playingSpeedLabel->setText("Playing speed:");
        
	float minPlayingSpeed = 0.0001f;
	float maxPlayingSpeed = Numerical<float>::_UpperValue();

	playingSpeedSpinBox = new FloatSpinBox(this);
    playingSpeedSpinBox->setGeometry(QRect(960-20, 250+10, 81, 22));
	playingSpeedSpinBox->setValue(1.0f);
	playingSpeedSpinBox->setRange(minPlayingSpeed, maxPlayingSpeed);
	playingSpeedSpinBox->setToolTip(QString::number(minPlayingSpeed) + " - " + QString::number(maxPlayingSpeed));
	connect(playingSpeedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PlayingSpeedIsChanged(double)));
	playingDurationLabel = new QLabel(this);
    playingDurationLabel->setGeometry(QRect(880-20-5, 290+10, 71+10+10, 21));
	playingDurationLabel->setText("Playing duration:"); 
        
	float minPlayingDuration = 0.01f;
	float maxPlayingDuration = Numerical<float>::_UpperValue();

	playingDurationSpinBox = new FloatSpinBox(this); 
    playingDurationSpinBox->setGeometry(QRect(960-20, 290+10, 81, 22));
	playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
	playingDurationSpinBox->setRange(minPlayingDuration, maxPlayingDuration);
	playingDurationSpinBox->setToolTip(QString::number(minPlayingDuration) + " - " + QString::number(maxPlayingDuration));
	connect(playingDurationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DurationIsChanged(double)));
	playingModeGroupBox = new QGroupBox(this);
    playingModeGroupBox->setGeometry(QRect(880-20-5, 290+10+20+20, 181+10+10+5+5+3, 141));
	playingModeGroupBox->setTitle("Playing mode:");
	QButtonGroup* group1 = new QButtonGroup(playingModeGroupBox);
	QButtonGroup* group2 = new QButtonGroup(playingModeGroupBox);
	QButtonGroup* group3 = new QButtonGroup(playingModeGroupBox);
	forwardRadioButton = new QRadioButton(playingModeGroupBox);
	forwardRadioButton->setGeometry(QRect(10+5+5, 20, 71, 21));
	forwardRadioButton->setText("Forward");
	connect(forwardRadioButton, SIGNAL(toggled(bool)), this, SLOT(ForwardPlayingModeEnabled(bool)));
	group1->addButton(forwardRadioButton);
	backwardRadioButton = new QRadioButton(playingModeGroupBox);   
    backwardRadioButton->setGeometry(QRect(90+5+10+10+5, 20, 71, 21));
	backwardRadioButton->setText("Backward");
	connect(backwardRadioButton, SIGNAL(toggled(bool)), this, SLOT(BackwardPlayingModeEnabled(bool)));
	group1->addButton(backwardRadioButton);
    pingPongRadioButton = new QRadioButton(playingModeGroupBox);
    pingPongRadioButton->setGeometry(QRect(10+5+5, 60, 71, 21));
	pingPongRadioButton->setText("Ping pong");
	connect(pingPongRadioButton, SIGNAL(toggled(bool)), this, SLOT(PingPongPlayingModeEnabled(bool)));
	group2->addButton(pingPongRadioButton);
	onewayRadioButton = new QRadioButton(playingModeGroupBox);
    onewayRadioButton->setGeometry(QRect(90+5+10+10+5, 60, 61, 21));
	onewayRadioButton->setText("Oneway");
	connect(onewayRadioButton, SIGNAL(toggled(bool)), this, SLOT(OnewayPlayingModeEnabled(bool)));
	group2->addButton(onewayRadioButton);
	loopRadioButton = new QRadioButton(playingModeGroupBox);
    loopRadioButton->setGeometry(QRect(10+5+5, 100, 91, 21));
	loopRadioButton->setText("Loop");
	connect(loopRadioButton, SIGNAL(toggled(bool)), this, SLOT(LoopPlayingModeEnabled(bool)));
	group3->addButton(loopRadioButton);
	singleshotRadioButton = new QRadioButton(playingModeGroupBox);
    singleshotRadioButton->setGeometry(QRect(90+5+10+10+5, 100, 71, 21));
	singleshotRadioButton->setText("Single shot");
	connect(singleshotRadioButton, SIGNAL(toggled(bool)), this, SLOT(SingleshotPlayingModeEnabled(bool)));
	group3->addButton(singleshotRadioButton);


	forwardRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::FORWARD_PLAYING);
	backwardRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::BACKWARD_PLAYING);
	pingPongRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::PING_PONG_PLAYING);
	onewayRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::ONEWAY_PLAYING);
	loopRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::ONEWAY_PLAYING);
	singleshotRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::SINGLE_SHOT_PLAYING);
	showScaleLabel = new QLabel(this);
    showScaleLabel->setGeometry(QRect(20, 610+30, 61, 41));
    showScaleLabel->setText("Show range:");
    
	showScaleCheckBox = new QCheckBox(this);
    showScaleCheckBox->setGeometry(QRect(80+8, 620+30, 21, 21));
	showScaleCheckBox->setChecked(engine->GetGraph()->IsXRangeShowEnabled());
	connect(showScaleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ShowScaleIsChanged(int)));
	scaleXLabel = new QLabel(this);
    scaleXLabel->setGeometry(QRect(110+8, 620+30, 41, 21));
	scaleXLabel->setText("Scale X:");
	 
	scaleXSpinBox = new FloatSpinBox(this);
    scaleXSpinBox->setGeometry(QRect(150+10, 620+30, 61, 21));
	scaleXSpinBox->setValue(engine->GetGraph()->GetScaleX()); 
	connect(scaleXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(ScaleXIsChanged(double)));
	scaleXSpinBox->setRange(0.1f, Numerical<float>::_UpperValue());
	scaleYLabel = new QLabel(this);
    scaleYLabel->setGeometry(QRect(230+8+2, 620+30, 41, 21));
	scaleYLabel->setText("Scale Y:");

    scaleYSpinBox = new FloatSpinBox(this);
    scaleYSpinBox->setGeometry(QRect(270+10+2, 620+30, 61, 22));
	scaleYSpinBox->setValue(engine->GetGraph()->GetScaleY()); 
	connect(scaleYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(ScaleYIsChanged(double)));
	scaleYSpinBox->setRange(0.1f, Numerical<float>::_UpperValue());
	playButton = new QPushButton(this);
	playButton->setGeometry(QRect(410, 620+30, 75, 23));
	playButton->setText("Play");
	connect(playButton, SIGNAL(clicked()), this, SLOT(PlayLerp()));
	pauseButton = new QPushButton(this);
    pauseButton->setGeometry(QRect(500, 620+30, 75, 23));
	pauseButton->setText("Stop");
	connect(pauseButton, SIGNAL(clicked()), this, SLOT(StopLerp()));
	currentZoomLabel = new QLabel(this);
    currentZoomLabel->setGeometry(QRect(740, 560+60+30, 41+40, 31));
    currentZoomLabel->setText("Zoom: 100%");
	homeButton = new QToolButton(this);
	homeButton->move(690, 560+60+30);

	QPixmap homeIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "FloatLerpCreator/home.png");
	homeButton->setFixedSize(homeIcon.size());
	homeButton->setIcon(homeIcon);
	homeButton->setToolTip("Home");
	homeButton->setAutoRaise(true);
	connect(homeButton, SIGNAL(clicked()), this, SLOT(SetHome()));
	helpButton = new QToolButton(this);
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	helpButton->setGeometry(QRect(980-50-40-40-20, 570+60+20+20, 32, 32));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
	okButton = new QPushButton(this);
    okButton->setGeometry(QRect(1100-40-40-20, 570+60+20+20, 101, 31));
	okButton->setText("Ok");
	connect(okButton, SIGNAL(clicked()), this, SLOT(IsOk()));
	cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(980-40-40-20, 570+60+20+20, 101, 31));
	cancelButton->setText("Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setFocus();

	setWindowTitle("Float Lerp Creator");
	setFixedSize(1170+10-40-20, 678+20+20);
}



void FloatLerpCreator::ForwardPlayingModeEnabled(bool _enable)
{
	if(_enable)
	{
		GetFloatLerp()->SetPlayingMode(PlayingObject::FORWARD_PLAYING);

		playingDurationSpinBox->blockSignals(true);
		playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
		playingDurationSpinBox->blockSignals(false);
		engine->ClearHistory();
		bool playing = GetFloatLerp()->IsPlaying();
		engine->StopLerp();
		if(playing) { engine->PlayLerp(); }
		engine->UpdateMark();
		engine->ShowCurveForAllLerps();
	}
}




void FloatLerpCreator::BackwardPlayingModeEnabled(bool _enable)
{
	if(_enable)
	{
		GetFloatLerp()->SetPlayingMode(PlayingObject::BACKWARD_PLAYING);

		playingDurationSpinBox->blockSignals(true);
		playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
		playingDurationSpinBox->blockSignals(false);
		engine->ClearHistory();
		bool playing = GetFloatLerp()->IsPlaying();
		engine->StopLerp();
		if(playing) { engine->PlayLerp(); }
		engine->UpdateMark();
		engine->ShowCurveForAllLerps();
	}
}



void FloatLerpCreator::PingPongPlayingModeEnabled(bool _enable)
{
	if(_enable)
	{
		GetFloatLerp()->SetPlayingMode(PlayingObject::PING_PONG_PLAYING);

		playingDurationSpinBox->blockSignals(true);
		playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
		playingDurationSpinBox->blockSignals(false);
		engine->ClearHistory();
		bool playing = GetFloatLerp()->IsPlaying();
		engine->StopLerp();
		if(playing) { engine->PlayLerp(); }
		engine->UpdateMark();
		engine->ShowCurveForAllLerps();
	}
}


void FloatLerpCreator::OnewayPlayingModeEnabled(bool _enable)
{
	if(_enable)
	{
		GetFloatLerp()->SetPlayingMode(PlayingObject::ONEWAY_PLAYING);

		playingDurationSpinBox->blockSignals(true);
		playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
		playingDurationSpinBox->blockSignals(false);
		engine->ClearHistory();
		bool playing = GetFloatLerp()->IsPlaying();
		engine->StopLerp();
		if(playing) { engine->PlayLerp(); }
		engine->UpdateMark();
		engine->ShowCurveForAllLerps();
	}
}



void FloatLerpCreator::LoopPlayingModeEnabled(bool _enable)
{
	if(_enable)
	{
		GetFloatLerp()->SetPlayingMode(PlayingObject::ONEWAY_PLAYING);

		playingDurationSpinBox->blockSignals(true);
		playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
		playingDurationSpinBox->blockSignals(false);
		engine->ClearHistory();
		bool playing = GetFloatLerp()->IsPlaying();
		engine->StopLerp();
		if(playing) { engine->PlayLerp(); }
		engine->UpdateMark();
		engine->ShowCurveForAllLerps();
	}
}


void FloatLerpCreator::SingleshotPlayingModeEnabled(bool _enable)
{
	if(_enable)
	{
		GetFloatLerp()->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING);

		playingDurationSpinBox->blockSignals(true);
		playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
		playingDurationSpinBox->blockSignals(false);
		engine->ClearHistory();
		bool playing = GetFloatLerp()->IsPlaying();
		engine->StopLerp();
		if(playing) { engine->PlayLerp(); }
		engine->UpdateMark();
		engine->ShowCurveForAllLerps();
	}
}


void FloatLerpCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}


bool FloatLerpCreator::IsCurrentLerpMode(int32 _mode)
{
	return engine->GetFloatLerp()->GetLerpMode() == _mode;
}



void FloatLerpCreator::ScaleYIsChanged(double _value)
{
	engine->GetGraph()->SetScaleY(_value);

	engine->UpdateMark();
}


void FloatLerpCreator::ScaleXIsChanged(double _value)
{
	engine->GetGraph()->SetScaleX(_value);

	engine->UpdateMark();
}


void FloatLerpCreator::ShowScaleIsChanged(int _enable)
{
	engine->GetGraph()->SetXRangeShowEnable(_enable);
	engine->GetGraph()->SetYRangeShowEnable(_enable);
}


void FloatLerpCreator::PlayLerp(void)
{
	if(engine->GetFloatLerp()->IsPlaying())
	{
		engine->PauseLerp(); 
	} 
	else
	{
		if(engine->GetFloatLerp()->GetCurrent() == engine->GetFloatLerp()->GetTarget() ||
		  ((engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::BACKWARD_PLAYING) && engine->GetFloatLerp()->GetCurrent() == 0.0f))
		{
			engine->ClearHistory();
			engine->StopLerp();
			engine->PlayLerp(); 
		}
		else
		{
			engine->PlayLerp();
		}
	}
}

void FloatLerpCreator::StopLerp(void)
{
	engine->StopLerp();
	engine->ClearHistory();
	playButton->setText("Play");
}



void FloatLerpCreator::PrepareToPlay(void)
{
	if(playButton->text() != "Play")
	{
		playButton->setText("Play"); 
	}
}


void FloatLerpCreator::PrepareToPause(void)
{
	if(playButton->text() != "Pause")
	{
		playButton->setText("Pause");
	}
}


void FloatLerpCreator::LerpModeIsChanged(QString _mode)
{
	float targetTime = engine->GetPlayingDurationInSec();

	engine->GetFloatLerp()->SetLerpMode(lerpModeComboBox->currentIndex());
	
	
	engine->SetPlayingDurationInSec(targetTime);

	playingDurationSpinBox->blockSignals(true);
	playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
	playingDurationSpinBox->blockSignals(false);

	playingSpeedSpinBox->blockSignals(true);
	playingSpeedSpinBox->setValue(engine->GetFloatLerp()->GetPlayingSpeed());
	playingSpeedSpinBox->blockSignals(false);

	engine->UpdateMark();
	engine->ShowCurveForAllLerps();
}



void FloatLerpCreator::DurationIsChanged(double _value)
{
	engine->SetPlayingDurationInSec(_value);

	playingSpeedSpinBox->blockSignals(true);
	playingSpeedSpinBox->setValue(engine->GetFloatLerp()->GetPlayingSpeed());
	playingSpeedSpinBox->blockSignals(false);


	engine->UpdateMark();
	engine->ShowCurveForAllLerps();
}


void FloatLerpCreator::PlayingSpeedIsChanged(double _playingSpeed)
{
	engine->SetPlayingSpeed(_playingSpeed);  

	playingDurationSpinBox->blockSignals(true);
	playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
	playingDurationSpinBox->blockSignals(false);


	engine->UpdateMark();
	engine->ShowCurveForAllLerps(); 
}


void FloatLerpCreator::TargetIsChanged(double _target)
{
	engine->SetTarget(_target);

	
	playingSpeedSpinBox->blockSignals(true);
	playingSpeedSpinBox->setValue(engine->GetFloatLerp()->GetPlayingSpeed());
	playingSpeedSpinBox->blockSignals(false);

	playingDurationSpinBox->blockSignals(true);
	playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
	playingDurationSpinBox->blockSignals(false);
	

	engine->UpdateMark();
	engine->ShowCurveForAllLerps();
}


Viewport* FloatLerpCreator::GetViewport(void)const
{
	return viewport;
}


void FloatLerpCreator::CreateEngine(void)
{
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.x = viewport->width();
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.y = viewport->height();
	def.rendererDef.realTimeWorkerDef.windowDef.position.x = viewport->x();
	def.rendererDef.realTimeWorkerDef.windowDef.position.y = viewport->y();
	def.rendererDef.realTimeWorkerDef.windowDef.clientWindow = viewport->GetHWND();
	def.rendererDef.realTimeWorkerDef.unsuspended = true;

	def.rendererDef.realTimeWorkerDef.windowDef.title = "float lerp viewer";

	engine = new FloatLerpViewer(this);
	
	if(engine->Create(def))
	{
		 
		EnginePlay();
	}
}



void FloatLerpCreator::EnginePause(void)
{
	intervalTimer->stop();
	disconnect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}


void FloatLerpCreator::CallFrame(void)
{
	engine->Frame();
}


void FloatLerpCreator::EnginePlay(void)
{
	intervalTimer->start(1.0f/60.0f);
		
	connect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}



void FloatLerpCreator::CreateViewport(void)
{
	viewport = new Viewport(800, 600, 10, 20, viewportGroupBox);

	connect(viewport, SIGNAL(CursorEnter()), this, SLOT(ViewportCursorEnter())); 
    connect(viewport, SIGNAL(CursorLeave()), this, SLOT(ViewportCursorLeave())); 
}



FloatLerp* FloatLerpCreator::GetFloatLerp(void)const
{
	return engine->GetFloatLerp();
}



void FloatLerpCreator::SetFloatLerp(QString _name)
{

	if(engine->GetFloatLerp()->LoadFromFile(_name.toAscii().data()))
	{
		nameLineEdit->setText(engine->GetFloatLerp()->GetName().c_str());
		lerpModeComboBox->blockSignals(true);
		lerpModeComboBox->setCurrentIndex(engine->GetFloatLerp()->GetLerpMode());
		lerpModeComboBox->blockSignals(false);
		targetSpinBox->blockSignals(true);
		targetSpinBox->setValue(engine->GetFloatLerp()->GetTarget()); 
		targetSpinBox->blockSignals(false);
		playingSpeedSpinBox->blockSignals(true);
		playingSpeedSpinBox->setValue(engine->GetFloatLerp()->GetPlayingSpeed());
		playingSpeedSpinBox->blockSignals(false);
		forwardRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::FORWARD_PLAYING);
		backwardRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::BACKWARD_PLAYING);
		pingPongRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::PING_PONG_PLAYING);
		onewayRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::ONEWAY_PLAYING);
		loopRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::ONEWAY_PLAYING);
		singleshotRadioButton->setChecked(engine->GetFloatLerp()->GetPlayingMode() & PlayingObject::SINGLE_SHOT_PLAYING);
		playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());

		engine->GetFloatLerp()->LoadFromFile(_name.toAscii().data());

		engine->UpdateMark();
		engine->ShowCurveFromStartToCurrent();
		

		engine->GetFloatLerp()->LoadFromFile(_name.toAscii().data());

		if(engine->GetFloatLerp()->IsPlaying())
		{ 
			engine->PlayLerp();
		}
		else 
		{
			engine->PauseLerp();
		}
	}

	
	setWindowTitle("Float Lerp Editor");
}



void FloatLerpCreator::ViewportCursorEnter(void)
{
	engine->ShowCursor();
}


void FloatLerpCreator::ViewportCursorLeave(void)
{
	engine->HideCursor();
}


void FloatLerpCreator::SetZoomValue(int _zoom)
{
	currentZoomLabel->setText("Zoom: " + QString::number(_zoom) + "%");
}


void FloatLerpCreator::UpdateCurrent(void)
{
	deltaSpinBox->setValue(engine->GetFloatLerp()->GetDelta());
	currentSpinBox->setValue(engine->GetFloatLerp()->GetCurrent());
	tSpinBox->setValue(engine->GetFloatLerp()->GetT());
}


void FloatLerpCreator::UpdatePlayingSpeed(void)
{
	playingSpeedSpinBox->blockSignals(true);
	playingDurationSpinBox->blockSignals(true);
	
	playingSpeedSpinBox->setValue(engine->GetFloatLerp()->GetPlayingSpeed());
	playingDurationSpinBox->setValue(engine->GetPlayingDurationInSec());
	
	playingSpeedSpinBox->blockSignals(false);
	playingDurationSpinBox->blockSignals(false);
}



void FloatLerpCreator::SetHome(void)
{
	engine->StopLerp();
	engine->ClearHistory();

	engine->GetCamera()->SetRotation(Quaternion());
	engine->GetCamera()->SetDistanceToViewingPoint(GetZoom100());
	engine->GetCamera()->SetViewingPoint(Vector3());

	Ratio::_SetKnown<float>(Ratio::WHOLE, GetZoom100());
	Ratio::_SetKnown<float>(Ratio::PART, engine->GetCamera()->GetDistanceToViewingPoint());
	SetZoomValue(Ratio::_GetUnknown<float>(Ratio::PART_RATIO));
}



float FloatLerpCreator::GetZoom100(void)const
{
	return Numerical<float>::_GetMax(engine->GetGraph()->GetSize().x, engine->GetGraph()->GetSize().y) + Angle::_RadiansToDegrees(engine->GetCamera()->GetFov());
}


QString FloatLerpCreator::GetObjectName(void)const
{
	return nameLineEdit->text();
}


void FloatLerpCreator::SetNameDisable(bool _disable)
{
	nameLineEdit->setDisabled(_disable);
}


void FloatLerpCreator::SetObjectName(QString _name)
{
	nameLineEdit->setText(_name);
}



FloatLerpCreator::~FloatLerpCreator(void)
{
	EnginePause();
 
	delete engine;
}


void FloatLerpCreator::closeEvent(QCloseEvent *_event)
{
	EnginePause();
}



void FloatLerpCreator::ShowDoc(void)
{
	QString path = (Editor::_GetWorkDir() + Editor::_GetDocsDir() + "FloatLerpCreator.doc");

	QUrl url = QUrl::fromLocalFile(path);

	QDesktopServices::openUrl(url);
}


void FloatLerpCreator::IsOk(void)
{
	{
		int32 errorCode;
		if(nameLineEdit->text().isEmpty())
		{
			ErrorDialog("Error", "Float lerp name is not defined", 155+30, 40);
			return;
		}
		if((errorCode = BooleanExprParser::_IsNameValid(nameLineEdit->text().toAscii().data())) >= 0)
		{
			ErrorDialog("Error", "Float lerp name is not valid: '" + QString(nameLineEdit->text().at(errorCode)) + "'", 190, 40);
			return;
		} 
	}
	if(File::_IsExist(AssetLibrary::_GetDir() + FloatLerp::_GetDir() + StringANSI(nameLineEdit->text().toAscii().data()) + "." + FloatLerp::_GetFileSuffix()))
	{ 
		WarningDialog warning("Warning", "Float lerp '" +  nameLineEdit->text() + "' is already exist. Rewrite file?", 200, 90);
		
		if(warning.exec() == QDialog::Rejected)
		{
			return;
		}
	}

	engine->GetFloatLerp()->Rename(nameLineEdit->text().toAscii().data());

	if(!fastCreatingEnable)
	{
		engine->GetFloatLerp()->SaveToFile();

		EnginePause();
		delete engine->GetFloatLerp();
	}
	else
	{
		if(AssetLibrary::_GetAssets().AddObject(engine->GetFloatLerp()) == NIL)
		{
			ErrorDialog("Error", "Name '" + nameLineEdit->text() + "' is already reserved", 195, 40);
			return;
		}
	}

	accept();
}