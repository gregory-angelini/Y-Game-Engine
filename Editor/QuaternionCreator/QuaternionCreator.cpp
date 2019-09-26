#include "QuaternionCreator.h"
#include <Engine/Output/File/File.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Editor/WarningDialog/WarningDialog.h>



QuaternionCreator::QuaternionCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
	intervalTimer = new QTimer(this);
	viewportGroupBox = new QGroupBox(this);
    viewportGroupBox->setGeometry(QRect(10, 10, 800 + 20, 600 + 30));
	viewportGroupBox->setTitle("Viewer:");


	CreateViewport();
	CreateEngine();
	angleFormatLabel = new QLabel(this);
    angleFormatLabel->setGeometry(QRect(830+5+50+200+40-80-5, 10+15, 81, 22));
	angleFormatLabel->setText("Angle format:");
	

	angleFormatComboBox = new QComboBox(this);
    angleFormatComboBox->setGeometry(QRect(830+5+50+200+40, 10+15, 81, 22));
	angleFormatComboBox->addItem("DEGREES");
	angleFormatComboBox->addItem("RADIANS");
	connect(angleFormatComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ChangeAngleFormat(QString)));
	axisAngleGroupBox = new QGroupBox(this);
    axisAngleGroupBox->setGeometry(QRect(830+5, 10+40, 391-5, 91));
	axisAngleGroupBox->setTitle("Axis Angle:");
	axisLabel = new QLabel(axisAngleGroupBox);  
    axisLabel->setGeometry(QRect(10, 20, 71, 21));
	axisLabel->setText("Axis (X/Y/Z):");
     
	axisXSpinBox = new FloatSpinBox(axisAngleGroupBox);
    axisXSpinBox->setGeometry(QRect(80, 20, 62, 22));
	axisXSpinBox->setValue(1.0f);
	connect(axisXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AxisAngleIsChanged(double)));

        
	axisYSpinBox = new FloatSpinBox(axisAngleGroupBox);
    axisYSpinBox->setGeometry(QRect(150, 20, 62, 22));
	connect(axisYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AxisAngleIsChanged(double)));
       
	axisZSpinBox = new FloatSpinBox(axisAngleGroupBox);  
    axisZSpinBox->setGeometry(QRect(220, 20, 62, 22));
	connect(axisZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AxisAngleIsChanged(double)));
	angleLabel = new QLabel(axisAngleGroupBox);
    angleLabel->setGeometry(QRect(10, 60, 41, 21));
	angleLabel->setText("Angle:");
      
	angleSpinBox = new FloatSpinBox(axisAngleGroupBox);
    angleSpinBox->setGeometry(QRect(80, 60, 62, 22));
	connect(angleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AxisAngleIsChanged(double)));
	eulerGroupBox = new QGroupBox(this);
    eulerGroupBox->setGeometry(QRect(830+5, 110+40, 391-5, 91-40));
	eulerGroupBox->setTitle("Euler Angles:");
	pitchLabel = new QLabel(eulerGroupBox);
    pitchLabel->setGeometry(QRect(10, 20, 51, 21));
	pitchLabel->setText("Pitch (X):");
	
	pitchSpinBox = new FloatSpinBox(eulerGroupBox);
    pitchSpinBox->setGeometry(QRect(60, 20, 62, 22));
	connect(pitchSpinBox, SIGNAL(valueChanged(double)), this, SLOT(EulerAnglesIsChanged(double)));
	yawLabel = new QLabel(eulerGroupBox);
    yawLabel->setGeometry(QRect(140, 20, 51, 21));
	yawLabel->setText("Yaw (Y):");

	yawSpinBox = new FloatSpinBox(eulerGroupBox);
    yawSpinBox->setGeometry(QRect(180, 20, 62, 22));
	connect(yawSpinBox, SIGNAL(valueChanged(double)), this, SLOT(EulerAnglesIsChanged(double)));
    rollLabel = new QLabel(eulerGroupBox);
    rollLabel->setGeometry(QRect(260, 20, 41, 21));
	rollLabel->setText("Roll (Z):");
     
    rollSpinBox = new FloatSpinBox(eulerGroupBox);
    rollSpinBox->setGeometry(QRect(300, 20, 62, 22));
	connect(rollSpinBox, SIGNAL(valueChanged(double)), this, SLOT(EulerAnglesIsChanged(double)));
	qDataLabel = new QLabel(this);
    qDataLabel->setGeometry(QRect(840, 220, 111, 21));
	qDataLabel->setText("Quaternion (X/Y/Z/W):");

	qxSpinBox = new FloatSpinBox(this);
    qxSpinBox->setGeometry(QRect(950, 220, 62, 22));
	qxSpinBox->setDisabled(true);
          
	qySpinBox = new FloatSpinBox(this);
    qySpinBox->setGeometry(QRect(1020, 220, 62, 22));
	qySpinBox->setDisabled(true);

	qzSpinBox = new FloatSpinBox(this);
    qzSpinBox->setGeometry(QRect(1090, 220, 62, 22));
	qzSpinBox->setDisabled(true);
 
	qwSpinBox = new FloatSpinBox(this);
    qwSpinBox->setGeometry(QRect(1160, 220, 62, 22));
	qwSpinBox->setDisabled(true);
	showGlobalSpinsLabel = new QLabel(this);
    showGlobalSpinsLabel->setGeometry(QRect(840, 290, 101, 41));
    showGlobalSpinsLabel->setText("Show global spins:");
      
	showGlobalSpinsCheckBox = new QCheckBox(this);
    showGlobalSpinsCheckBox->setGeometry(QRect(950, 300, 21, 20));
	showGlobalSpinsCheckBox->setChecked(true);
	connect(showGlobalSpinsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ShowGlobalSpins(int)));
	

        
	showGlobalAxisesLabel = new QLabel(this);
    showGlobalAxisesLabel->setGeometry(QRect(840, 330, 101, 41));
	showGlobalAxisesLabel->setText("Show global axises:");
	
	showGlobalAxisesCheckBox = new QCheckBox(this);
    showGlobalAxisesCheckBox->setGeometry(QRect(950, 340, 16, 21));
	showGlobalAxisesCheckBox->setChecked(true);
	connect(showGlobalAxisesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ShowGlobalAxises(int)));
	rotationModeLabel = new QLabel(this);
    rotationModeLabel->setGeometry(QRect(840, 260, 81, 21));
	rotationModeLabel->setText("Rotation mode:");

	rotationModeComboBox = new QComboBox(this);
    rotationModeComboBox->setGeometry(QRect(950, 260, 171, 22));
	rotationModeComboBox->addItem("EULER_SCROLLBAR");
	rotationModeComboBox->addItem("SPHERICAL_SWING_TWIST");
	rotationModeComboBox->addItem("SPHERICAL_TWO_POLE");
	rotationModeComboBox->addItem("SPHERICAL_FROM_TO");
	connect(rotationModeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(SelectRotationMode(QString)));
	currentZoomLabel = new QLabel(this);
    currentZoomLabel->setGeometry(QRect(740, 560+60+30, 41+40, 31));
    currentZoomLabel->setText("Zoom: 100%");
    relativeRotationLabel = new QLabel(this);
    relativeRotationLabel->setGeometry(QRect(320, 570+60+30, 151, 16));
    relativeRotationLabel->setText("Relative rotation: [0, 0, 0]");
	homeButton = new QToolButton(this);
	homeButton->move(690, 560+60+30);

	QPixmap homeIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "QuaternionCreator/home.png");
	homeButton->setFixedSize(homeIcon.size());
	homeButton->setIcon(homeIcon);

	homeButton->setToolTip("Home");
	homeButton->setAutoRaise(true);
	connect(homeButton, SIGNAL(clicked()), this, SLOT(SetHome()));
	helpButton = new QToolButton(this);
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	helpButton->setGeometry(QRect(980-50, 570+60+20, 32, 32));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
	rightView = new QPushButton(this);
    rightView->setGeometry(QRect(690-60-20, 560+60+30, 60, 32));
	rightView->setText("Right");
	rightView->setToolTip("Right view");
	connect(rightView, SIGNAL(clicked()), this, SLOT(RightView()));
	topDownView = new QPushButton(this);
    topDownView->setGeometry(QRect(690-60-60-25, 560+60+30, 60, 32));
	topDownView->setText("Top-Down");
	topDownView->setToolTip("Top-Down view");
	connect(topDownView, SIGNAL(clicked()), this, SLOT(TopDownView()));
	frontView = new QPushButton(this);
    frontView->setGeometry(QRect(690-60-60-60-30, 560+60+30, 60, 32));
	frontView->setText("Front");
	frontView->setToolTip("Front view");
	connect(frontView, SIGNAL(clicked()), this, SLOT(FrontView()));
	okButton = new QPushButton(this);
    okButton->setGeometry(QRect(1100, 570+60+20, 101, 31));
	okButton->setText("Ok");
	connect(okButton, SIGNAL(clicked()), this, SLOT(IsOk()));
	cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(980, 570+60+20, 101, 31));
	cancelButton->setText("Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setFocus();

	setWindowTitle("Quaternion Editor");
    setFixedSize(1232, 616+60+30);
}



void QuaternionCreator::ConvertAngle(QString _format)
{
	angleSpinBox->blockSignals(true);
	if(_format == "DEGREES")
	{
		angleSpinBox->setValue(Angle::_RadiansToDegrees(angleSpinBox->value()));

	}
	else if(_format == "RADIANS")
	{
		angleSpinBox->setValue(Angle::_DegreesToRadians(angleSpinBox->value()));
	}
	angleSpinBox->blockSignals(false);
}




void QuaternionCreator::ConvertEulers(QString _format)
{
	pitchSpinBox->blockSignals(true);
	yawSpinBox->blockSignals(true);
	rollSpinBox->blockSignals(true);

	if(_format == "DEGREES")
	{
		pitchSpinBox->setValue(Angle::_RadiansToDegrees(pitchSpinBox->value()));
		yawSpinBox->setValue(Angle::_RadiansToDegrees(yawSpinBox->value()));
		rollSpinBox->setValue(Angle::_RadiansToDegrees(rollSpinBox->value()));
	}
	else if(_format == "RADIANS")
	{
		pitchSpinBox->setValue(Angle::_DegreesToRadians(pitchSpinBox->value()));
		yawSpinBox->setValue(Angle::_DegreesToRadians(yawSpinBox->value()));
		rollSpinBox->setValue(Angle::_DegreesToRadians(rollSpinBox->value()));
	}
	pitchSpinBox->blockSignals(false);
	yawSpinBox->blockSignals(false);
	rollSpinBox->blockSignals(false);
}



void QuaternionCreator::ChangeAngleFormat(QString _value)
{
	ConvertAngle(_value);
	ConvertEulers(_value);
}



void QuaternionCreator::AxisAngleIsChanged(double _value)
{  
	Vector3 axis = Vector3(axisXSpinBox->value(), axisYSpinBox->value(), axisZSpinBox->value());
	axis.Normalize();

	if(angleFormatComboBox->itemText(angleFormatComboBox->currentIndex()) == "DEGREES")
	{
		engine->GetSphere()->SetRotation(Quaternion(AxisAngle(axis, Angle::_DegreesToRadians(angleSpinBox->value()))));

	}
	else if(angleFormatComboBox->itemText(angleFormatComboBox->currentIndex()) == "RADIANS")
	{
		engine->GetSphere()->SetRotation(Quaternion(AxisAngle(axis, angleSpinBox->value())));
	}
	Quaternion quaternion = engine->GetSphere()->GetRotation();
	quaternion.Normalize();
	
	qxSpinBox->setValue(quaternion.x);
	qySpinBox->setValue(quaternion.y);
	qzSpinBox->setValue(quaternion.z);
	qwSpinBox->setValue(quaternion.w);
	UpdateEuler(quaternion);
}
 


void QuaternionCreator::EulerAnglesIsChanged(double _value)
{  
	if(angleFormatComboBox->itemText(angleFormatComboBox->currentIndex()) == "DEGREES")
	{
		engine->GetSphere()->SetRotation(Quaternion(Euler(Angle::_DegreesToRadians(pitchSpinBox->value()), Angle::_DegreesToRadians(yawSpinBox->value()), Angle::_DegreesToRadians(rollSpinBox->value()))));
	}
	else if(angleFormatComboBox->itemText(angleFormatComboBox->currentIndex()) == "RADIANS")
	{
		engine->GetSphere()->SetRotation(Quaternion(Euler(pitchSpinBox->value(), yawSpinBox->value(), rollSpinBox->value())));
	}
	Quaternion quaternion = engine->GetSphere()->GetRotation();
	quaternion.Normalize();
	
	qxSpinBox->setValue(quaternion.x);
	qySpinBox->setValue(quaternion.y);
	qzSpinBox->setValue(quaternion.z);
	qwSpinBox->setValue(quaternion.w);
	UpdateAxisAngle(quaternion);
}


void QuaternionCreator::RightView(void)
{
	engine->SetRightView();
}


void QuaternionCreator::FrontView(void)
{
	engine->SetFrontView();
}


void QuaternionCreator::TopDownView(void)
{
	engine->SetTopDownView();
}


void QuaternionCreator::SetRelativeRotation(QString _text)
{
	relativeRotationLabel->setText(_text);
}


void QuaternionCreator::SelectRotationMode(QString _mode)
{
	if(_mode == "EULER_SCROLLBAR")
	{
		engine->SetEulerScrollbar();
	}
	else if(_mode == "SPHERICAL_SWING_TWIST")
	{
		engine->SetSphericalSwingTwist();
	}
	else if(_mode == "SPHERICAL_TWO_POLE")
	{
		engine->SetSphericalTwoPole();
	}
	else if(_mode == "SPHERICAL_FROM_TO")
	{
		engine->SetSphericalFromTo();
	}
}


void QuaternionCreator::ShowGlobalAxises(int _state)
{
	engine->ShowGlobalAxises(_state);
}



void QuaternionCreator::ShowGlobalSpins(int _state)
{
	engine->ShowGlobalSpins(_state);
}



Viewport* QuaternionCreator::GetViewport(void)const
{
	return viewport;
}



void QuaternionCreator::CreateEngine(void)
{
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.x = 800;
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.y = 600;
	def.rendererDef.realTimeWorkerDef.windowDef.position.x = viewport->x();
	def.rendererDef.realTimeWorkerDef.windowDef.position.y = viewport->y();
	def.rendererDef.realTimeWorkerDef.windowDef.clientWindow = viewport->GetHWND();
	def.rendererDef.realTimeWorkerDef.unsuspended = true;
	def.rendererDef.realTimeWorkerDef.windowDef.title = "quaternion viewer";

	engine = new QuaternionViewer(this);
	
	if(engine->Create(def))
	{
		 
		EnginePlay();
	}
}



void QuaternionCreator::EnginePause(void)
{
	intervalTimer->stop();
	disconnect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}


void QuaternionCreator::CallFrame(void)
{
	engine->Frame();
}


void QuaternionCreator::EnginePlay(void)
{
	intervalTimer->start(1.0f/60.0f);
		
	connect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}



void QuaternionCreator::CreateViewport(void)
{
	viewport = new Viewport(800, 600, 10, 20, viewportGroupBox);

	connect(viewport, SIGNAL(CursorEnter()), this, SLOT(ViewportCursorEnter())); 
    connect(viewport, SIGNAL(CursorLeave()), this, SLOT(ViewportCursorLeave())); 
}


Quaternion QuaternionCreator::GetQuaternion(void)const
{
	return Quaternion::_GetNormalize(Quaternion(qxSpinBox->value(), qySpinBox->value(), qzSpinBox->value(), qwSpinBox->value()));
}


void QuaternionCreator::UpdateAxisAngle(Quaternion _quaternion)
{
	AxisAngle axisAngle = _quaternion.ToAxisAngle();
	axisXSpinBox->blockSignals(true);
	axisYSpinBox->blockSignals(true);
	axisZSpinBox->blockSignals(true);
	angleSpinBox->blockSignals(true);
	axisXSpinBox->setValue(axisAngle.axis.x);
	axisYSpinBox->setValue(axisAngle.axis.y);
	axisZSpinBox->setValue(axisAngle.axis.z);
	if(angleFormatComboBox->itemText(angleFormatComboBox->currentIndex()) == "DEGREES")
	{
		angleSpinBox->setValue(Angle::_RadiansToDegrees(axisAngle.angle));

	}
	else if(angleFormatComboBox->itemText(angleFormatComboBox->currentIndex()) == "RADIANS")
	{
		angleSpinBox->setValue(axisAngle.angle);
	}
	axisXSpinBox->blockSignals(false);
	axisYSpinBox->blockSignals(false);
	axisZSpinBox->blockSignals(false);
	angleSpinBox->blockSignals(false);
}


void QuaternionCreator::UpdateEuler(Quaternion _quaternion)
{
	Euler euler = _quaternion.ToShortestEulerPitchYawRoll();
	pitchSpinBox->blockSignals(true);
	yawSpinBox->blockSignals(true);
	rollSpinBox->blockSignals(true);

	if(angleFormatComboBox->itemText(angleFormatComboBox->currentIndex()) == "DEGREES")
	{
		pitchSpinBox->setValue(Angle::_RadiansToDegrees(euler.pitch));
		yawSpinBox->setValue(Angle::_RadiansToDegrees(euler.yaw));
		rollSpinBox->setValue(Angle::_RadiansToDegrees(euler.roll));
	}
	else if(angleFormatComboBox->itemText(angleFormatComboBox->currentIndex()) == "RADIANS")
	{
		pitchSpinBox->setValue(euler.pitch);
		yawSpinBox->setValue(euler.yaw);
		rollSpinBox->setValue(euler.roll);
	}
	pitchSpinBox->blockSignals(false);
	yawSpinBox->blockSignals(false);
	rollSpinBox->blockSignals(false);

}


void QuaternionCreator::SetQuaternion(Quaternion _quaternion, bool _modifySphere)
{
	_quaternion.Normalize();
	qxSpinBox->setValue(_quaternion.x);
	qySpinBox->setValue(_quaternion.y);
	qzSpinBox->setValue(_quaternion.z);
	qwSpinBox->setValue(_quaternion.w);
	UpdateAxisAngle(_quaternion);
	UpdateEuler(_quaternion);
	
	if(_modifySphere)
	{
		engine->GetSphere()->SetRotation(_quaternion);
	}
}



void QuaternionCreator::ViewportCursorEnter(void)
{
	engine->ShowCursor();
}


void QuaternionCreator::ViewportCursorLeave(void)
{
	engine->HideCursor();
}


void QuaternionCreator::SetZoomValue(int _zoom)
{
	currentZoomLabel->setText("Zoom: " + QString::number(_zoom) + "%");
}


void QuaternionCreator::SetHome(void)
{
	SetQuaternion(Quaternion::_GetIdentity());

	engine->SetFrontView();
	engine->SetZoom100();
	
	currentZoomLabel->setText("Zoom: 100%");
}



QuaternionCreator::~QuaternionCreator(void)
{
	EnginePause();
 
	delete engine;
}


void QuaternionCreator::closeEvent(QCloseEvent *_event)
{
	EnginePause();
}



void QuaternionCreator::ShowDoc(void)
{
	QString path = (Editor::_GetWorkDir() + Editor::_GetDocsDir() + "QuaternionCreator.doc");

	QUrl url = QUrl::fromLocalFile(path);

	QDesktopServices::openUrl(url);
}


void QuaternionCreator::IsOk(void)
{
	accept();
}


