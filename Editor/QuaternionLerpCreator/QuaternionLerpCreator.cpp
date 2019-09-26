#include "QuaternionLerpCreator.h"
#include <Engine/Output/File/File.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Editor/FloatLerpCreator/FloatLerpCreator.h>
#include <Editor/Editor/Editor.h>


QuaternionLerpCreator::QuaternionLerpCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), fastCreatingEnable(false)
{
	intervalTimer = new QTimer(this);
	viewportGroupBox = new QGroupBox(this);
    viewportGroupBox->setGeometry(QRect(10, 10, 800 + 20, 600 + 30));
	viewportGroupBox->setTitle("Viewer:");


	CreateViewport();
	CreateEngine();
	nameLabel = new QLabel(this); 
	nameLabel->setGeometry(QRect(840+10+10, 10+10+30-30, 41, 20));
	nameLabel->setText("Name:");

	nameMaxLength = 256;

    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setGeometry(QRect(950-70+15+10, 10+10+30-30, 171-11, 20));
	nameLineEdit->setMaxLength(nameMaxLength);
	nameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
    connect(nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(QuaternionLerpNameIsChanged(QString)));
	floatLerpGroupBox = new QGroupBox(this);
    floatLerpGroupBox->setGeometry(QRect(840+10, 10+10+5+20+5, 184+40+45-50-50+100+100+50+20, 50+40+40-20-40-10));
	floatLerpGroupBox->setTitle("Float lerp:");
	fLerpNameLabel = new QLabel(floatLerpGroupBox); 
	fLerpNameLabel->setGeometry(QRect(10, 10+15, 41, 20));
	fLerpNameLabel->setText("Name:");


    fLerpNameLineEdit = new QLineEdit(floatLerpGroupBox);
    fLerpNameLineEdit->setGeometry(QRect(70-10-5, 10+15, 171-11, 20));
	fLerpNameLineEdit->setMaxLength(nameMaxLength);
	fLerpNameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	connect(fLerpNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(FloatLerpNameIsChanged(QString)));
	loadButton = new QPushButton(floatLerpGroupBox);
    loadButton->setGeometry(QRect(70-10-5+180-5, 10+15, 51+20-10+20, 21));
	loadButton->setFocus();
	loadButton->setText("Load");  
	connect(loadButton, SIGNAL(clicked()), this, SLOT(LoadFloatLerp()));
	qStartDataLabel = new QLabel(this);
    qStartDataLabel->setGeometry(QRect(840+10, 110+30+8-50-5+40+40, 111, 21));
	qStartDataLabel->setText("Start Q (X/Y/Z/W):");

	qStartXSpinBox = new FloatSpinBox(this);
    qStartXSpinBox->setGeometry(QRect(950+10, 110+30+8-50-5+40+40, 62, 22));
	qStartXSpinBox->setDisabled(true);
          
	qStartYSpinBox = new FloatSpinBox(this);
    qStartYSpinBox->setGeometry(QRect(1020+10, 110+30+8-50-5+40+40, 62, 22));
	qStartYSpinBox->setDisabled(true);

	qStartZSpinBox = new FloatSpinBox(this);
    qStartZSpinBox->setGeometry(QRect(1090+10, 110+30+8-50-5+40+40, 62, 22));
	qStartZSpinBox->setDisabled(true);
 
	qStartWSpinBox = new FloatSpinBox(this);
    qStartWSpinBox->setGeometry(QRect(1160+10, 110+30+8-50-5+40+40, 62, 22));
	qStartWSpinBox->setDisabled(true);
	startQImportButton = new QPushButton(this);
    startQImportButton->setGeometry(QRect(1160+70+10, 110+30+8-50-5+40+40, 60-10, 22));
	startQImportButton->setText("import");
	connect(startQImportButton, SIGNAL(clicked()), this, SLOT(StartQImport()));
	qTargetDataLabel = new QLabel(this);
    qTargetDataLabel->setGeometry(QRect(840+10, 170+30+12-50-5+40+40, 111, 21));
	qTargetDataLabel->setText("Target Q (X/Y/Z/W):");

	qTargetXSpinBox = new FloatSpinBox(this);
    qTargetXSpinBox->setGeometry(QRect(950+10, 170+30+12-50-5+40+40, 62, 22));
	qTargetXSpinBox->setDisabled(true);
          
	qTargetYSpinBox = new FloatSpinBox(this);
    qTargetYSpinBox->setGeometry(QRect(1020+10, 170+30+12-50-5+40+40, 62, 22));
	qTargetYSpinBox->setDisabled(true);

	qTargetZSpinBox = new FloatSpinBox(this);
    qTargetZSpinBox->setGeometry(QRect(1090+10, 170+30+12-50-5+40+40, 62, 22));
	qTargetZSpinBox->setDisabled(true);
 
	qTargetWSpinBox = new FloatSpinBox(this);
    qTargetWSpinBox->setGeometry(QRect(1160+10, 170+30+12-50-5+40+40, 62, 22));
	qTargetWSpinBox->setDisabled(true);
	targetQImportButton = new QPushButton(this);
    targetQImportButton->setGeometry(QRect(1160+70+10, 170+30+12-50-5+40+40, 60-10, 22));
	targetQImportButton->setText("import");
	connect(targetQImportButton, SIGNAL(clicked()), this, SLOT(TargetQImport()));
	qCurrentDataLabel = new QLabel(this);
    qCurrentDataLabel->setGeometry(QRect(840+10, 140+30+10-50-5+40+40, 111, 21));
	qCurrentDataLabel->setText("Current Q (X/Y/Z/W):");

	qCurrentXSpinBox = new FloatSpinBox(this);
    qCurrentXSpinBox->setGeometry(QRect(950+10, 140+30+10-50-5+40+40, 62, 22));
	qCurrentXSpinBox->setDisabled(true);
          
	qCurrentYSpinBox = new FloatSpinBox(this);
    qCurrentYSpinBox->setGeometry(QRect(1020+10, 140+30+10-50-5+40+40, 62, 22));
	qCurrentYSpinBox->setDisabled(true);

	qCurrentZSpinBox = new FloatSpinBox(this);
    qCurrentZSpinBox->setGeometry(QRect(1090+10, 140+30+10-50-5+40+40, 62, 22));
	qCurrentZSpinBox->setDisabled(true);
 
	qCurrentWSpinBox = new FloatSpinBox(this);
    qCurrentWSpinBox->setGeometry(QRect(1160+10, 140+30+10-50-5+40+40, 62, 22));
	qCurrentWSpinBox->setDisabled(true);
	viewGroupBox = new QGroupBox(this);
    viewGroupBox->setGeometry(QRect(840+10, 10+15+200+40+40-5-30, 184+40+45-50-50+100+100+50+20, 50+40+40-20+200+40+30-10-5+5));
	viewGroupBox->setTitle("View:");
	angleFormatLabel = new QLabel(viewGroupBox);
    angleFormatLabel->setGeometry(QRect(20+100+100+5, 10+15+200+40+40-200-80, 81, 22));
	angleFormatLabel->setText("Angle format:");
	

	angleFormatComboBox = new QComboBox(viewGroupBox);
    angleFormatComboBox->setGeometry(QRect(20+100+80+100+5, 10+15+200+40+40-200-80, 81, 22));
	angleFormatComboBox->addItem("DEGREES");
	angleFormatComboBox->addItem("RADIANS");
	connect(angleFormatComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ChangeAngleFormat(QString)));
	axisAngleGroupBox = new QGroupBox(viewGroupBox);
    axisAngleGroupBox->setGeometry(QRect(20+5, 10+40+200+40+40-200-80, 391-5, 91));
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
	eulerGroupBox = new QGroupBox(viewGroupBox);
    eulerGroupBox->setGeometry(QRect(20+5, 110+40+200+40+40-200-80, 391-5, 91-40));
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
	showLocalSpinsLabel = new QLabel(viewGroupBox);
    showLocalSpinsLabel->setGeometry(QRect(20+5, 290+200-30-5+40+40-200-80-10, 101, 41));
    showLocalSpinsLabel->setText("Show local spins:");
      
	showLocalSpinsCheckBox = new QCheckBox(viewGroupBox);
    showLocalSpinsCheckBox->setGeometry(QRect(20+90+5+10, 300+200-30-5+40+40-200-80-10, 21, 20));
	showLocalSpinsCheckBox->setChecked(true);
	connect(showLocalSpinsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ShowLocalSpins(int)));
	

	showGlobalSpinsLabel = new QLabel(viewGroupBox);
    showGlobalSpinsLabel->setGeometry(QRect(20+5, 330+200-30-5+40+40-200-80-10, 101, 41));
    showGlobalSpinsLabel->setText("Show global spins:");
      
	showGlobalSpinsCheckBox = new QCheckBox(viewGroupBox);
    showGlobalSpinsCheckBox->setGeometry(QRect(20+90+5+10, 340+200-30-5+40+40-200-80-10, 21, 20));
	showGlobalSpinsCheckBox->setChecked(true);
	connect(showGlobalSpinsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ShowGlobalSpins(int)));
	
        
	showGlobalAxisesLabel = new QLabel(viewGroupBox);
    showGlobalAxisesLabel->setGeometry(QRect(20+5, 370+200-30-10+40+40-200-80-10+3, 101, 41));
	showGlobalAxisesLabel->setText("Show global axises:");
	
	showGlobalAxisesCheckBox = new QCheckBox(viewGroupBox);
    showGlobalAxisesCheckBox->setGeometry(QRect(20+90+5+10, 380+200-30-10+40+40-200-80-10+3, 16, 21));
	showGlobalAxisesCheckBox->setChecked(true);
	connect(showGlobalAxisesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ShowGlobalAxises(int)));
	rotationModeLabel = new QLabel(viewGroupBox);
    rotationModeLabel->setGeometry(QRect(20+5, 260+200-30-5+40+40-200-80-10, 81, 21));
	rotationModeLabel->setText("Rotation mode:");

	rotationModeComboBox = new QComboBox(viewGroupBox);
    rotationModeComboBox->setGeometry(QRect(20+90+5, 260+200-30-5+40+40-200-80-10, 171, 22));
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

	QPixmap homeIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "QuaternionLerpCreator/home.png");
	homeButton->setFixedSize(homeIcon.size());
	homeButton->setIcon(homeIcon);

	homeButton->setToolTip("Home");
	homeButton->setAutoRaise(true);
	connect(homeButton, SIGNAL(clicked()), this, SLOT(SetHome()));
	helpButton = new QToolButton(this);
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	helpButton->setGeometry(QRect(980-50+60, 570+60+20, 32, 32));
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
	startQButton = new QPushButton(this);
    startQButton->setGeometry(QRect(690-20-60-60-60-30-110-100-60-30-40-50-50, 560+60+30, 60, 32));
	startQButton->setText("Start");
	connect(startQButton, SIGNAL(clicked()), this, SLOT(StartQ()));
	startQButton->setStyleSheet("background-color: rgb(255, 100, 100);");
	targetQButton = new QPushButton(this);
    targetQButton->setGeometry(QRect(690-20-60-60-60-30-110-100-60-30-40-50+20, 560+60+30, 60, 32));
	targetQButton->setText("Target");
	connect(targetQButton, SIGNAL(clicked()), this, SLOT(TargetQ()));
	targetQButton->setStyleSheet("background-color: rgb(255, 100, 100);");
	playButton = new QPushButton(this);
    playButton->setGeometry(QRect(690-20-60-60-60-30-110-100-60-30-40+20+20, 560+60+30, 60, 32));
	playButton->setText("Play");
	connect(playButton, SIGNAL(clicked()), this, SLOT(Play()));
	stopButton = new QPushButton(this);
    stopButton->setGeometry(QRect(690-20-60-60-60-30-110-100-60-20+20+20+20, 560+60+30, 60, 32));
	stopButton->setText("Stop");
	connect(stopButton, SIGNAL(clicked()), this, SLOT(Stop()));
	okButton = new QPushButton(this);
    okButton->setGeometry(QRect(1100+60, 570+60+20, 101, 31));
	okButton->setText("Ok");
	connect(okButton, SIGNAL(clicked()), this, SLOT(IsOk()));
	cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(980+60, 570+60+20, 101, 31));
	cancelButton->setText("Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setFocus();

	setWindowTitle("Quaternion Lerp Editor");
    setFixedSize(1232+80, 616+60+30);
}


void QuaternionLerpCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}



void QuaternionLerpCreator::CommonFloatLerpEnabled(bool _enable)
{
	if(_enable)
	{
		{
		}
	}
}


void QuaternionLerpCreator::PrivateFloatLerpEnabled(bool _enable)
{
	if(_enable)
	{
		{
		}
	}
}

void QuaternionLerpCreator::FloatLerpNameIsChanged(QString _name)
{
	if(engine->GetQuaternionLerp()->GetFloatLerp())
	{
		engine->GetQuaternionLerp()->GetFloatLerp()->Rename(fLerpNameLineEdit->text().toAscii().data());
	}
}


void QuaternionLerpCreator::QuaternionLerpNameIsChanged(QString _name)
{
	engine->GetQuaternionLerp()->Rename(nameLineEdit->text().toAscii().data());

	if(engine->GetSphere()->IsStartLerpValid() && engine->GetSphere()->IsTargetLerpValid())
	{
		engine->UpdateTriggers();
	}
}



void QuaternionLerpCreator::StartQImport(void)
{
	QString path = QFileDialog::getOpenFileName(this,
												"Choose quaternion variable",
												(AssetLibrary::_GetDir() +  AbstractVariable::_GetDir()).c_str(),
												"*." + QString(AbstractVariable::_GetFileSuffix().c_str()));

	if(path.count() > 0)
	{
		AbstractVariable* var;

		if(var = AbstractVariable::_LoadFromFile(path.toAscii().data(), false))
		{
			if(AbstractVariable::_GetValueType<Quaternion>() == var->GetValueType())
			{
				Quaternion q = dynamic_cast<Variable<Quaternion>*>(var)->GetValue();

				startQButton->setStyleSheet("background-color: rgb(100, 255, 100);");

				engine->SetStartQ(q);
				SetStartQ(q);
			}
		}
	}
}


void QuaternionLerpCreator::TargetQImport(void)
{
	QString path = QFileDialog::getOpenFileName(this,
												"Choose quaternion variable",
												(AssetLibrary::_GetDir() +  AbstractVariable::_GetDir()).c_str(),
												"*." + QString(AbstractVariable::_GetFileSuffix().c_str()));

	if(path.count() > 0)
	{
		AbstractVariable* var;

		if(var = AbstractVariable::_LoadFromFile(path.toAscii().data(), false))
		{
			if(AbstractVariable::_GetValueType<Quaternion>() == var->GetValueType())
			{
				Quaternion q = dynamic_cast<Variable<Quaternion>*>(var)->GetValue();

				targetQButton->setStyleSheet("background-color: rgb(100, 255, 100);");

				engine->SetTargetQ(q);
				SetTargetQ(q);
			}
		}
	}
}


void QuaternionLerpCreator::EditFloatLerp(void)
{ 
}




void QuaternionLerpCreator::LoadFloatLerp(void)
{
	QString path = QFileDialog::getOpenFileName(this,
												"Choose float lerp",
												(AssetLibrary::_GetDir() +  FloatLerp::_GetDir()).c_str(),
												"*." + QString(FloatLerp::_GetFileSuffix().c_str()));

	if(path.count() > 0)
	{
			
		QFileInfo fileInfo(path);
		QString fileName = fileInfo.baseName();
		 
		ReloadFloatLerp(fileName); 
	}
} 



void QuaternionLerpCreator::ReloadFloatLerp(QString _name)
{
	engine->GetQuaternionLerp()->BindFloatLerp(NIL);

	if(File::_IsExist(AssetLibrary::_GetDir() + FloatLerp::_GetDir() + _name.toAscii().data() + StringANSI(".") + FloatLerp::_GetFileSuffix()))
	{
		FloatLerp* fLerp = FloatLerp::_LoadFromFile(_name.toAscii().data());

		if(fLerp)
		{
			engine->GetQuaternionLerp()->BindFloatLerp(fLerp);

			fLerpNameLineEdit->setText(engine->GetQuaternionLerp()->GetFloatLerp()->GetName().c_str());
			engine->GetScene()->AddToUpdateList(engine->GetQuaternionLerp()->GetFloatLerp());
		
			bool playingState = fLerp->IsPlaying();
			if(engine->GetSphere()->IsStartLerpValid() && engine->GetSphere()->IsTargetLerpValid())
			{
				engine->ShowCurveFromStartToCurrent();
			}
			if(!playingState) { fLerp->Pause(); }
		}
	}
}


void QuaternionLerpCreator::SetStartQ(Quaternion _quaternion)
{
	qStartXSpinBox->setValue(_quaternion.x);
	qStartYSpinBox->setValue(_quaternion.y);
	qStartZSpinBox->setValue(_quaternion.z);
	qStartWSpinBox->setValue(_quaternion.w);
}



void QuaternionLerpCreator::SetTargetQ(Quaternion _quaternion)
{
	qTargetXSpinBox->setValue(_quaternion.x);
	qTargetYSpinBox->setValue(_quaternion.y);
	qTargetZSpinBox->setValue(_quaternion.z);
	qTargetWSpinBox->setValue(_quaternion.w);
}


void QuaternionLerpCreator::TargetQ(void)
{
	targetQButton->setStyleSheet("background-color: rgb(100, 255, 100);");

	engine->SetTargetQ(); 

	SetTargetQ(GetQuaternion()); 
		
	
	if(engine->GetQuaternionLerp()->GetFloatLerp())
	{
		bool playingState = engine->GetQuaternionLerp()->GetFloatLerp()->IsPlaying();
		if(engine->GetSphere()->IsStartLerpValid() && engine->GetSphere()->IsTargetLerpValid())
		{
			engine->ShowCurveFromStartToCurrent();
		}
		if(!playingState) { engine->GetQuaternionLerp()->GetFloatLerp()->Pause(); }
	}
}


void QuaternionLerpCreator::StartQ(void)
{
	startQButton->setStyleSheet("background-color: rgb(100, 255, 100);");

	engine->SetStartQ(); 

	SetStartQ(GetQuaternion());

		
	if(engine->GetQuaternionLerp()->GetFloatLerp())
	{
		bool playingState = engine->GetQuaternionLerp()->GetFloatLerp()->IsPlaying();
		if(engine->GetSphere()->IsStartLerpValid() && engine->GetSphere()->IsTargetLerpValid())
		{
			engine->ShowCurveFromStartToCurrent();
		}
		if(!playingState) { engine->GetQuaternionLerp()->GetFloatLerp()->Pause(); }
	}
}




void QuaternionLerpCreator::Stop(void)
{
	engine->Stop();
	playButton->setText("Play");
}


void QuaternionLerpCreator::PrepareToPlay(void)
{
	if(playButton->text() != "Play")
	{
		playButton->setText("Play"); 
	}
}


void QuaternionLerpCreator::PrepareToPause(void)
{
	if(playButton->text() != "Pause")
	{
		playButton->setText("Pause");
	}
}


void QuaternionLerpCreator::Play(void)
{ 
	if(engine->GetQuaternionLerp()->GetFloatLerp())
	{
		if(engine->GetQuaternionLerp()->GetFloatLerp()->IsPlaying())
		{
			engine->Pause();
		}
		else
		{
			if(engine->GetQuaternionLerp()->GetFloatLerp()->GetCurrent() == engine->GetQuaternionLerp()->GetFloatLerp()->GetTarget())
			{
				engine->Stop();
				engine->Play(); 
			}
			else
			{
				engine->Play();
			}
		}
	}
}


void QuaternionLerpCreator::SetObjectName(QString _name)
{
	nameLineEdit->setText(_name);
}


void QuaternionLerpCreator::SetNameDisable(bool _disable)
{
	nameLineEdit->setDisabled(_disable);
}


void QuaternionLerpCreator::ConvertAngle(QString _format)
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




void QuaternionLerpCreator::ConvertEulers(QString _format)
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



void QuaternionLerpCreator::ChangeAngleFormat(QString _value)
{
	ConvertAngle(_value);
	ConvertEulers(_value);
}



void QuaternionLerpCreator::AxisAngleIsChanged(double _value)
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
	
	qCurrentXSpinBox->setValue(quaternion.x);
	qCurrentYSpinBox->setValue(quaternion.y);
	qCurrentZSpinBox->setValue(quaternion.z);
	qCurrentWSpinBox->setValue(quaternion.w);
	UpdateEuler(quaternion);
}
 


void QuaternionLerpCreator::EulerAnglesIsChanged(double _value)
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
	
	qCurrentXSpinBox->setValue(quaternion.x);
	qCurrentYSpinBox->setValue(quaternion.y);
	qCurrentZSpinBox->setValue(quaternion.z);
	qCurrentWSpinBox->setValue(quaternion.w);
	UpdateAxisAngle(quaternion);
}


void QuaternionLerpCreator::RightView(void)
{
	engine->SetRightView();
}


void QuaternionLerpCreator::FrontView(void)
{
	engine->SetFrontView();
}


void QuaternionLerpCreator::TopDownView(void)
{
	engine->SetTopDownView();
}


void QuaternionLerpCreator::SetRelativeRotation(QString _text)
{
	relativeRotationLabel->setText(_text);
}


void QuaternionLerpCreator::SelectRotationMode(QString _mode)
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


void QuaternionLerpCreator::ShowGlobalAxises(int _state)
{
	engine->ShowGlobalAxises(_state);
}



void QuaternionLerpCreator::ShowLocalSpins(int _state)
{
	engine->ShowLocalSpins(_state);
}


void QuaternionLerpCreator::ShowGlobalSpins(int _state)
{
	engine->ShowGlobalSpins(_state);
}



Viewport* QuaternionLerpCreator::GetViewport(void)const
{
	return viewport;
}



void QuaternionLerpCreator::CreateEngine(void)
{
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.x = 800;
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.y = 600;
	def.rendererDef.realTimeWorkerDef.windowDef.position.x = viewport->x();
	def.rendererDef.realTimeWorkerDef.windowDef.position.y = viewport->y();
	def.rendererDef.realTimeWorkerDef.windowDef.clientWindow = viewport->GetHWND();
	def.rendererDef.realTimeWorkerDef.unsuspended = true;


	def.rendererDef.realTimeWorkerDef.windowDef.title = "quaternion viewer";

	engine = new QuaternionLerpViewer(this);
	
	if(engine->Create(def))
	{
		 
		EnginePlay();
	}
}



void QuaternionLerpCreator::EnginePause(void)
{
	intervalTimer->stop();
	disconnect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}


void QuaternionLerpCreator::CallFrame(void)
{
	engine->Frame();
}


void QuaternionLerpCreator::EnginePlay(void)
{
	intervalTimer->start(1.0f/60.0f);
		
	connect(intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}



void QuaternionLerpCreator::CreateViewport(void)
{
	viewport = new Viewport(800, 600, 10, 20, viewportGroupBox);

	connect(viewport, SIGNAL(CursorEnter()), this, SLOT(ViewportCursorEnter())); 
    connect(viewport, SIGNAL(CursorLeave()), this, SLOT(ViewportCursorLeave())); 
}


Quaternion QuaternionLerpCreator::GetQuaternion(void)const
{
	return Quaternion::_GetNormalize(Quaternion(qCurrentXSpinBox->value(), qCurrentYSpinBox->value(), qCurrentZSpinBox->value(), qCurrentWSpinBox->value()));
}


void QuaternionLerpCreator::UpdateAxisAngle(Quaternion _quaternion)
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


void QuaternionLerpCreator::UpdateEuler(Quaternion _quaternion)
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


QuaternionLerp* QuaternionLerpCreator::GetQuaternionLerp(void)
{
	return engine->GetQuaternionLerp();
}



void QuaternionLerpCreator::SetQuaternionLerp(QString _name)
{
	delete engine->GetQuaternionLerp()->GetFloatLerp();
	if(engine->GetQuaternionLerp()->LoadFromFile(_name.toAscii().data()))
	{
		engine->UpdateStartTargetQ();
		engine->GetScene()->AddToUpdateList(engine->GetQuaternionLerp()->GetFloatLerp());
	}
	SetStartQ(engine->GetQuaternionLerp()->GetFrom()); 
	SetTargetQ(engine->GetQuaternionLerp()->GetTo());
	nameLineEdit->blockSignals(true);
	nameLineEdit->setText(engine->GetQuaternionLerp()->GetName().c_str());
	nameLineEdit->blockSignals(false);
	if(engine->GetQuaternionLerp()->GetFloatLerp())
	{
		fLerpNameLineEdit->setText(engine->GetQuaternionLerp()->GetFloatLerp()->GetName().c_str());
	}
	{
	}
	startQButton->setStyleSheet("background-color: rgb(100, 255, 100);");
	targetQButton->setStyleSheet("background-color: rgb(100, 255, 100);");

	if(engine->GetSphere()->IsStartLerpValid() && engine->GetSphere()->IsTargetLerpValid())
	{
		engine->UpdateTriggers();
	}

	engine->ShowCurveFromStartToCurrent();

	setWindowTitle("Quaternion Lerp Editor");
}



void QuaternionLerpCreator::SetQuaternion(Quaternion _quaternion, bool _modifySphere)
{
	_quaternion.Normalize();
	qCurrentXSpinBox->setValue(_quaternion.x);
	qCurrentYSpinBox->setValue(_quaternion.y);
	qCurrentZSpinBox->setValue(_quaternion.z);
	qCurrentWSpinBox->setValue(_quaternion.w);
	UpdateAxisAngle(_quaternion);
	UpdateEuler(_quaternion);
	
	if(_modifySphere)
	{
		engine->GetSphere()->SetRotation(_quaternion);
	}
}



void QuaternionLerpCreator::ViewportCursorEnter(void)
{
	engine->ShowCursor();
}


void QuaternionLerpCreator::ViewportCursorLeave(void)
{
	engine->HideCursor();
}


void QuaternionLerpCreator::SetZoomValue(int _zoom)
{
	currentZoomLabel->setText("Zoom: " + QString::number(_zoom) + "%");
}


void QuaternionLerpCreator::SetHome(void)
{
	SetQuaternion(Quaternion::_GetIdentity());

	engine->SetFrontView();
	engine->SetZoom100();
	
	currentZoomLabel->setText("Zoom: 100%");
}



QuaternionLerpCreator::~QuaternionLerpCreator(void)
{
	EnginePause();
 
	delete engine;
}


void QuaternionLerpCreator::closeEvent(QCloseEvent *_event)
{
	EnginePause();
}



void QuaternionLerpCreator::ShowDoc(void)
{
	QString path = (Editor::_GetWorkDir() + Editor::_GetDocsDir() + "QuaternionLerpCreator.doc");

	QUrl url = QUrl::fromLocalFile(path);

	QDesktopServices::openUrl(url);
}


void QuaternionLerpCreator::IsOk(void)
{
	{
		int32 errorCode;
		if(nameLineEdit->text().isEmpty())
		{
			ErrorDialog("Error", "Quaternion lerp name is not defined", 155+30, 40);
			return;
		}
		if((errorCode = BooleanExprParser::_IsNameValid(nameLineEdit->text().toAscii().data())) >= 0)
		{
			ErrorDialog("Error", "Quaternion lerp name is not valid: '" + QString(nameLineEdit->text().at(errorCode)) + "'", 190, 40);
			return;
		} 
	}
	if(engine->GetQuaternionLerp()->GetFloatLerp() && engine->GetQuaternionLerp()->GetFloatLerp()->GetName().compare(nameLineEdit->text().toAscii().data()) == 0)
	{
		ErrorDialog("Error", "Name ' " + nameLineEdit->text() + "' is already reserved", 160, 40);
		return;
	}
	if(File::_IsExist(AssetLibrary::_GetDir() + QuaternionLerp::_GetDir() + StringANSI(nameLineEdit->text().toAscii().data()) + "." + QuaternionLerp::_GetFileSuffix()))
	{ 
		WarningDialog warning("Warning", "Quaternion lerp '" +  nameLineEdit->text() + "' is already exist. Rewrite file?", 200, 90);
		
		if(warning.exec() == QDialog::Rejected)
		{
			return;
		}
	}
	

	if(!fastCreatingEnable)
	{
		if(engine->GetQuaternionLerp()->GetFloatLerp())
		{
			AssetLibrary::_GetAssets().AddObject(engine->GetQuaternionLerp()->GetFloatLerp());
		}

		engine->GetQuaternionLerp()->SaveToFile();

		EnginePause();
		delete engine->GetQuaternionLerp();
	}
	else
	{

		if(AssetLibrary::_GetAssets().AddObject(engine->GetQuaternionLerp()) == NIL)
		{
			ErrorDialog("Error", "Name '" + nameLineEdit->text() + "' is already reserved", 195, 40);
			return;
		}
		else
		{

			if(engine->GetQuaternionLerp()->GetFloatLerp())
			{
				AssetLibrary::_GetAssets().AddObject(engine->GetQuaternionLerp()->GetFloatLerp());
			}
		}
	}

	accept();
}


