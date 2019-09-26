#include "TextureCreator.h"
#include <Engine/Output/File/File.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Parser/BooleanExprParser/BooleanExprParser.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Editor/PointsSizeDialog/PointsSizeDialog.h> 
#include <Editor/LinesWidthDialog/LinesWidthDialog.h>
QString TextureCreator::startDir = "";



TextureCreator::TextureCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), fastCreatingEnable(false), shapeNumber(0), verticesShowEnable(true), edgesShowEnable(true), quadShowEnable(true), gizmoXYShowEnable(true), gridShowEnable(false), vectorLerpShowCurveEnable(true), saveCurveDialog(NIL), saveShapeDialog(NIL), importShapeDialog(NIL), editorMode(TEXTURE_EDITOR)
{
	nameMaxLength = 256;
	texture = new Texture2D();
	texturedAnimation = new TexturedAnimation();
	atlas = new Atlas2D();


	CreateViewport();
	CreateEngine();
	CreateMenu();

	backgroundColor = engine->GetBackgroundColor();
    loadTextureButton = new QPushButton(this);
    loadTextureButton->setText("Load");
    loadTextureButton->setGeometry(QRect(10, 10+20, 81, 23));
	loadTextureButton->setFocus();
    connect(loadTextureButton, SIGNAL(clicked()), this, SLOT(LoadTexture()));
	nameLabel = new QLabel(this);
    nameLabel->setText("Name:");
    nameLabel->setGeometry(QRect(110, 10+20, 31, 21));
	
	nameLineEdit = new QLineEdit(this);
    nameLineEdit->setGeometry(QRect(150, 10+20, 261-60, 20));
    nameLineEdit->setMaxLength(nameMaxLength);
	nameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	connect(nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged()));
	tabWidget = new QTabWidget(this);
    tabWidget->setGeometry(QRect(10, 120-55, 401+30, 521+55+10));
	connect(tabWidget, SIGNAL(currentChanged(int)), SLOT(CurrentTabIsChanged(int)));
	propertiesTab = new QWidget();
	tabWidget->addTab(propertiesTab, "Properties");


	CreateHitboxTab();

	CreateVectorLerpTab();

	CreateTileboxTab();
	homeButton = new QToolButton(this);
	QPixmap homeIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TextureCreator/home.png");
	homeButton->setIcon(homeIcon);
	homeButton->setFixedSize(homeIcon.size());
	homeButton->move(930-10, 610+40+10);
	homeButton->setToolTip("Home");
	homeButton->setAutoRaise(true);
	connect(homeButton, SIGNAL(clicked()), this, SLOT(SetHome()));
	currentZoomScaleLabel = new QLabel(this);
    currentZoomScaleLabel->setGeometry(QRect(970-10, 610+40+10+5, 81, 21));
	currentZoomScaleLabel->setText("Zoom: 100%");
	worldCursorPositionLabel = new QLabel(this);
    worldCursorPositionLabel->setGeometry(QRect(970-10-150, 610+40+10+5, 81+20, 21));
	worldCursorPositionLabel->setText("x: 0; y: 0");
	amountOfPointsLabel = new QLabel(this);
    amountOfPointsLabel->setGeometry(QRect(970-10-150-90, 610+40+10+5, 81+50, 21));
	amountOfPointsLabel->setText("Points: 0");  


	SetHome();
	helpButton = new QToolButton(this);
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	helpButton->setGeometry(QRect(1080-7, 610+40+10, 32, 32));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
	cancelButton = new QPushButton(this);
	cancelButton->setText("Cancel");
	cancelButton->setFocusPolicy(Qt::NoFocus);
    cancelButton->setGeometry(QRect(1110, 610+40+10, 101, 31));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    okButton = new QPushButton(this);
    okButton->setText("Ok");
	okButton->setFocusPolicy(Qt::NoFocus);
    okButton->setGeometry(QRect(1220, 610+40+10, 101, 31));
	okButton->setEnabled(false);
    setFixedSize(1341, 665+20+20);
} 


void TextureCreator::CreateTileboxTab(void)
{
    tileboxTab = new QWidget();
    tabWidget->addTab(tileboxTab, "Tilebox");

	tabWidget->setCurrentIndex(0);
}



void TextureCreator::CreateHitboxTab(void)
{
    hitboxTab = new QWidget();
	tabWidget->addTab(hitboxTab, "Hitbox");
	hitboxNameLabel = new QLabel(hitboxTab);
    hitboxNameLabel->setGeometry(QRect(10, 20, 61, 21));
	hitboxNameLabel->setText("Name:");

	hitboxNameLineEdit = new QLineEdit(hitboxTab); 
    hitboxNameLineEdit->setGeometry(QRect(120-20, 16, 131, 21));
	hitboxNameLineEdit->setMaxLength(nameMaxLength);
	hitboxNameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
 	connect(hitboxNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(HitboxNameIsChanged(QString)));
	brushLabel = new QLabel(hitboxTab);
    brushLabel->setGeometry(QRect(10, 50, 61, 21));
	brushLabel->setText("Brush:");

	brushComboBox = new QComboBox(hitboxTab);
    brushComboBox->setGeometry(QRect(120-20, 46, 131, 21));
	brushComboBox->addItem("POLYGON");
	brushComboBox->addItem("CIRCLE");
	brushComboBox->addItem("QUAD");
	brushComboBox->setCurrentIndex(0);
	connect(brushComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(BrushIsChanged(QString)));
	buildModeLabel = new QLabel(hitboxTab);
    buildModeLabel->setGeometry(QRect(10, 80, 61, 21));
	buildModeLabel->setText("Build mode:");
    
	buildModeComboBox = new QComboBox(hitboxTab);
    buildModeComboBox->setGeometry(QRect(120-20, 76, 131, 21));
	buildModeComboBox->addItem("SPLIT");
	buildModeComboBox->addItem("DELAUNAY");
	connect(buildModeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ChangeBuildMode(QString)));
	buildModeComboBox->setCurrentIndex(0);
	hitboxEnableLabel = new QLabel(hitboxTab);
    hitboxEnableLabel->setGeometry(QRect(310+30, 10, 51, 31));
	hitboxEnableLabel->setText("Enable:");

	hitboxEnableCheckBox = new QCheckBox(hitboxTab);
    hitboxEnableCheckBox->setGeometry(QRect(360+30, 10, 31, 31));
	hitboxEnableCheckBox->setChecked(false);
    connect(hitboxEnableCheckBox, SIGNAL(stateChanged(int)), this, SLOT(HitboxIsChanged(int)));
	circleSegmentsLabel = new QLabel(hitboxTab);
    circleSegmentsLabel->setGeometry(QRect(310-40-5+30, 46, 51, 21));
	circleSegmentsLabel->setText("Segments:");
        
	circleSegmentsSpinBox = new IntSpinBox(hitboxTab);
    circleSegmentsSpinBox->setGeometry(QRect(360-40+30, 46, 101-25-15, 22));
	circleSegmentsSpinBox->setRange(3, Numerical<int32>::_UpperValue());
	circleSegmentsSpinBox->setToolTip(QString::number(3) + " - " + QString::number(Numerical<int32>::_UpperValue()));
	circleSegmentsSpinBox->setEnabled(false);
	circleSegmentsSpinBox->setValue(36);
	lockLabel = new QLabel(hitboxTab);
    lockLabel->setGeometry(QRect(310+30, 40+30, 51, 31));
	lockLabel->setText("Lock:");

	lockCheckBox = new QCheckBox(hitboxTab);
    lockCheckBox->setGeometry(QRect(360+30, 40+30, 31, 31)); 
	lockCheckBox->setEnabled(false);
	connect(lockCheckBox, SIGNAL(stateChanged(int)), this, SLOT(SetShapeLock(int)));
	shapeList = new TextureShapeListWidget(hitboxTab);
    shapeList->setGeometry(QRect(10, 110, 371+30, 341+70));
	connect(shapeList, SIGNAL(itemSelectionChanged()), this, SLOT(ChangeSelectShape()));
    connect(shapeList, SIGNAL(SaveShapeAs()), this, SLOT(SaveShapeAs()));
    connect(shapeList, SIGNAL(ImportShape()), this, SLOT(ImportShape()));
	connect(shapeList, SIGNAL(RemoveShape()), this, SLOT(RemoveSelectedShape()));
	addShapeToolButton = new QToolButton(hitboxTab);
    addShapeToolButton->setGeometry(QRect(10, 460+70, 21, 21));
	QPixmap addShapeIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TextureCreator/add.png");
	addShapeToolButton->setIcon(addShapeIcon);
	addShapeToolButton->setToolTip("Add shape");
	addShapeToolButton->setEnabled(false);
	addShapeToolButton->setAutoRaise(true);
	connect(addShapeToolButton, SIGNAL(clicked()), this, SLOT(CreateShape()));
	removeShapeToolButton = new QToolButton(hitboxTab);
    removeShapeToolButton->setGeometry(QRect(40, 460+70, 21, 21));
	QPixmap removeShapeIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TextureCreator/remove.png");
	removeShapeToolButton->setIcon(removeShapeIcon);
	removeShapeToolButton->setToolTip("Remove shape");
	removeShapeToolButton->setAutoRaise(true);
	connect(removeShapeToolButton, SIGNAL(clicked()), this, SLOT(RemoveSelectedShape()));
	createSubpointsToolButton = new QToolButton(hitboxTab);
    createSubpointsToolButton->setGeometry(QRect(40+40, 460+70, 70, 21));
	QPixmap subpointIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TextureCreator/sub-point.png");
	createSubpointsToolButton->setIconSize(QSize(70, 16));
	createSubpointsToolButton->setIcon(subpointIcon);
	createSubpointsToolButton->setToolTip("Create sub-points");
	createSubpointsToolButton->setAutoRaise(true);
	connect(createSubpointsToolButton, SIGNAL(clicked()), this, SLOT(CreateSubPoints()));
	changePivotPushButton = new QPushButton(hitboxTab);
    changePivotPushButton->setGeometry(QRect(170+30, 460+70, 51, 21));
	changePivotPushButton->setText("Pivot");
	changePivotPushButton->setCheckable(true);
	changePivotPushButton->setToolTip("Change pivot");
	connect(changePivotPushButton, SIGNAL(clicked()), this, SLOT(ChangePivot()));
	importHitboxPushButton = new QPushButton(hitboxTab);
    importHitboxPushButton->setGeometry(QRect(170+70+30, 460+70, 51, 21));
	importHitboxPushButton->setText("Import");
	importHitboxPushButton->setToolTip("Import hitbox");
	connect(importHitboxPushButton, SIGNAL(clicked()), this, SLOT(ImportHitbox()));
	hitboxSaveAsPushButton = new QPushButton(hitboxTab);
    hitboxSaveAsPushButton->setGeometry(QRect(240+70+30, 460+70, 61, 21));
	hitboxSaveAsPushButton->setText("Save as...");
	hitboxSaveAsPushButton->setToolTip("Save hitbox as...");
	connect(hitboxSaveAsPushButton, SIGNAL(clicked()), this, SLOT(SaveHitboxAs()));
}


void TextureCreator::HitboxNameIsChanged(QString _name)
{
	if(editorMode == TEXTURED_ANIMATION_EDITOR)
	{
		if(atlas->IsExist())
		{
			hitboxNames.Get(animationCurrentFrameSpinBox->value() - 1)->key = hitboxNameLineEdit->text();
		}
	}
}


void TextureCreator::CreateVectorLerpTab(void)
{
    vectorLerpTab = new QWidget();
    tabWidget->addTab(vectorLerpTab, "Vector lerp");
    vectorLerpNameLabel = new QLabel(vectorLerpTab);
	vectorLerpNameLabel->setText("Name:"); 
    vectorLerpNameLabel->setGeometry(QRect(20, 20, 61, 21));
        
	vectorLerpNameLineEdit = new QLineEdit(vectorLerpTab);
    vectorLerpNameLineEdit->setGeometry(QRect(80, 20, 181, 20));
	vectorLerpNameLineEdit->setMaxLength(nameMaxLength);
	vectorLerpNameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	connect(vectorLerpNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(VectorLerpNameIsChanged()));
	vectorLerpImportPushButton = new QPushButton(vectorLerpTab);
    vectorLerpImportPushButton->setGeometry(QRect(270, 20, 51+5, 21));
	vectorLerpImportPushButton->setText("Import");
	vectorLerpImportPushButton->setToolTip("Import vector lerp");
	connect(vectorLerpImportPushButton, SIGNAL(clicked()), this, SLOT(ImportVectorLerp()));
	vectorLerpSaveAsPushButton = new QPushButton(vectorLerpTab);
    vectorLerpSaveAsPushButton->setGeometry(QRect(330-2+6, 20, 61+5, 21));
	vectorLerpSaveAsPushButton->setText("Save as...");
	vectorLerpSaveAsPushButton->setToolTip("Save vector lerp as...");
	connect(vectorLerpSaveAsPushButton, SIGNAL(clicked()), this, SLOT(SaveVectorLerpAs()));
	vectorLerpModeLabel = new QLabel(vectorLerpTab);
    vectorLerpModeLabel->setGeometry(QRect(20, 50, 61, 21)); 
	vectorLerpModeLabel->setText("Lerp mode:");

	vectorLerpModeComboBox = new QComboBox(vectorLerpTab);
    vectorLerpModeComboBox->setGeometry(QRect(80, 50, 121, 22));
    vectorLerpModeComboBox->addItem("BAZIER");
    vectorLerpModeComboBox->addItem("LAGRANGE");
	vectorLerpModeComboBox->addItem("RING");
	vectorLerpModeComboBox->setCurrentIndex(GetVectorLerpModeIndex());
	connect(vectorLerpModeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ChangeVectorLerpMode(QString)));
	vectorLerpSegmentsLabel = new QLabel(vectorLerpTab);
    vectorLerpSegmentsLabel->setGeometry(QRect(20, 80, 61, 21));
	vectorLerpSegmentsLabel->setText("Segments:");
        
	vectorLerpSegmentsSpinBox = new IntSpinBox(vectorLerpTab);
    vectorLerpSegmentsSpinBox->setGeometry(QRect(80, 80, 101-10, 22));   
	vectorLerpSegmentsSpinBox->setRange(1, Numerical<int32>::_UpperValue());
	vectorLerpSegmentsSpinBox->setToolTip(QString::number(1) + " - " + QString::number(Numerical<int32>::_UpperValue()));
	vectorLerpSegmentsSpinBox->setValue(engine->GetVectorLerp()->GetAmountOfSegments());
	connect(vectorLerpSegmentsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ChangeVectorLerpAmountOfSegments(int)));
	vectorLerpSaveCurvePushButton = new QPushButton(vectorLerpTab);
    vectorLerpSaveCurvePushButton->setGeometry(QRect(200-15-4, 80, 70, 22)); 
	vectorLerpSaveCurvePushButton->setText("Save curve"); 
	connect(vectorLerpSaveCurvePushButton, SIGNAL(clicked()), this, SLOT(VectorLerpSaveCurveAs()));
	vectorLerpCurrentLabel = new QLabel(vectorLerpTab);
    vectorLerpCurrentLabel->setText("Current V:");
    vectorLerpCurrentLabel->setGeometry(QRect(20, 110, 61, 21));
        	
	vectorLerpCurrentXSpinBox = new FloatSpinBox(vectorLerpTab);
    vectorLerpCurrentXSpinBox->setGeometry(QRect(80, 110, 71, 22));
	vectorLerpCurrentXSpinBox->setEnabled(false);

	vectorLerpCurrentYSpinBox = new FloatSpinBox(vectorLerpTab);
    vectorLerpCurrentYSpinBox->setGeometry(QRect(160, 110, 71, 22));
	vectorLerpCurrentYSpinBox->setEnabled(false);

    vectorLerpCurrentZSpinBox = new FloatSpinBox(vectorLerpTab);
    vectorLerpCurrentZSpinBox->setGeometry(QRect(240, 110, 71, 22));
	vectorLerpCurrentZSpinBox->setEnabled(false);
	vectorLerpDeltaLabel = new QLabel(vectorLerpTab);
    vectorLerpDeltaLabel->setText("Delta V:");
    vectorLerpDeltaLabel->setGeometry(QRect(20, 140, 61, 21));
        	
	vectorLerpDeltaXSpinBox = new FloatSpinBox(vectorLerpTab);
    vectorLerpDeltaXSpinBox->setGeometry(QRect(80, 140, 71, 22));
	vectorLerpDeltaXSpinBox->setEnabled(false);

	vectorLerpDeltaYSpinBox = new FloatSpinBox(vectorLerpTab);
    vectorLerpDeltaYSpinBox->setGeometry(QRect(160, 140, 71, 22));
	vectorLerpDeltaYSpinBox->setEnabled(false);

    vectorLerpDeltaZSpinBox = new FloatSpinBox(vectorLerpTab);
    vectorLerpDeltaZSpinBox->setGeometry(QRect(240, 140, 71, 22));
	vectorLerpDeltaZSpinBox->setEnabled(false);
    vectorLerpFloatLerpLabel = new QLabel(vectorLerpTab);
    vectorLerpFloatLerpLabel->setGeometry(QRect(150+15, 180-20-25+40, 81, 21));
	vectorLerpFloatLerpLabel->setText("Float lerp:");
    vectorLerpFloatLerpScrollArea = new QScrollArea(vectorLerpTab);
    vectorLerpFloatLerpScrollArea->setGeometry(QRect(5, 200-20-25+40, 386+30, 291+85+25-40));
	vectorLerpFloatLerpScrollAreaWidget = new QWidget(vectorLerpFloatLerpScrollArea);
    vectorLerpFloatLerpScrollAreaWidget->setGeometry(QRect(0, 0, 386-20+30, 291+85+25+30));
	vectorLerpFloatLerpScrollArea->setWidget(vectorLerpFloatLerpScrollAreaWidget);
	vectorLerpFloatLerpNameLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpNameLabel->setGeometry(QRect(20, 10, 61, 21));
	vectorLerpFloatLerpNameLabel->setText("Name:");
	
	vectorLerpFloatLerpNameLineEdit = new QLineEdit(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpNameLineEdit->setGeometry(QRect(70+40, 10, 171, 21)); 
	vectorLerpFloatLerpNameLineEdit->setMaxLength(nameMaxLength);
	vectorLerpFloatLerpNameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
    connect(vectorLerpFloatLerpNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(VectorLerpFloatLerpNameIsChanged()));
	vectorLerpFloatLerpImportPushButton = new QPushButton(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpImportPushButton->setGeometry(QRect(250+43, 10, 71-4, 21));
	vectorLerpFloatLerpImportPushButton->setText("Import");
	vectorLerpFloatLerpImportPushButton->setToolTip("Import float lerp");
	connect(vectorLerpFloatLerpImportPushButton, SIGNAL(clicked()), this, SLOT(ImportVectorLerpFloatLerp()));
	vectorLerpFloatLerpSaveAsPushButton = new QPushButton(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpSaveAsPushButton->setGeometry(QRect(250+43, 10+30, 71-4, 21));
	vectorLerpFloatLerpSaveAsPushButton->setText("Save as..."); 
	vectorLerpFloatLerpSaveAsPushButton->setToolTip("Save float lerp as...");
	connect(vectorLerpFloatLerpSaveAsPushButton, SIGNAL(clicked()), this, SLOT(SaveVectorLerpFloatLerpAs()));
	vectorLerpFloatLerpTargetLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpTargetLabel->setGeometry(QRect(20, 40, 71, 21));
	vectorLerpFloatLerpTargetLabel->setText("Target:");

    vectorLerpFloatLerpTargetSpinBox = new FloatSpinBox(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpTargetSpinBox->setGeometry(QRect(110, 40, 81, 22)); 
	vectorLerpFloatLerpTargetSpinBox->setToolTip("0.001 - " + QString::number(Numerical<float>::_UpperValue()));
	vectorLerpFloatLerpTargetSpinBox->setDecimals(3);
	vectorLerpFloatLerpTargetSpinBox->setRange(0.001f, Numerical<float>::_UpperValue());
	vectorLerpFloatLerpTargetSpinBox->setSingleStep(1.0f);
	vectorLerpFloatLerpTargetSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetTarget());
	connect(vectorLerpFloatLerpTargetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VectorLerpFloatLerpTargetIsChanged(double)));
	vectorLerpFloatLerpCurrentLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpCurrentLabel->setGeometry(QRect(20, 70, 71, 21));
	vectorLerpFloatLerpCurrentLabel->setText("Current:");

	vectorLerpFloatLerpCurrentSpinBox = new FloatSpinBox(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpCurrentSpinBox->setGeometry(QRect(110, 70, 81, 22));
	vectorLerpFloatLerpCurrentSpinBox->setRange(0.0f, engine->GetVectorLerp()->GetFloatLerp()->GetTarget());
	vectorLerpFloatLerpCurrentSpinBox->setToolTip("0.0 - " + QString::number(engine->GetVectorLerp()->GetFloatLerp()->GetTarget()));
	connect(vectorLerpFloatLerpCurrentSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VectorLerpFloatLerpCurrentIsChanged(double)));
	vectorLerpFloatLerpTLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);  
    vectorLerpFloatLerpTLabel->setGeometry(QRect(20, 100, 71, 21));
	vectorLerpFloatLerpTLabel->setText("T:");

	vectorLerpFloatLerpTSpinBox = new FloatSpinBox(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpTSpinBox->setGeometry(QRect(110, 100, 81, 22)); 
	vectorLerpFloatLerpTSpinBox->setToolTip("0.0 - 1.0");
	vectorLerpFloatLerpTSpinBox->setDecimals(3);
	vectorLerpFloatLerpTSpinBox->setRange(0.0f, 1.0f);
	connect(vectorLerpFloatLerpTSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VectorLerpFloatLerpTIsChanged(double)));
	vectorLerpFloatLerpDeltaLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpDeltaLabel->setGeometry(QRect(20, 130, 71, 21));
	vectorLerpFloatLerpDeltaLabel->setText("Delta:");
	    
	vectorLerpFloatLerpDeltaSpinBox = new FloatSpinBox(vectorLerpFloatLerpScrollAreaWidget);    
    vectorLerpFloatLerpDeltaSpinBox->setGeometry(QRect(110, 130, 81, 22));
	vectorLerpFloatLerpDeltaSpinBox->setEnabled(false);
	vectorLerpFloatLerpModeLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpModeLabel->setGeometry(QRect(20, 160, 81, 21));
	vectorLerpFloatLerpModeLabel->setText("Lerp mode:");
	  
	vectorLerpFloatLerpModeComboBox = new QComboBox(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpModeComboBox->setGeometry(QRect(110, 160, 141, 22));
	vectorLerpFloatLerpModeComboBox->addItem("UNITS_PER_SECOND");
	vectorLerpFloatLerpModeComboBox->addItem("FIXED_TIME");
	vectorLerpFloatLerpModeComboBox->addItem("PARABOLA");
	vectorLerpFloatLerpModeComboBox->addItem("DAMPING");
	vectorLerpFloatLerpModeComboBox->addItem("MAGNETIC");
	vectorLerpFloatLerpModeComboBox->setCurrentIndex(engine->GetVectorLerp()->GetFloatLerp()->GetLerpMode());
	connect(vectorLerpFloatLerpModeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(VectorLerpFloatLerpModeIsChanged(QString)));
	vectorLerpFloatLerpPlayingSpeedLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpPlayingSpeedLabel->setGeometry(QRect(20, 190, 71, 21));
	vectorLerpFloatLerpPlayingSpeedLabel->setText("Playing speed:");

	vectorLerpFloatLerpPlayingSpeedSpinBox = new FloatSpinBox(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpPlayingSpeedSpinBox->setGeometry(QRect(110, 190, 81, 22));
    vectorLerpFloatLerpPlayingSpeedSpinBox->setToolTip("0.001 - " + QString::number(Numerical<float>::_UpperValue()));
	vectorLerpFloatLerpPlayingSpeedSpinBox->setDecimals(3);
	vectorLerpFloatLerpPlayingSpeedSpinBox->setRange(0.001f, Numerical<float>::_UpperValue());
	vectorLerpFloatLerpPlayingSpeedSpinBox->setSingleStep(1.0f);
	vectorLerpFloatLerpPlayingSpeedSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingSpeed());
	connect(vectorLerpFloatLerpPlayingSpeedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VectorLerpFloatLerpPlayingSpeedIsChanged(double)));
	vectorLerpFloatLerpPlayingDurationLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpPlayingDurationLabel->setGeometry(QRect(20, 220, 91, 21));
	vectorLerpFloatLerpPlayingDurationLabel->setText("Playing duration:");

    vectorLerpFloatLerpPlayingDurationSpinBox = new FloatSpinBox(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpPlayingDurationSpinBox->setGeometry(QRect(110, 220, 81, 22));
    vectorLerpFloatLerpPlayingDurationSpinBox->setToolTip("0.001 - " + QString::number(Numerical<float>::_UpperValue()) + " (sec)");
	vectorLerpFloatLerpPlayingDurationSpinBox->setDecimals(3);
	vectorLerpFloatLerpPlayingDurationSpinBox->setRange(0.001f, Numerical<float>::_UpperValue());
	vectorLerpFloatLerpPlayingDurationSpinBox->setSingleStep(1.0f);
	vectorLerpFloatLerpPlayingDurationSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingDurationInSec());
	connect(vectorLerpFloatLerpPlayingDurationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VectorLerpFloatLerpPlayingDurationIsChanged(double)));
	vectorLerpFloatLerpElapsedTimeLabel = new QLabel(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpElapsedTimeLabel->setGeometry(QRect(20, 220+30, 91, 21));
	vectorLerpFloatLerpElapsedTimeLabel->setText("Elapsed time:");

    vectorLerpFloatLerpElapsedTimeSpinBox = new FloatSpinBox(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpElapsedTimeSpinBox->setGeometry(QRect(110, 220+30, 81, 22));
	vectorLerpFloatLerpElapsedTimeSpinBox->setDecimals(3);
	vectorLerpFloatLerpElapsedTimeSpinBox->setSingleStep(1.0f);
	vectorLerpFloatLerpElapsedTimeSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetElapsedTimeInSec());
	connect(vectorLerpFloatLerpElapsedTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(VectorLerpFloatLerpElapsedTimeIsChanged(double)));
	vectorLerpFloatLerpPlayingModeGroupBox = new QGroupBox(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpPlayingModeGroupBox->setGeometry(QRect(10, 255+30, 241, 131));
	vectorLerpFloatLerpPlayingModeGroupBox->setTitle("Playing mode:");
	QButtonGroup* group1 = new QButtonGroup(vectorLerpFloatLerpPlayingModeGroupBox);
	QButtonGroup* group2 = new QButtonGroup(vectorLerpFloatLerpPlayingModeGroupBox);
	QButtonGroup* group3 = new QButtonGroup(vectorLerpFloatLerpPlayingModeGroupBox);
	vectorLerpFloatLerpForwardRadioButton = new QRadioButton(vectorLerpFloatLerpPlayingModeGroupBox);
    vectorLerpFloatLerpForwardRadioButton->setGeometry(QRect(30, 30, 71, 21));
	vectorLerpFloatLerpForwardRadioButton->setText("Forward");
	connect(vectorLerpFloatLerpForwardRadioButton, SIGNAL(toggled(bool)), this, SLOT(VectorLerpFloatLerpForwardPlayingModeEnable(bool)));
	group1->addButton(vectorLerpFloatLerpForwardRadioButton);
	vectorLerpFloatLerpBackwardRadioButton = new QRadioButton(vectorLerpFloatLerpPlayingModeGroupBox);
    vectorLerpFloatLerpBackwardRadioButton->setGeometry(QRect(140, 30, 71, 21));
	vectorLerpFloatLerpBackwardRadioButton->setText("Backward");
    connect(vectorLerpFloatLerpBackwardRadioButton, SIGNAL(toggled(bool)), this, SLOT(VectorLerpFloatLerpBackwardPlayingModeEnable(bool)));
	group1->addButton(vectorLerpFloatLerpBackwardRadioButton);
	vectorLerpFloatLerpPingPongRadioButton = new QRadioButton(vectorLerpFloatLerpPlayingModeGroupBox);
    vectorLerpFloatLerpPingPongRadioButton->setGeometry(QRect(30, 60, 71, 21));
	vectorLerpFloatLerpPingPongRadioButton->setText("Ping pong");
    connect(vectorLerpFloatLerpPingPongRadioButton, SIGNAL(toggled(bool)), this, SLOT(VectorLerpFloatLerpPingPongPlayingModeEnable(bool)));
	group2->addButton(vectorLerpFloatLerpPingPongRadioButton);
	vectorLerpFloatLerpOnewayRadioButton = new QRadioButton(vectorLerpFloatLerpPlayingModeGroupBox);
    vectorLerpFloatLerpOnewayRadioButton->setGeometry(QRect(140, 60, 71, 21));
	vectorLerpFloatLerpOnewayRadioButton->setText("Oneway");
    connect(vectorLerpFloatLerpOnewayRadioButton, SIGNAL(toggled(bool)), this, SLOT(VectorLerpFloatLerpOnewayPlayingModeEnable(bool)));
	group2->addButton(vectorLerpFloatLerpOnewayRadioButton);
	vectorLerpFloatLerpSingleShotRadioButton = new QRadioButton(vectorLerpFloatLerpPlayingModeGroupBox);
    vectorLerpFloatLerpSingleShotRadioButton->setGeometry(QRect(140, 90, 81, 21));
	vectorLerpFloatLerpSingleShotRadioButton->setText("Single shot");
 	connect(vectorLerpFloatLerpSingleShotRadioButton, SIGNAL(toggled(bool)), this, SLOT(VectorLerpFloatLerpSingleShotPlayingModeEnable(bool)));
	group3->addButton(vectorLerpFloatLerpSingleShotRadioButton);
	vectorLerpFloatLerpLoopRadioButton = new QRadioButton(vectorLerpFloatLerpPlayingModeGroupBox);
    vectorLerpFloatLerpLoopRadioButton->setGeometry(QRect(30, 90, 71, 21));
	vectorLerpFloatLerpLoopRadioButton->setText("Loop");
    connect(vectorLerpFloatLerpLoopRadioButton, SIGNAL(toggled(bool)), this, SLOT(VectorLerpFloatLerpLoopPlayingModeEnable(bool)));
	group3->addButton(vectorLerpFloatLerpLoopRadioButton);
    

	vectorLerpFloatLerpForwardRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::FORWARD_PLAYING);
	vectorLerpFloatLerpBackwardRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::BACKWARD_PLAYING);
	vectorLerpFloatLerpPingPongRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::PING_PONG_PLAYING);
	vectorLerpFloatLerpOnewayRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::ONEWAY_PLAYING);
	vectorLerpFloatLerpLoopRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::LOOP_PLAYING);
	vectorLerpFloatLerpSingleShotRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::SINGLE_SHOT_PLAYING);
	vectorLerpFloatLerpPlayPushButton = new QPushButton(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpPlayPushButton->setGeometry(QRect(270+10, 360-70, 61, 23));
    vectorLerpFloatLerpPlayPushButton->setText("Play");
	connect(vectorLerpFloatLerpPlayPushButton, SIGNAL(clicked()), this, SLOT(VectorLerpFloatLerpPlay()));
	vectorLerpFloatLerpStopPushButton = new QPushButton(vectorLerpFloatLerpScrollAreaWidget);
    vectorLerpFloatLerpStopPushButton->setGeometry(QRect(270+10, 400-70, 61, 23)); 
	vectorLerpFloatLerpStopPushButton->setText("Stop");
	connect(vectorLerpFloatLerpStopPushButton, SIGNAL(clicked()), this, SLOT(VectorLerpFloatLerpStop()));        
}


TextureCreator::~TextureCreator(void)
{
	EnginePause();

	delete engine;


	delete viewMenu;
	delete pointsMenu;
	delete linesMenu;

	delete spriteMenu;
	delete cameraMenu;

	delete vectorLerpMenu;
	delete gizmoMenu;
	delete gridMenu;
}


void TextureCreator::SetVectorLerpCurrent(Vector3 _value)
{
	vectorLerpCurrentXSpinBox->setValue(_value.x);
	vectorLerpCurrentYSpinBox->setValue(_value.y);
	vectorLerpCurrentZSpinBox->setValue(_value.z);
}


void TextureCreator::SetVectorLerpDelta(Vector3 _value)
{
	vectorLerpDeltaXSpinBox->setValue(_value.x);
	vectorLerpDeltaYSpinBox->setValue(_value.y);
	vectorLerpDeltaZSpinBox->setValue(_value.z);
}


int32 TextureCreator::GetCircleSegments(void)const
{
	return circleSegmentsSpinBox->value();
}



void TextureCreator::SetVectorLerpFloatLerpCurrent(float _value)
{ 
	vectorLerpFloatLerpCurrentSpinBox->blockSignals(true);
	vectorLerpFloatLerpCurrentSpinBox->setValue(_value);
	vectorLerpFloatLerpCurrentSpinBox->blockSignals(false);
}


void TextureCreator::SetVectorLerpFloatLerpDelta(float _value)
{
	vectorLerpFloatLerpDeltaSpinBox->setValue(_value);
}


void TextureCreator::SetVectorLerpFloatLerpT(float _value)
{
	vectorLerpFloatLerpTSpinBox->blockSignals(true);
	vectorLerpFloatLerpTSpinBox->setValue(_value);
	vectorLerpFloatLerpTSpinBox->blockSignals(false);
}


void TextureCreator::UpdateVectorLerpFloatLerpElapsedTime(float _value)
{
	vectorLerpFloatLerpElapsedTimeSpinBox->blockSignals(true);
	vectorLerpFloatLerpElapsedTimeSpinBox->setValue(_value);
	vectorLerpFloatLerpElapsedTimeSpinBox->blockSignals(false);
}



void TextureCreator::TexturedAnimationPositionOffsetEnable(int _enable)
{
	texturedAnimation->SetPositionOffsetEnable(_enable);
}



void TextureCreator::UpdateAnimationElapsedTime(float _value) 
{
	animationElapsedTimeSpinBox->blockSignals(true);
	animationElapsedTimeSpinBox->setValue(_value);
	animationElapsedTimeSpinBox->blockSignals(false);
}



void TextureCreator::UpdateVectorLerpFloatLerpInfo(void)
{
	vectorLerpFloatLerpNameLineEdit->blockSignals(true);
	vectorLerpFloatLerpNameLineEdit->setText(engine->GetVectorLerp()->GetFloatLerp()->GetName().c_str());
	vectorLerpFloatLerpNameLineEdit->blockSignals(false);
	vectorLerpFloatLerpTargetSpinBox->blockSignals(true);
	vectorLerpFloatLerpTargetSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetTarget());
	vectorLerpFloatLerpTargetSpinBox->blockSignals(false);
	vectorLerpFloatLerpCurrentSpinBox->blockSignals(true);
	vectorLerpFloatLerpCurrentSpinBox->setRange(0.0f, engine->GetVectorLerp()->GetFloatLerp()->GetTarget());
	vectorLerpFloatLerpCurrentSpinBox->setToolTip("0.0 - " + QString::number(engine->GetVectorLerp()->GetFloatLerp()->GetTarget()));
	vectorLerpFloatLerpCurrentSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetCurrent());
	vectorLerpFloatLerpCurrentSpinBox->blockSignals(false);
	vectorLerpFloatLerpTSpinBox->blockSignals(true);
	vectorLerpFloatLerpTSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetT());
	vectorLerpFloatLerpTSpinBox->blockSignals(false);
	vectorLerpFloatLerpDeltaSpinBox->blockSignals(true);
	vectorLerpFloatLerpDeltaSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetDelta());
	vectorLerpFloatLerpDeltaSpinBox->blockSignals(false);
	vectorLerpFloatLerpModeComboBox->blockSignals(true);
	vectorLerpFloatLerpModeComboBox->setCurrentIndex(engine->GetVectorLerp()->GetFloatLerp()->GetLerpMode());
	vectorLerpFloatLerpModeComboBox->blockSignals(false);
	vectorLerpFloatLerpPlayingSpeedSpinBox->blockSignals(true);
	vectorLerpFloatLerpPlayingSpeedSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingSpeed());
	vectorLerpFloatLerpPlayingSpeedSpinBox->blockSignals(false);
	vectorLerpFloatLerpPlayingDurationSpinBox->blockSignals(true);
	vectorLerpFloatLerpPlayingDurationSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingDurationInSec());
	vectorLerpFloatLerpPlayingDurationSpinBox->blockSignals(false);
	vectorLerpFloatLerpElapsedTimeSpinBox->blockSignals(true);
	vectorLerpFloatLerpElapsedTimeSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetElapsedTimeInSec());
	vectorLerpFloatLerpElapsedTimeSpinBox->blockSignals(false);
	vectorLerpFloatLerpForwardRadioButton->blockSignals(true);
	vectorLerpFloatLerpBackwardRadioButton->blockSignals(true);
	vectorLerpFloatLerpPingPongRadioButton->blockSignals(true);
	vectorLerpFloatLerpOnewayRadioButton->blockSignals(true);
	vectorLerpFloatLerpLoopRadioButton->blockSignals(true);
	vectorLerpFloatLerpSingleShotRadioButton->blockSignals(true);

	vectorLerpFloatLerpForwardRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::FORWARD_PLAYING);
	vectorLerpFloatLerpBackwardRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::BACKWARD_PLAYING);
	vectorLerpFloatLerpPingPongRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::PING_PONG_PLAYING);
	vectorLerpFloatLerpOnewayRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::ONEWAY_PLAYING);
	vectorLerpFloatLerpLoopRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::LOOP_PLAYING);
	vectorLerpFloatLerpSingleShotRadioButton->setChecked(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::SINGLE_SHOT_PLAYING);

	vectorLerpFloatLerpForwardRadioButton->blockSignals(false);
	vectorLerpFloatLerpBackwardRadioButton->blockSignals(false);
	vectorLerpFloatLerpPingPongRadioButton->blockSignals(false);
	vectorLerpFloatLerpOnewayRadioButton->blockSignals(false);
	vectorLerpFloatLerpLoopRadioButton->blockSignals(false);
	vectorLerpFloatLerpSingleShotRadioButton->blockSignals(false);
}



void TextureCreator::VectorLerpFloatLerpPlay(void)
{
	if(engine->GetVectorLerp()->GetFloatLerp()->IsPlaying())
	{
		engine->GetVectorLerp()->GetFloatLerp()->Pause(); 
	}
	else
	{
		if(engine->GetVectorLerp()->GetFloatLerp()->GetCurrent() == engine->GetVectorLerp()->GetFloatLerp()->GetTarget() ||
		   ((engine->GetVectorLerp()->GetFloatLerp()->GetPlayingMode() & PlayingObject::BACKWARD_PLAYING) && engine->GetVectorLerp()->GetFloatLerp()->GetCurrent() == 0.0f))
		{
			engine->GetVectorLerp()->GetFloatLerp()->Stop();
			engine->GetVectorLerp()->GetFloatLerp()->Play(); 
		}
		else
		{
			engine->GetVectorLerp()->GetFloatLerp()->Play();
		}
	}
}


void TextureCreator::VectorLerpFloatLerpStop(void)
{
	engine->GetVectorLerp()->GetFloatLerp()->Stop();
	engine->GetVectorLerpCurrentVertex()->SetPosition(engine->GetVectorLerp()->GetCurrent());
	vectorLerpFloatLerpPlayPushButton->setText("Play");
	vectorLerpCurrentXSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().x);
	vectorLerpCurrentYSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().y);
	vectorLerpCurrentZSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().z);
	vectorLerpDeltaXSpinBox->setValue(engine->GetVectorLerp()->GetDelta().x);
	vectorLerpDeltaYSpinBox->setValue(engine->GetVectorLerp()->GetDelta().y);
	vectorLerpDeltaZSpinBox->setValue(engine->GetVectorLerp()->GetDelta().z);
	vectorLerpFloatLerpCurrentSpinBox->blockSignals(true);
	vectorLerpFloatLerpCurrentSpinBox->setRange(0.0f, engine->GetVectorLerp()->GetFloatLerp()->GetTarget());
	vectorLerpFloatLerpCurrentSpinBox->setToolTip("0.0 - " + QString::number(engine->GetVectorLerp()->GetFloatLerp()->GetTarget()));
	vectorLerpFloatLerpCurrentSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetCurrent());
	vectorLerpFloatLerpCurrentSpinBox->blockSignals(false);
	vectorLerpFloatLerpTSpinBox->blockSignals(true);
	vectorLerpFloatLerpTSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetT());
	vectorLerpFloatLerpTSpinBox->blockSignals(false);
	vectorLerpFloatLerpDeltaSpinBox->blockSignals(true);
	vectorLerpFloatLerpDeltaSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetDelta());
	vectorLerpFloatLerpDeltaSpinBox->blockSignals(false);
	vectorLerpFloatLerpElapsedTimeSpinBox->blockSignals(true);
	vectorLerpFloatLerpElapsedTimeSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetElapsedTimeInSec());
	vectorLerpFloatLerpElapsedTimeSpinBox->blockSignals(false);
}



void TextureCreator::VectorLerpFloatLerpForwardPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		engine->GetVectorLerp()->GetFloatLerp()->SetPlayingMode(PlayingObject::FORWARD_PLAYING);
	}
}




void TextureCreator::VectorLerpFloatLerpBackwardPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		engine->GetVectorLerp()->GetFloatLerp()->SetPlayingMode(PlayingObject::BACKWARD_PLAYING);
	}
}



void TextureCreator::VectorLerpFloatLerpPingPongPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		engine->GetVectorLerp()->GetFloatLerp()->SetPlayingMode(PlayingObject::PING_PONG_PLAYING);
	}
}


void TextureCreator::VectorLerpFloatLerpOnewayPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		engine->GetVectorLerp()->GetFloatLerp()->SetPlayingMode(PlayingObject::ONEWAY_PLAYING);
	}
}



void TextureCreator::VectorLerpFloatLerpLoopPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		engine->GetVectorLerp()->GetFloatLerp()->SetPlayingMode(PlayingObject::LOOP_PLAYING);
	}
}


void TextureCreator::VectorLerpFloatLerpSingleShotPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		engine->GetVectorLerp()->GetFloatLerp()->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING);
	}
}



void TextureCreator::BrushIsChanged(QString _name)
{
	if(_name == "QUAD")
	{
		circleSegmentsSpinBox->setEnabled(false);

		ChangeSelectShape();
	}
	else if(_name == "CIRCLE")
	{
		circleSegmentsSpinBox->setEnabled(true);

		ChangeSelectShape();
	}
	else
	{
		circleSegmentsSpinBox->setEnabled(false);
	}
}



void TextureCreator::SetTextureFiltering(int32 _mode)
{
	textureFilteringComboBox->blockSignals(true);

	switch(_mode)
	{
		case Texture2D::POINT_FILTERING:
		{
			textureFilteringComboBox->setCurrentIndex(0);
			break;
		}
				
		case Texture2D::BILINEAR_FILTERING:
		{
			textureFilteringComboBox->setCurrentIndex(1);
			break;
		}

		case Texture2D::TRILINEAR_FILTERING:
		{
			textureFilteringComboBox->setCurrentIndex(2);
			break;
		}

		case Texture2D::ANISOTROPY_FILTERING:
		{
			textureFilteringComboBox->setCurrentIndex(3);
			break;
		}
	}
	textureFilteringComboBox->blockSignals(false);
}




void TextureCreator::TextureFilteringIsChanged(QString _name)
{
	if(_name == "POINT")
	{
		texture->SetFilteringMode(Texture2D::POINT_FILTERING);
	}
	else if(_name == "BILINEAR")
	{
		texture->SetFilteringMode(Texture2D::BILINEAR_FILTERING);
	}
	else if(_name == "TRILINEAR")
	{
		texture->SetFilteringMode(Texture2D::TRILINEAR_FILTERING);
	}
	else if(_name == "ANISOTROPY")
	{
		texture->SetFilteringMode(Texture2D::ANISOTROPY_FILTERING);
	}
}



void TextureCreator::SetAtlasFiltering(int32 _mode)
{
	atlasFilteringComboBox->blockSignals(true);

	switch(_mode)
	{
		case Texture2D::POINT_FILTERING:
		{
			atlasFilteringComboBox->setCurrentIndex(0);
			break;
		}
				
		case Texture2D::BILINEAR_FILTERING:
		{
			atlasFilteringComboBox->setCurrentIndex(1);
			break;
		}

		case Texture2D::TRILINEAR_FILTERING:
		{
			atlasFilteringComboBox->setCurrentIndex(2);
			break;
		}

		case Texture2D::ANISOTROPY_FILTERING:
		{
			atlasFilteringComboBox->setCurrentIndex(3);
			break;
		}
	}
	atlasFilteringComboBox->blockSignals(false);
}



void TextureCreator::AtlasFilteringIsChanged(QString _name)
{
	if(_name == "POINT")
	{ 
		atlas->SetFilteringMode(Texture2D::POINT_FILTERING);
	}
	else if(_name == "BILINEAR")
	{
		atlas->SetFilteringMode(Texture2D::BILINEAR_FILTERING);
	}
	else if(_name == "TRILINEAR")
	{
		atlas->SetFilteringMode(Texture2D::TRILINEAR_FILTERING);
	}
	else if(_name == "ANISOTROPY")
	{
		atlas->SetFilteringMode(Texture2D::ANISOTROPY_FILTERING);
	}
}


void TextureCreator::VectorLerpFloatLerpModeIsChanged(QString _mode)
{ 
	engine->GetVectorLerp()->GetFloatLerp()->SetLerpMode(GetFloatLerpModeIndex(vectorLerpFloatLerpModeComboBox->itemText(vectorLerpFloatLerpModeComboBox->currentIndex())));

	vectorLerpFloatLerpPlayingDurationSpinBox->blockSignals(true);
    vectorLerpFloatLerpPlayingDurationSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingDurationInSec());
	vectorLerpFloatLerpPlayingDurationSpinBox->blockSignals(false);
	
	vectorLerpFloatLerpPlayingDurationSpinBox->setEnabled(IsVectorLerpFloatLerpPlayingDurationSupported());
	vectorLerpFloatLerpElapsedTimeSpinBox->setEnabled(IsVectorLerpFloatLerpPlayingDurationSupported());
}


bool TextureCreator::IsVectorLerpFloatLerpPlayingDurationSupported(void)const
{
	QString mode = vectorLerpFloatLerpModeComboBox->itemText(vectorLerpFloatLerpModeComboBox->currentIndex());

	if(mode == "UNITS_PER_SECOND")
	{
		return true;
	}
	else if(mode == "FIXED_TIME")
	{
		return true;
	}
	return false;
}



void TextureCreator::PrepareVectorLerpFloatLerpToPlay(void)
{
	if(vectorLerpFloatLerpPlayPushButton->text() != "Play")
	{
		vectorLerpFloatLerpPlayPushButton->setText("Play"); 
	}
}


void TextureCreator::PrepareVectorLerpFloatLerpToPause(void)
{
	if(vectorLerpFloatLerpPlayPushButton->text() != "Pause")
	{
		vectorLerpFloatLerpPlayPushButton->setText("Pause");
	}
}

int32 TextureCreator::GetFloatLerpModeIndex(QString _mode)
{
	if(_mode == "UNITS_PER_SECOND")
	{
		return FloatLerp::UNITS_PER_SECOND;
	}
	else if(_mode == "FIXED_TIME")
	{
		return FloatLerp::FIXED_TIME;
	}
	else if(_mode == "PARABOLA")
	{
		return FloatLerp::PARABOLA;
	}
	else if(_mode == "DAMPING")
	{
		return FloatLerp::DAMPING;
	}
	else if(_mode == "MAGNETIC")
	{
		return FloatLerp::MAGNETIC;
	}
	return -1;
}



void TextureCreator::VectorLerpFloatLerpTIsChanged(double _value)
{ 
	if(vectorLerpFloatLerpBackwardRadioButton->isChecked())
	{
		if(IsVectorLerpFloatLerpPlayingDurationSupported())
		{
			engine->GetVectorLerpCurrentVertex()->SetPosition(engine->GetVectorLerp()->GetNVertex(1.0f - _value));
		}
		else
		{
			engine->GetVectorLerpCurrentVertex()->SetPosition(engine->GetVectorLerp()->GetNVertex(_value));
		}
	}
	else
	{
		engine->GetVectorLerpCurrentVertex()->SetPosition(engine->GetVectorLerp()->GetNVertex(_value));
	}
}



void TextureCreator::VectorLerpFloatLerpTargetIsChanged(double _value)
{
	engine->GetVectorLerp()->GetFloatLerp()->SetTarget(_value);

	vectorLerpFloatLerpPlayingDurationSpinBox->blockSignals(true);
    vectorLerpFloatLerpPlayingDurationSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingDurationInSec());
	
	vectorLerpFloatLerpCurrentSpinBox->setRange(0.0f, engine->GetVectorLerp()->GetFloatLerp()->GetTarget());
	vectorLerpFloatLerpCurrentSpinBox->setToolTip("0.0 - " + QString::number(engine->GetVectorLerp()->GetFloatLerp()->GetTarget()));
	vectorLerpFloatLerpPlayingDurationSpinBox->blockSignals(false);
}


void TextureCreator::VectorLerpFloatLerpPlayingSpeedIsChanged(double _value)
{
	engine->GetVectorLerp()->GetFloatLerp()->SetPlayingSpeed(_value);
		
	vectorLerpFloatLerpPlayingDurationSpinBox->blockSignals(true);
	vectorLerpFloatLerpPlayingDurationSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingDurationInSec());
	vectorLerpFloatLerpPlayingDurationSpinBox->blockSignals(false);
}


void TextureCreator::VectorLerpFloatLerpPlayingDurationIsChanged(double _value)
{
	engine->GetVectorLerp()->GetFloatLerp()->SetPlayingDurationInSec(_value);

	vectorLerpFloatLerpPlayingSpeedSpinBox->blockSignals(true);
	vectorLerpFloatLerpPlayingSpeedSpinBox->setValue(engine->GetVectorLerp()->GetFloatLerp()->GetPlayingSpeed());
	vectorLerpFloatLerpPlayingSpeedSpinBox->blockSignals(false);
}



void TextureCreator::VectorLerpFloatLerpCurrentIsChanged(double _value)
{
	vectorLerpFloatLerpTSpinBox->setValue(_value / engine->GetVectorLerp()->GetFloatLerp()->GetPlayingDurationInSec());
}



 void TextureCreator::VectorLerpFloatLerpElapsedTimeIsChanged(double _value)
{
	vectorLerpFloatLerpTSpinBox->setValue(_value / engine->GetVectorLerp()->GetFloatLerp()->GetPlayingDurationInSec());
}



void TextureCreator::ImportPoints(void)
{
	QFileDialog dialog(this, "Choose vector3 list");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + AbstractList::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + AbstractList::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		StringANSI name = File::_GetBaseName(assets.at(0).toStdString());

		engine->LoadPoints(name);
	}
}



void TextureCreator::SavePointsAs(void)
{
	if(engine->GetPoints().IsNotEmpty())
	{
		QString name = "vertices";

		QString pathFile = QFileDialog::getSaveFileName(this,
														"Save vertices as...",
														(AssetLibrary::_GetDir() + AbstractList::_GetDir()).c_str() + name,
														QString("*") + AbstractList::_GetFileSuffix().c_str());

		if(!pathFile.isEmpty())
		{	
			QString name = File::_GetBaseName(pathFile.toStdString()).c_str();
			if(File::_IsExist(AssetLibrary::_GetDir() + AbstractList::_GetDir() + name.toStdString() + "." + AbstractList::_GetFileSuffix()))
			{ 
				WarningDialog warning("Warning", "List '" + name + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					return;
				}
			}

			engine->SavePoints(name.toStdString());
		}
	}
	else
	{
		ErrorDialog("Error", "Points is not defined", 145, 40);
	}
}



void TextureCreator::SaveCurveDialogFileFilterIsChanged(const QString& _filter)
{
	if(saveCurveDialog)
	{
		if(_filter.toStdString() == (StringANSI("*.") + AbstractList::_GetFileSuffix()))
		{
			saveCurveDialog->setDirectory((AssetLibrary::_GetDir() + AbstractList::_GetDir()).c_str());
		}
		else if(_filter.toStdString() == (StringANSI("*.") + AbstractObject::_GetFileSuffix()))
		{
			saveCurveDialog->setDirectory((AssetLibrary::_GetDir() + AbstractObject::_GetDir()).c_str());
		}
	}
}


void TextureCreator::VectorLerpSaveCurveAs(void)
{
	if(engine->GetVectorLerpCurve().IsNotEmpty())
	{
		QString name = "curve";

	    saveCurveDialog = new QFileDialog(this, "Save curve as...");
		saveCurveDialog->setAcceptMode(QFileDialog::AcceptSave);
		QStringList filtersList;
		filtersList.push_back(QString("*.") + AbstractList::_GetFileSuffix().c_str());
		filtersList.push_back(QString("*.") + AbstractObject::_GetFileSuffix().c_str());

		saveCurveDialog->setNameFilters(filtersList);
		saveCurveDialog->setDirectory((AssetLibrary::_GetDir() + AbstractList::_GetDir()).c_str());
		saveCurveDialog->selectFile(name);
		connect(saveCurveDialog, SIGNAL(filterSelected(const QString&)), this, SLOT(SaveCurveDialogFileFilterIsChanged(const QString&)));

		if(saveCurveDialog->exec())
		{ 
			QStringList files = saveCurveDialog->selectedFiles();
			QString name = File::_GetBaseName(files.at(0).toStdString()).c_str();

			if(saveCurveDialog->selectedFilter() == QString("*.") + AbstractList::_GetFileSuffix().c_str())
			{
				if(File::_IsExist(AssetLibrary::_GetDir() + AbstractList::_GetDir() + name.toStdString() + "." + AbstractList::_GetFileSuffix()))
				{ 
					WarningDialog warning("Warning", "List '" + name + "' is already exist. Rewrite file?", 200, 90);
					
					if(warning.exec() == QDialog::Rejected)
					{
						return;
					}
				}
				engine->VectorLerpSaveCurveAsList(name.toStdString());
			}
			else if(saveCurveDialog->selectedFilter() == QString("*.") + AbstractObject::_GetFileSuffix().c_str())
			{
				if(File::_IsExist(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + name.toStdString() + "." + AbstractObject::_GetFileSuffix()))
				{ 
					WarningDialog warning("Warning", "Object '" + name + "' is already exist. Rewrite file?", 200, 90);
					
					if(warning.exec() == QDialog::Rejected)
					{
						return;
					}
				}
				engine->VectorLerpSaveCurveAsObject(name.toStdString());
			}
		}

		delete saveCurveDialog;
		saveCurveDialog = NIL;
	}
	else
	{
		ErrorDialog("Error", "Curve is not defined", 145, 40);
	}
}


int TextureCreator::GetSelectedShapeIndex(void)const
{
	return shapeList->GetSelectedShapeIndex();
}


int32 TextureCreator::GetVectorLerpModeIndex(void)
{
	if(engine->GetVectorLerp()->GetLerpMode() == VectorLerp::BAZIER)
	{
		return 0;
	}
	else if(engine->GetVectorLerp()->GetLerpMode() == VectorLerp::LAGRANGE)
	{
		return 1;
	}
	else if(engine->GetVectorLerp()->GetLerpMode() == VectorLerp::RING)
	{
		return 2;
	}
	return -1;
}


int32 TextureCreator::GetVectorLerpMode(QString _mode)
{
	if(_mode == "BAZIER")
	{
		return VectorLerp::BAZIER;
	}
	else if(_mode == "LAGRANGE")
	{
		return VectorLerp::LAGRANGE;
	}
	else if(_mode == "RING")
	{
		return VectorLerp::RING;
	}
	return -1;
}


void TextureCreator::ChangeBuildMode(QString _mode)
{
	engine->CurrentBuildModeIsChanged(_mode.toStdString());

	if(buildModeComboBox->itemText(buildModeComboBox->currentIndex()) == "DELAUNAY")
	{
		createSubpointsToolButton->setEnabled(false);
	}
	else
	{
		createSubpointsToolButton->setEnabled(true);
	}
}



void TextureCreator::ChangeVectorLerpMode(QString _mode)
{
	engine->GetVectorLerp()->SetLerpMode(GetVectorLerpMode(_mode));
	engine->UpdateVectorLerpCurve();
}



void TextureCreator::ChangeVectorLerpAmountOfSegments(int _segments)
{
	engine->GetVectorLerp()->SetAmountOfSegments(_segments);
	engine->UpdateVectorLerpCurve();
}



void TextureCreator::CurrentTabIsChanged(int _index)
{
	if(editorMode == TEXTURED_ANIMATION_EDITOR)
	{
		if(tabWidget->tabText(_index) == "Hitbox") 
		{
			animationCurrentFrameSpinBox->setGeometry(QRect(130+10+30+30+30+10, 16, 51, 21));
			animationCurrentFrameSpinBox->setParent(hitboxTab);
			animationCurrentFrameSpinBox->show();
		}
		else if(tabWidget->tabText(_index) == "Properties") 
		{
			animationCurrentFrameSpinBox->setGeometry(QRect(90+10+5-10, 170-10, 51, 21));
		    animationCurrentFrameSpinBox->setParent(animationGroupBox);
			animationCurrentFrameSpinBox->show();
		}
	}

	engine->CurrentTabIsChanged(tabWidget->tabText(_index).toStdString());
}



QString TextureCreator::GetCurrentTabName(void)const
{
	return tabWidget->tabText(tabWidget->indexOf(tabWidget->currentWidget()));
}


void TextureCreator::ChangePivot(void)
{
	engine->ChangePivot();
}


void TextureCreator::CreateSubPoints(void)
{
	engine->CreateSubPoints();
}

 
void TextureCreator::PivotChangeState(bool _state)
{
	changePivotPushButton->setChecked(_state);
}


void TextureCreator::ViewportCursorEnter(void)
{
	engine->ShowCursor();
}


void TextureCreator::ViewportCursorLeave(void)
{
	engine->HideCursor();
}




void TextureCreator::StopAnimation(void)
{
	texturedAnimation->Stop();

	animationPlayButton->setText("Play");

	engine->GetSprite()->SetPosition(Vector3());
	animationCurrentFrameSpinBox->blockSignals(true);
	animationCurrentFrameSpinBox->setValue(texturedAnimation->GetCurrentFrame() + 1);
	animationCurrentFrameSpinBox->blockSignals(false);
	AnimationUpdateCurrentFrame(texturedAnimation->GetCurrentFrame());
	animationElapsedTimeSpinBox->blockSignals(true);
	animationElapsedTimeSpinBox->setValue(texturedAnimation->GetElapsedTimeInSec());
	animationElapsedTimeSpinBox->blockSignals(false);
}



void TextureCreator::PlayAnimation(void)
{
	if(texturedAnimation->IsPlaying())
	{
		texturedAnimation->Pause();
	}
	else
	{
		if(!texturedAnimation->IsLastFrame())
		{
			texturedAnimation->Play();
		}
		else
		{
			texturedAnimation->Stop();
			texturedAnimation->Play();
		}
	}
}



void TextureCreator::AnimationFPSIsChanged(double _fps)
{
	texturedAnimation->SetFPS(_fps);

	animationDurationSpinBox->blockSignals(true);
	animationDurationSpinBox->setValue(texturedAnimation->GetPlayingDurationInSec());
	animationDurationSpinBox->blockSignals(false);

	animationFrameDurationSpinBox->blockSignals(true);
	animationFrameDurationSpinBox->setValue(texturedAnimation->GetPlayingFrameDurationInSec());
	animationFrameDurationSpinBox->blockSignals(false);
}



void TextureCreator::AnimationDurationIsChanged(double _duration)
{
	texturedAnimation->SetPlayingDurationInSec(_duration);

	animationFPSSpinBox->blockSignals(true);
	animationFPSSpinBox->setValue(texturedAnimation->GetFPS()); 
	animationFPSSpinBox->blockSignals(false);
		
	animationFrameDurationSpinBox->blockSignals(true);
	animationFrameDurationSpinBox->setValue(texturedAnimation->GetPlayingFrameDurationInSec());
	animationFrameDurationSpinBox->blockSignals(false);
}


void TextureCreator::AnimationFrameDurationIsChanged(double _frameDuration)
{
	texturedAnimation->SetPlayingFrameDurationInSec(_frameDuration);

	animationDurationSpinBox->blockSignals(true);
	animationDurationSpinBox->setValue(texturedAnimation->GetPlayingDurationInSec());
	animationDurationSpinBox->blockSignals(false);

	animationFPSSpinBox->blockSignals(true);
	animationFPSSpinBox->setValue(texturedAnimation->GetFPS()); 
	animationFPSSpinBox->blockSignals(false);
}
 


void TextureCreator::SetTexturedAnimationEditor(void)
{ 
	editorMode = TEXTURED_ANIMATION_EDITOR;

	loadTextureButton->hide();
	nameLabel->move(10, 10+20);
	nameLineEdit->move(50, 10+20);
    texturedAnimationPropertyScrollArea = new QScrollArea(propertiesTab);
    texturedAnimationPropertyScrollArea->setGeometry(QRect(5, 5, 386+30, 291+100+85+25-40+60+30));
	texturedAnimationPropertyScrollAreaWidget = new QWidget(texturedAnimationPropertyScrollArea);
    texturedAnimationPropertyScrollAreaWidget->setGeometry(QRect(0, 0, 386-20+30, 291+100+85+25-45+60+30+30+30));
	texturedAnimationPropertyScrollArea->setWidget(texturedAnimationPropertyScrollAreaWidget);
	animationGroupBox = new QGroupBox(texturedAnimationPropertyScrollAreaWidget);
	animationGroupBox->setGeometry(QRect(10, 10, 386-10-10+5+7, 111+50+150+100+100-160-100-70-40+70-10-5));
    animationGroupBox->setTitle("Abstract animation:");
	animationFPSLabel = new QLabel(animationGroupBox);
	animationFPSLabel->setGeometry(QRect(10+5, 20, 31, 21));
	animationFPSLabel->setText("FPS:");

	animationFPSSpinBox = new FloatSpinBox(animationGroupBox);
	animationFPSSpinBox->setGeometry(QRect(90+10+5-10, 20, 81+10, 22));
	animationFPSSpinBox->setEnabled(false);
	connect(animationFPSSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AnimationFPSIsChanged(double)));
	animationFPSSpinBox->setDecimals(4);
	animationDurationLabel = new QLabel(animationGroupBox);
	animationDurationLabel->setGeometry(QRect(10+5, 50, 71, 21));
	animationDurationLabel->setText("Duration:");


	animationDurationSpinBox = new FloatSpinBox(animationGroupBox);
	animationDurationSpinBox->setGeometry(QRect(90+10+5-10, 50, 81+10, 22));
	animationDurationSpinBox->setEnabled(false);
	animationDurationSpinBox->setDecimals(4);
	connect(animationDurationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AnimationDurationIsChanged(double)));
	animationFrameDurationLabel = new QLabel(animationGroupBox);
	animationFrameDurationLabel->setGeometry(QRect(10+5, 80, 81, 21));
	animationFrameDurationLabel->setText("Frame duration:");
       
	animationFrameDurationSpinBox = new FloatSpinBox(animationGroupBox);
	animationFrameDurationSpinBox->setGeometry(QRect(90+10+5-10, 80, 81+10, 22));
	animationFrameDurationSpinBox->setEnabled(false);
	animationFrameDurationSpinBox->setDecimals(4);
	connect(animationFrameDurationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AnimationFrameDurationIsChanged(double)));
	animationElapsedTimeLabel = new QLabel(animationGroupBox);
    animationElapsedTimeLabel->setGeometry(QRect(10+5, 110, 91, 21));
	animationElapsedTimeLabel->setText("Elapsed time:");

    animationElapsedTimeSpinBox = new FloatSpinBox(animationGroupBox);
    animationElapsedTimeSpinBox->setGeometry(QRect(90+10+5-10, 110, 81+10, 22));
	animationElapsedTimeSpinBox->setDecimals(4);
	animationElapsedTimeSpinBox->setEnabled(false);
	connect(animationElapsedTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AnimationElapsedTimeIsChanged(double)));
	animationPlayingModeGroupBox = new QGroupBox(animationGroupBox);
    animationPlayingModeGroupBox->setGeometry(QRect(150+25+15+10-10+5, 20-5, 241-40-20-15+2+7, 131-30+7+8));
	animationPlayingModeGroupBox->setTitle("Playing mode:");
	QButtonGroup* group1 = new QButtonGroup(animationPlayingModeGroupBox);
	QButtonGroup* group2 = new QButtonGroup(animationPlayingModeGroupBox);
	QButtonGroup* group3 = new QButtonGroup(animationPlayingModeGroupBox);
	animationForwardRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationForwardRadioButton->setGeometry(QRect(30-20, 30-20+7+6, 71, 21));
	animationForwardRadioButton->setText("Forward");
	connect(animationForwardRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationForwardPlayingModeEnable(bool)));
	group1->addButton(animationForwardRadioButton);
	animationBackwardRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationBackwardRadioButton->setGeometry(QRect(140-40-10+3, 30-20+7+6, 71, 21));
	animationBackwardRadioButton->setText("Backward");
    connect(animationBackwardRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationBackwardPlayingModeEnable(bool)));
	group1->addButton(animationBackwardRadioButton);
	animationPingPongRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationPingPongRadioButton->setGeometry(QRect(30-20, 60-20+7+6, 71, 21));
	animationPingPongRadioButton->setText("Ping pong");
    connect(animationPingPongRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationPingPongPlayingModeEnable(bool)));
	group2->addButton(animationPingPongRadioButton);
	animationOnewayRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationOnewayRadioButton->setGeometry(QRect(140-40-10+3, 60-20+7+6, 71, 21));
	animationOnewayRadioButton->setText("Oneway");
    connect(animationOnewayRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationOnewayPlayingModeEnable(bool)));
	group2->addButton(animationOnewayRadioButton);
	animationSingleShotRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationSingleShotRadioButton->setGeometry(QRect(140-40-10+3, 90-20+7+6, 81, 21));
	animationSingleShotRadioButton->setText("Single shot");
 	connect(animationSingleShotRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationSingleShotPlayingModeEnable(bool)));
	group3->addButton(animationSingleShotRadioButton);
	animationLoopRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationLoopRadioButton->setGeometry(QRect(30-20, 90-20+7+6, 71, 21));
	animationLoopRadioButton->setText("Loop");
    connect(animationLoopRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationLoopPlayingModeEnable(bool)));
	group3->addButton(animationLoopRadioButton);
    

	animationForwardRadioButton->setChecked(true);
	animationBackwardRadioButton->setChecked(false);
	animationOnewayRadioButton->setChecked(true);
	animationPingPongRadioButton->setChecked(false);
	animationSingleShotRadioButton->setChecked(true);
	animationLoopRadioButton->setChecked(false);
    animationCurrentFrameLine = new QFrame(animationGroupBox);
    animationCurrentFrameLine->setGeometry(QRect(10+5, 140-5, 120+150+50+20-2+15, 20));
    animationCurrentFrameLine->setFrameShape(QFrame::HLine);
    animationCurrentFrameLine->setFrameShadow(QFrame::Sunken);
	animationCurrentFrameLabel = new QLabel(animationGroupBox);
    animationCurrentFrameLabel->setGeometry(QRect(10+5, 170-10-4, 71, 31));
    animationCurrentFrameLabel->setText("Current frame:"); 

	animationCurrentFrameSpinBox = new QSpinBox(animationGroupBox);
    animationCurrentFrameSpinBox->setGeometry(QRect(90+10+5-10, 170-10, 51, 21));
	animationCurrentFrameSpinBox->setRange(1, 1);
	animationCurrentFrameSpinBox->setToolTip(QString::number(0) + " - " + QString::number(0));
	animationCurrentFrameSpinBox->setEnabled(false);
	connect(animationCurrentFrameSpinBox, SIGNAL(valueChanged(int)), this, SLOT(AnimationCurrentFrameIsChanged(int)));
	animationPlayButton = new QPushButton(animationGroupBox);
	animationPlayButton->setGeometry(QRect(10+80+5+12+75+10, 170-10, 71, 21));
	animationPlayButton->setText("Play");
	connect(animationPlayButton, SIGNAL(clicked()), this, SLOT(PlayAnimation()));
	animationStopButton = new QPushButton(animationGroupBox);
	animationStopButton->setGeometry(QRect(10+80+5+12+75+80+10, 170-10, 71, 21));
	animationStopButton->setText("Stop");
	connect(animationStopButton, SIGNAL(clicked()), this, SLOT(StopAnimation()));
	texturedAnimationGroupBox = new QGroupBox(texturedAnimationPropertyScrollAreaWidget);
	texturedAnimationGroupBox->setGeometry(QRect(10, 200-40-20+10+10+70-20, 386-10-10+5+7, 60-5+30));
	texturedAnimationGroupBox->setTitle("Textured animation:");
    texturedAnimationPositionOffsetEnableLabel = new QLabel(texturedAnimationGroupBox);
    texturedAnimationPositionOffsetEnableLabel->setGeometry(15, 10+10, 40+60+15, 21);
	texturedAnimationPositionOffsetEnableLabel->setText("Position offset enable:");

	texturedAnimationPositionOffsetEnableCheckBox = new QCheckBox(texturedAnimationGroupBox); 
    texturedAnimationPositionOffsetEnableCheckBox->setGeometry(QRect(10+40+60+15+10-3+5, 10+10, 21, 21));
	connect(texturedAnimationPositionOffsetEnableCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TexturedAnimationPositionOffsetEnable(int)));
	texturedAnimationPositionOffsetTemplateButton = new QPushButton(texturedAnimationGroupBox);
    texturedAnimationPositionOffsetTemplateButton->setGeometry(QRect(10+40+60+15+15-3+20+10+10, 20, 71, 21));
	texturedAnimationPositionOffsetTemplateButton->setText("Template");
	texturedAnimationPositionOffsetTemplateButton->setEnabled(false);
    animationShowPreviousFrameLabel = new QLabel(texturedAnimationGroupBox);
    animationShowPreviousFrameLabel->setGeometry(15, 50, 40+60+15, 21);
	animationShowPreviousFrameLabel->setText("Show previous position:");

	animationShowPreviousFrameCheckBox = new QCheckBox(texturedAnimationGroupBox); 
    animationShowPreviousFrameCheckBox->setGeometry(QRect(10+40+60+15+10-3+5, 50, 21, 21));
	animationShowPreviousFrameCheckBox->setChecked(true);
	connect(animationShowPreviousFrameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AnimationShowPreviousPositionOffset(int)));
	atlasGroupBox = new QGroupBox(texturedAnimationPropertyScrollAreaWidget);
	atlasGroupBox->setGeometry(QRect(10, 200-40-20+10+10+70+30+10+30, 386-10-10+5+7, 111+50+150+100+100-200-23-7+3));
	atlasGroupBox->setTitle("Atlas:");
	atlasNameLabel = new QLabel(atlasGroupBox);
	atlasNameLabel->setGeometry(QRect(20-10+5, 20, 41, 21));
	atlasNameLabel->setText("Name:");

	atlasNameLineEdit = new QLineEdit(atlasGroupBox);
	atlasNameLineEdit->setGeometry(QRect(70-5-10, 20, 161, 20));
	atlasNameLineEdit->setMaxLength(nameMaxLength);
	atlasNameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
    connect(atlasNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(AtlasNameIsChanged(QString)));
	animationCreateAtlasButton = new QPushButton(atlasGroupBox);
    animationCreateAtlasButton->setGeometry(QRect(70+161+20-5-10-10, 20, 61-5+10, 22));
	animationCreateAtlasButton->setText("Create");  
	connect(animationCreateAtlasButton, SIGNAL(clicked()), this, SLOT(AnimationCreateAtlas()));
	animationImportAtlasButton = new QPushButton(atlasGroupBox);
    animationImportAtlasButton->setGeometry(QRect(70+161+20-5+60+10-10-10-10+5+10, 20, 61-5+10, 22)); 
	animationImportAtlasButton->setText("Import");  
	animationImportAtlasButton->setToolTip("Import atlas");
	connect(animationImportAtlasButton, SIGNAL(clicked()), this, SLOT(ImportAtlas()));
	animationSaveAtlasAsButton = new QPushButton(atlasGroupBox);
    animationSaveAtlasAsButton->setGeometry(QRect(70+161+20-5+60+10-10-10-10+5+10, 20+30, 61-5+10, 22)); 
	animationSaveAtlasAsButton->setText("Save as...");  
	animationSaveAtlasAsButton->setToolTip("Save atlas as...");
	connect(animationSaveAtlasAsButton, SIGNAL(clicked()), this, SLOT(SaveAtlasAs()));
	atlasFilteringLabel = new QLabel(atlasGroupBox);
    atlasFilteringLabel->setGeometry(QRect(15, 50, 91, 21));
	atlasFilteringLabel->setText("Texture filtering:");

	atlasFilteringComboBox = new QComboBox(atlasGroupBox);
    atlasFilteringComboBox->setGeometry(QRect(10+80+5+12, 46+5, 108, 21));
	atlasFilteringComboBox->addItem("POINT");
	atlasFilteringComboBox->addItem("BILINEAR");
	atlasFilteringComboBox->addItem("TRILINEAR");
	atlasFilteringComboBox->addItem("ANISOTROPY");
	atlasFilteringComboBox->setCurrentIndex(0);
	connect(atlasFilteringComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(AtlasFilteringIsChanged(QString)));
    atlasAmountOfAnimFrameLabel = new QLabel(atlasGroupBox);
    atlasAmountOfAnimFrameLabel->setGeometry(QRect(15, 80, 121+10, 21));
	atlasAmountOfAnimFrameLabel->setText("Amount of frames: 0/0");
	atlasFrameGroupBox = new QGroupBox(atlasGroupBox);
	atlasFrameGroupBox->setGeometry(QRect(15-3, 130+10-30, 150+100+70+10+10+10+7, 190-20+30-15-25+5));
	atlasFrameGroupBox->setTitle("Frame:");
	atlasFrameNameLabel = new QLabel(atlasFrameGroupBox);
	atlasFrameNameLabel->setGeometry(QRect(10, 20, 41, 21));
	atlasFrameNameLabel->setText("Name:");

	atlasFrameNameLineEdit = new QLineEdit(atlasFrameGroupBox);
	atlasFrameNameLineEdit->setGeometry(QRect(60-5-5, 20, 161-10, 20));
	atlasFrameNameLineEdit->setMaxLength(nameMaxLength);
	atlasFrameNameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	atlasFrameNameLineEdit->setEnabled(false);
    connect(atlasFrameNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(AtlasFrameNameIsChanged(QString)));
	
	
	sizeMin = 1;
	sizeMax = Texture2D::_GetTextureMaxSize();
	atlasFrameWidthLabel = new QLabel(atlasFrameGroupBox);
    atlasFrameWidthLabel->setGeometry(QRect(10, 50, 41+30, 21));
	atlasFrameWidthLabel->setText("Width:"); 

	atlasFrameWidthSpinBox = new QSpinBox(atlasFrameGroupBox);
    atlasFrameWidthSpinBox->setGeometry(QRect(60-5-5, 50, 71, 22));
	atlasFrameWidthSpinBox->setRange(sizeMin, sizeMax);
	atlasFrameWidthSpinBox->setToolTip(QString::number(sizeMin) + " - " + QString::number(sizeMax));
	atlasFrameWidthSpinBox->setEnabled(false);
	atlasFrameHeightLabel = new QLabel(atlasFrameGroupBox);
    atlasFrameHeightLabel->setGeometry(QRect(10, 80, 41+30, 21));
	atlasFrameHeightLabel->setText("Height:");

    atlasFrameHeightSpinBox = new QSpinBox(atlasFrameGroupBox);
    atlasFrameHeightSpinBox->setGeometry(QRect(60-5-5, 80, 71, 22));
	atlasFrameHeightSpinBox->setEnabled(false);
	atlasFrameHeightSpinBox->setRange(sizeMin, sizeMax);
	atlasFrameHeightSpinBox->setToolTip(QString::number(sizeMin) + " - " + QString::number(sizeMax));
    texturedAnimationPositionOffsetLine = new QFrame(atlasFrameGroupBox);
    texturedAnimationPositionOffsetLine->setGeometry(QRect(10, 110+10-30-5+20-5+30-20-5-30+30, 120+150+50+20-2, 20));
    texturedAnimationPositionOffsetLine->setFrameShape(QFrame::HLine);
    texturedAnimationPositionOffsetLine->setFrameShadow(QFrame::Sunken);
	atlasPositionOffsetLabel = new QLabel(atlasFrameGroupBox);
    atlasPositionOffsetLabel->setGeometry(QRect(10, 110+10-30-5+50-5-30+30, 21+90, 21));
    atlasPositionOffsetLabel->setText("Position offset X/Y/Z:"); 

	atlasXPositionOffsetSpinBox = new FloatSpinBox(atlasFrameGroupBox);
    atlasXPositionOffsetSpinBox->setGeometry(QRect(20+5+95, 110+10-30-5+50-5, 71+10-10, 22));
	atlasXPositionOffsetSpinBox->setDecimals(4);
	atlasXPositionOffsetSpinBox->setRange(Numerical<float>::_LowerValue(), Numerical<float>::_UpperValue());
	atlasXPositionOffsetSpinBox->setToolTip(QString::number(Numerical<float>::_LowerValue()) + " - " + QString::number(Numerical<float>::_UpperValue()));
	atlasXPositionOffsetSpinBox->setEnabled(false);
	connect(atlasXPositionOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AtlasXPositionOffsetIsChanged(double)));


	atlasYPositionOffsetSpinBox = new FloatSpinBox(atlasFrameGroupBox);
    atlasYPositionOffsetSpinBox->setGeometry(QRect(120+10+10+70-10-3, 110+10-30-5+50-5-30+30, 71+10-10, 22));
	atlasYPositionOffsetSpinBox->setDecimals(4);
	atlasYPositionOffsetSpinBox->setRange(Numerical<float>::_LowerValue(), Numerical<float>::_UpperValue());
	atlasYPositionOffsetSpinBox->setToolTip(QString::number(Numerical<float>::_LowerValue()) + " - " + QString::number(Numerical<float>::_UpperValue()));
	atlasYPositionOffsetSpinBox->setEnabled(false);
	connect(atlasYPositionOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AtlasYPositionOffsetIsChanged(double)));


	atlasZPositionOffsetSpinBox = new FloatSpinBox(atlasFrameGroupBox);
    atlasZPositionOffsetSpinBox->setGeometry(QRect(220+15+10+10+45-10-10-3-3, 110+10-30-5+50-5-30+30, 71+10-10, 22));
	atlasZPositionOffsetSpinBox->setDecimals(4);
	atlasZPositionOffsetSpinBox->setRange(Numerical<float>::_LowerValue(), Numerical<float>::_UpperValue());
	atlasZPositionOffsetSpinBox->setToolTip(QString::number(Numerical<float>::_LowerValue()) + " - " + QString::number(Numerical<float>::_UpperValue()));
	atlasZPositionOffsetSpinBox->setEnabled(false);
	connect(atlasZPositionOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AtlasZPositionOffsetIsChanged(double)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(TexturedAnimationIsOk()));

	setWindowTitle("Textured Animation Creator");
} 


void TextureCreator::AnimationElapsedTimeIsChanged(double _value)
{ 
	animationCurrentFrameSpinBox->setValue(_value / animationFrameDurationSpinBox->value());
}



void TextureCreator::SaveAtlasAs(void)
{
	if(atlas->IsExist())
	{
		QString atlasName = atlasNameLineEdit->text();

		QString pathFile = QFileDialog::getSaveFileName(this,
														"Save atlas as...",
														(AssetLibrary::_GetDir() + Atlas2D::_GetDir()).c_str() + atlasName,
														QString("*") + Atlas2D::_GetFileSuffix().c_str());

		if(!pathFile.isEmpty())
		{	
			QString name = File::_GetBaseName(pathFile.toStdString()).c_str();
			if(File::_IsExist(AssetLibrary::_GetDir() + Atlas2D::_GetDir() + name.toStdString() + "." + Atlas2D::_GetFileSuffix()))
			{  
				WarningDialog warning("Warning", "Atlas '" + name + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					return;
				}
			}
			for(int j = 0; j < atlas->GetAmountOfFrames(); j++)
			{ 
				Hitbox* hitbox = new Hitbox();
				hitbox->Rename(hitboxNames.Get(j)->key.toStdString());
				for(int i = 0; i < engine->GetShapes().GetSize(); i++)
				{
					TextureViewer::GraphicsSurfaceEx* object = engine->GetObjects().Get(i);

					if(object && object->GetFrameIndex() == j)
					{
						TextureShapeListWidget::ItemShape* it = dynamic_cast<TextureShapeListWidget::ItemShape*>(shapeList->topLevelItem(i));

						if(it)
						{
							StringANSI shapeName = it->text(TextureShapeListWidget::ItemShape::NAME_PROPERTY).toStdString();
							int32 collisionGroupIndex = it->text(TextureShapeListWidget::ItemShape::COLLISION_GROUP_PROPERTY).toInt();
							bool collisionEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_ENABLE_PROPERTY) == "true" ? true : false;
							bool collisionPolygonsInfoEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_POLYGONS_INFO_PROPERTY) == "true" ? true : false;
							Mesh* mesh = engine->GetShapes().Get(i);
							mesh->SetBoundingVolumeType(engine->GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString()));
							
							if(mesh)
							{
								if(AssetLibrary::_IsAssetExist(mesh->GetName(), AssetLibrary::MESH_ASSET))
								{
									AssetLibrary::_GetAssets().AddObject(mesh);
								}
								hitbox->AddShape(mesh, shapeName, collisionGroupIndex, collisionEnable, collisionPolygonsInfoEnable);
							} 
						} 
					}
				}
	
				if(hitbox->GetShapes().IsNotEmpty())
				{
					if(AssetLibrary::_IsAssetExist(hitbox->GetName(), AssetLibrary::HITBOX_ASSET))
					{
						AssetLibrary::_GetAssets().AddObject(hitbox);
					}
					
					atlas->BindHitbox(j, hitbox);
				}
				else
				{
					delete hitbox;
				}
			} 

			atlas->Rename(name.toStdString());
					
			atlas->SaveToFile();
		}
	}
	else
	{
		ErrorDialog("Error", "Atlas is not defined", 170, 40);
	}
}


void TextureCreator::AnimationCurrentFrameIsChanged(int _index)
{	
	texturedAnimation->SetCurrentFrame(_index - 1);
   
	animationElapsedTimeSpinBox->blockSignals(true);
	animationElapsedTimeSpinBox->setValue(texturedAnimation->GetPlayingFrameDurationInSec() * (_index - 1));
	animationElapsedTimeSpinBox->blockSignals(false);


	if(editorMode == TEXTURED_ANIMATION_EDITOR)
	{
		hitboxNameLineEdit->blockSignals(true);
		hitboxNameLineEdit->setText(hitboxNames.Get(_index - 1)->key);
		hitboxNameLineEdit->blockSignals(false);
	} 

	AnimationUpdateCurrentFrame(_index - 1); 
}




void TextureCreator::AnimationUpdateCurrentFrame(int32 _index)
{
	Atlas2D::Frame* frameInfo = atlas->GetFrameInfo(_index);
	
	if(frameInfo)
	{
		atlasFrameNameLineEdit->blockSignals(true);
		atlasFrameNameLineEdit->setText(frameInfo->GetName().c_str());
		atlasFrameNameLineEdit->blockSignals(false);
		atlasFrameWidthSpinBox->blockSignals(true);
		atlasFrameHeightSpinBox->blockSignals(true);
		
		atlasFrameWidthSpinBox->setValue(frameInfo->GetSizeInPixels().x);
		atlasFrameHeightSpinBox->setValue(frameInfo->GetSizeInPixels().y);
		
		atlasFrameWidthSpinBox->blockSignals(false);
		atlasFrameHeightSpinBox->blockSignals(false);
		atlasXPositionOffsetSpinBox->blockSignals(true);
		atlasYPositionOffsetSpinBox->blockSignals(true);
		atlasZPositionOffsetSpinBox->blockSignals(true);
		
		atlasXPositionOffsetSpinBox->setValue(frameInfo->GetPositionOffset().x);
		atlasYPositionOffsetSpinBox->setValue(frameInfo->GetPositionOffset().y);
		atlasZPositionOffsetSpinBox->setValue(frameInfo->GetPositionOffset().z);
		
		atlasXPositionOffsetSpinBox->blockSignals(false);
		atlasYPositionOffsetSpinBox->blockSignals(false);
		atlasZPositionOffsetSpinBox->blockSignals(false);
	}
}
 

void TextureCreator::SetTextureEditor(void)
{ 
	editorMode = TEXTURE_EDITOR;
    texturePropertyScrollArea = new QScrollArea(propertiesTab);
    texturePropertyScrollArea->setGeometry(QRect(5, 5, 386, 291+85+25-40-235));
	texturePropertyScrollAreaWidget = new QWidget(texturePropertyScrollArea);
    texturePropertyScrollAreaWidget->setGeometry(QRect(0, 0, 386-20, 291+85+25-45-235));
	texturePropertyScrollArea->setWidget(texturePropertyScrollAreaWidget);
	sizeMin = 1;
	sizeMax = Texture2D::_GetTextureMaxSize();
	textureWidthLabel = new QLabel(texturePropertyScrollAreaWidget);
    textureWidthLabel->setGeometry(QRect(10, 20, 81, 21));
	textureWidthLabel->setText("Width:");

    textureWidthSpinBox = new IntSpinBox(texturePropertyScrollAreaWidget);
	textureWidthSpinBox->setGeometry(QRect(100, 20, 111, 22));
	textureWidthSpinBox->setEnabled(false);
	textureWidthSpinBox->setRange(sizeMin, sizeMax);
	textureWidthSpinBox->setToolTip(QString::number(sizeMin) + " - " + QString::number(sizeMax) + " pixels");
	textureHeightLabel = new QLabel(texturePropertyScrollAreaWidget);
    textureHeightLabel->setGeometry(QRect(10, 50, 81, 21));
	textureHeightLabel->setText("Height:");

	textureHeightSpinBox = new IntSpinBox(texturePropertyScrollAreaWidget);
	textureHeightSpinBox->setGeometry(QRect(100, 50, 111, 22));
	textureHeightSpinBox->setEnabled(false);
	textureHeightSpinBox->setRange(sizeMin, sizeMax);
	textureHeightSpinBox->setToolTip(QString::number(sizeMin) + " - " + QString::number(sizeMax) + " pixels");
	textureFilteringLabel = new QLabel(texturePropertyScrollAreaWidget);
    textureFilteringLabel->setGeometry(QRect(10, 80, 85, 21));
	textureFilteringLabel->setText("Texture filtering:");

	textureFilteringComboBox = new QComboBox(texturePropertyScrollAreaWidget);
    textureFilteringComboBox->setGeometry(QRect(100, 76+5, 111, 21));
	textureFilteringComboBox->addItem("POINT");
	textureFilteringComboBox->addItem("BILINEAR");
	textureFilteringComboBox->addItem("TRILINEAR");
	textureFilteringComboBox->addItem("ANISOTROPY");
	textureFilteringComboBox->setCurrentIndex(0);
	connect(textureFilteringComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(TextureFilteringIsChanged(QString)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(TextureIsOk()));

	setWindowTitle("Texture Creator");
}


void TextureCreator::SetVectorLerpEditor(void)
{
	tabWidget->setCurrentIndex(tabWidget->indexOf(vectorLerpTab));

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	setWindowTitle("Vector Lerp Creator");
}


void TextureCreator::SetFloatLerpEditor(void)
{
	tabWidget->setCurrentIndex(tabWidget->indexOf(vectorLerpTab));

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	setWindowTitle("Float Lerp Creator");
}



void TextureCreator::SetHitboxEditor(void)
{
	tabWidget->setCurrentIndex(tabWidget->indexOf(hitboxTab));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	setWindowTitle("Hitbox Creator");
}




void TextureCreator::AnimationCreateAtlas(void)
{
	QString path = QFileDialog::getExistingDirectory(this,
													 "Choose directory",
													 startDir,
													 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if(path.count() == 0)
	{
		return;
	}

	CreateAtlas(path);
}


void TextureCreator::CreateAtlas(QString _path)
{
	engine->DestroyPoints(); 
	DestroyAllShapes();

	QFileInfo fileInfo(_path);
	startDir = fileInfo.path();
	 
	engine->GetScene()->GetRenderList().Remove(engine->GetSprite());
	engine->GetScene()->GetUpdateList().Remove(texturedAnimation);
	if(atlas->LoadImagesFromFolder(_path.toStdString()))
	{ 
		QString folderName = fileInfo.completeBaseName();
		if(atlasNameLineEdit->isEnabled())
		{
			atlasNameLineEdit->setText(folderName);
		}
		if(nameLineEdit->isEnabled())
		{
			if(nameLineEdit->text().length() == 0)
			{
				nameLineEdit->setText(folderName);
			}
		}
		animationFPSSpinBox->setEnabled(true);
		animationDurationSpinBox->setEnabled(true);
		animationFrameDurationSpinBox->setEnabled(true);
		animationElapsedTimeSpinBox->setEnabled(true);

		atlasFrameNameLineEdit->setEnabled(true);
		atlasXPositionOffsetSpinBox->setEnabled(true);
		atlasYPositionOffsetSpinBox->setEnabled(true);
		atlasZPositionOffsetSpinBox->setEnabled(true);
		animationCurrentFrameSpinBox->setRange(1, atlas->GetAmountOfFrames());
		animationCurrentFrameSpinBox->setToolTip(QString::number(1) + " - " + QString::number(atlas->GetAmountOfFrames()));
		animationCurrentFrameSpinBox->setEnabled(true);
		texturedAnimation->BindAtlas(atlas);
		animationMinDuration = 0.1f;
		animationMaxDuration = 60.0f;

		animationMinFrameDuration = animationMinDuration / atlas->GetAmountOfFrames();
		animationMaxFrameDuration = animationMaxDuration / atlas->GetAmountOfFrames();
		
		animationMinFPS = 1.0f / animationMaxFrameDuration; 
		animationMaxFPS = 1.0f / animationMinFrameDuration; 

		animationDurationSpinBox->blockSignals(true);
		animationDurationSpinBox->setSingleStep(animationMaxDuration / (atlas->GetAmountOfFrames() * 2));
		animationDurationSpinBox->setToolTip(QString::number(animationMinDuration) + " - " + QString::number(animationMaxDuration) + " (sec)"); 
		animationDurationSpinBox->setRange(animationMinDuration, animationMaxDuration);
	    animationDurationSpinBox->blockSignals(false);
	

		animationFrameDurationSpinBox->blockSignals(true);
		animationFrameDurationSpinBox->setSingleStep(animationMaxFrameDuration / (atlas->GetAmountOfFrames() * 2));
		animationFrameDurationSpinBox->setToolTip(QString::number(animationMinFrameDuration) + " - " + QString::number(animationMaxFrameDuration) + " (sec)"); 
		animationFrameDurationSpinBox->setRange(animationMinFrameDuration, animationMaxFrameDuration);
        animationFrameDurationSpinBox->blockSignals(false);
	

		animationFPSSpinBox->blockSignals(true);
		animationFPSSpinBox->setSingleStep(animationMaxFPS / (atlas->GetAmountOfFrames() * 2));
		animationFPSSpinBox->setRange(animationMinFPS, animationMaxFPS);
		animationFPSSpinBox->setToolTip(QString::number(animationMinFPS) + " - " + QString::number(animationMaxFPS) + " (frames per second)"); 
		animationFPSSpinBox->blockSignals(false);
	

		animationElapsedTimeSpinBox->blockSignals(true);
		animationElapsedTimeSpinBox->setSingleStep(1.0f);
		animationElapsedTimeSpinBox->setToolTip("0.0 - " + QString::number(animationMaxDuration) + " (sec)");
		animationElapsedTimeSpinBox->setRange(0.0f, animationMaxDuration);
		animationElapsedTimeSpinBox->blockSignals(false);
		atlasAmountOfAnimFrameLabel->setText("Amount of frames: " + QString::number(atlas->GetAmountOfFrames()) + "/" + QString::number(Atlas2D::_GetMaxFrames()));
		engine->GetSprite()->BindFrontTexturedAnimation(texturedAnimation);
		engine->GetSprite()->GetMesh()->CreateQuad(Quad(Vector3(), texturedAnimation->GetCurrentFrameSize() / engine->GetPixelsPerMeter()));
		engine->GetSprite()->SetPosition(Vector3());
				
		animationCurrentFrameSpinBox->blockSignals(true);
		animationCurrentFrameSpinBox->setValue(texturedAnimation->GetCurrentFrame() + 1);
		animationCurrentFrameSpinBox->blockSignals(false);
		AnimationUpdateCurrentFrame(texturedAnimation->GetCurrentFrame());
		if(texturedAnimation->GetFPS() < animationMinFPS)
		{ 
			animationFPSSpinBox->setValue(animationMinFPS);
			animationDurationSpinBox->setValue(animationMaxDuration);
			animationFrameDurationSpinBox->setValue(animationMaxFrameDuration);
		}
		else if(texturedAnimation->GetFPS() > animationMaxFPS)
		{ 
			animationFPSSpinBox->setValue(animationMaxFPS);
			animationDurationSpinBox->setValue(animationMinDuration);
			animationFrameDurationSpinBox->setValue(animationMinFrameDuration);
		}
		else
		{
			animationFPSSpinBox->setValue(texturedAnimation->GetFPS());
			animationDurationSpinBox->setValue(texturedAnimation->GetPlayingDurationInSec());
			animationFrameDurationSpinBox->setValue(texturedAnimation->GetPlayingFrameDurationInSec());
		}
		SetHome();
		hitboxNames.Clear();
		
		if(atlas->IsExist())
		{
			for(int32 i = 0; i < atlas->GetAmountOfFrames(); i++)
			{ 
				hitboxNames.AddToTail(QString()); 
			}
		}
		 
		engine->GetScene()->AddToRenderList(engine->GetSprite());
		engine->GetScene()->AddToUpdateList(texturedAnimation);
		
		okButton->setEnabled(true);
	}
	else
	{
		okButton->setEnabled(false);
	}
}


bool TextureCreator::IsAnimationPreviousPositionShowEnabled(void)const
{
	return animationShowPreviousFrameCheckBox->isChecked();
}



void TextureCreator::AtlasXPositionOffsetIsChanged(double _x)
{
	Atlas2D::Frame* frameInfo = atlas->GetFrameInfo(animationCurrentFrameSpinBox->value() - 1);

	if(frameInfo)
	{
		Vector3 offset = frameInfo->GetPositionOffset();
		offset.x = _x;
		frameInfo->SetPositionOffset(offset);
	}
}


void TextureCreator::AtlasYPositionOffsetIsChanged(double _y)
{
	Atlas2D::Frame* frameInfo = atlas->GetFrameInfo(animationCurrentFrameSpinBox->value() - 1);

	if(frameInfo)
	{
		Vector3 offset = frameInfo->GetPositionOffset();
		offset.y = _y;
		frameInfo->SetPositionOffset(offset);
	}
}



void TextureCreator::AtlasZPositionOffsetIsChanged(double _z)
{
	Atlas2D::Frame* frameInfo = atlas->GetFrameInfo(animationCurrentFrameSpinBox->value() - 1);

	if(frameInfo)
	{
		Vector3 offset = frameInfo->GetPositionOffset();
		offset.z = _z;
		frameInfo->SetPositionOffset(offset);
	}
}



void TextureCreator::AnimationForwardPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		texturedAnimation->SetPlayingMode(PlayingObject::FORWARD_PLAYING);
	}
}



void TextureCreator::AnimationBackwardPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		texturedAnimation->SetPlayingMode(PlayingObject::BACKWARD_PLAYING);
	}
}



void TextureCreator::AnimationPingPongPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		texturedAnimation->SetPlayingMode(PlayingObject::PING_PONG_PLAYING);
	}
}


void TextureCreator::AnimationOnewayPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		texturedAnimation->SetPlayingMode(PlayingObject::ONEWAY_PLAYING);
	}
}



void TextureCreator::AnimationLoopPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		texturedAnimation->SetPlayingMode(PlayingObject::LOOP_PLAYING);
	}
}


void TextureCreator::AnimationSingleShotPlayingModeEnable(bool _enable)
{
	if(_enable)
	{
		texturedAnimation->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING);
	}
}


void TextureCreator::PrepareToPlayAnimation(void)
{
	if(animationPlayButton->text() != "Play")
	{
		animationPlayButton->setText("Play"); 
	}
}
 

void TextureCreator::PrepareToPauseAnimation(void)
{
	if(animationPlayButton->text() != "Pause")
	{
		animationPlayButton->setText("Pause");
	}
}


void TextureCreator::SetAnimationCurrentFrame(int32 _index)
{
	animationCurrentFrameSpinBox->blockSignals(true);
	animationCurrentFrameSpinBox->setValue(_index + 1); 
	animationCurrentFrameSpinBox->blockSignals(false);

	AnimationUpdateCurrentFrame(_index);
} 
	


void TextureCreator::ImportAtlas(void)
{
	QFileDialog dialog(this, "Choose atlas");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + Atlas2D::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + Atlas2D::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		StringANSI atlasName = File::_GetBaseName(assets.at(0).toStdString());

		ReloadAtlas(atlasName.c_str()); 
	}
}


void TextureCreator::ReloadAtlas(QString _name)
{
	engine->DestroyPoints(); 
	DestroyAllShapes();

	engine->GetScene()->GetRenderList().Remove(engine->GetSprite());
	engine->GetScene()->GetUpdateList().Remove(texturedAnimation);
	if(atlas->LoadFromFile(_name.toStdString()))
	{
		if(atlasNameLineEdit->isEnabled())
		{
			atlasNameLineEdit->setText(_name);
		}
		if(nameLineEdit->isEnabled())
		{
			if(nameLineEdit->text().length() == 0)
			{
				nameLineEdit->setText(_name);
			}
		}
		animationFPSSpinBox->setEnabled(true);
		animationDurationSpinBox->setEnabled(true);
		animationFrameDurationSpinBox->setEnabled(true);
		animationElapsedTimeSpinBox->setEnabled(true);

		atlasFrameNameLineEdit->setEnabled(true);
		atlasXPositionOffsetSpinBox->setEnabled(true);
		atlasYPositionOffsetSpinBox->setEnabled(true);
		atlasZPositionOffsetSpinBox->setEnabled(true);
		animationCurrentFrameSpinBox->setRange(1, atlas->GetAmountOfFrames());
		animationCurrentFrameSpinBox->setToolTip(QString::number(1) + " - " + QString::number(atlas->GetAmountOfFrames()));
		animationCurrentFrameSpinBox->setEnabled(true);
		texturedAnimation->BindAtlas(atlas);
		animationMinDuration = 0.1f;
		animationMaxDuration = 60.0f;

		animationMinFrameDuration = animationMinDuration / atlas->GetAmountOfFrames();
		animationMaxFrameDuration = animationMaxDuration / atlas->GetAmountOfFrames();
		
		animationMinFPS = 1.0f / animationMaxFrameDuration; 
		animationMaxFPS = 1.0f / animationMinFrameDuration; 

		animationDurationSpinBox->blockSignals(true);
		animationDurationSpinBox->setSingleStep(animationMaxDuration / (atlas->GetAmountOfFrames() * 2));
		animationDurationSpinBox->setToolTip(QString::number(animationMinDuration) + " - " + QString::number(animationMaxDuration) + " (sec)"); 
		animationDurationSpinBox->setRange(animationMinDuration, animationMaxDuration);
	    animationDurationSpinBox->blockSignals(false);
	

		animationFrameDurationSpinBox->blockSignals(true);
		animationFrameDurationSpinBox->setSingleStep(animationMaxFrameDuration / (atlas->GetAmountOfFrames() * 2));
		animationFrameDurationSpinBox->setToolTip(QString::number(animationMinFrameDuration) + " - " + QString::number(animationMaxFrameDuration) + " (sec)"); 
		animationFrameDurationSpinBox->setRange(animationMinFrameDuration, animationMaxFrameDuration);
        animationFrameDurationSpinBox->blockSignals(false);
	

		animationFPSSpinBox->blockSignals(true);
		animationFPSSpinBox->setSingleStep(animationMaxFPS / (atlas->GetAmountOfFrames() * 2));
		animationFPSSpinBox->setRange(animationMinFPS, animationMaxFPS);
		animationFPSSpinBox->setToolTip(QString::number(animationMinFPS) + " - " + QString::number(animationMaxFPS) + " (frames per second)"); 
		animationFPSSpinBox->blockSignals(false);
	

		animationElapsedTimeSpinBox->blockSignals(true);
		animationElapsedTimeSpinBox->setSingleStep(1.0f);
		animationElapsedTimeSpinBox->setToolTip("0.0 - " + QString::number(animationMaxDuration) + " (sec)");
		animationElapsedTimeSpinBox->setRange(0.0f, animationMaxDuration);
		animationElapsedTimeSpinBox->blockSignals(false);
		SetAtlasFiltering(atlas->GetFilteringMode());
		atlasAmountOfAnimFrameLabel->setText("Amount of frames: " + QString::number(atlas->GetAmountOfFrames()) + "/" + QString::number(Atlas2D::_GetMaxFrames()));
		engine->GetSprite()->BindFrontTexturedAnimation(texturedAnimation);
		engine->GetSprite()->GetMesh()->CreateQuad(Quad(Vector3(), texturedAnimation->GetCurrentFrameSize() / engine->GetPixelsPerMeter()));
		engine->GetSprite()->SetPosition(Vector3());

		animationCurrentFrameSpinBox->blockSignals(true);
		animationCurrentFrameSpinBox->setValue(texturedAnimation->GetCurrentFrame() + 1);
		animationCurrentFrameSpinBox->blockSignals(false);
		AnimationUpdateCurrentFrame(texturedAnimation->GetCurrentFrame());
		if(texturedAnimation->GetFPS() < animationMinFPS)
		{ 
			animationFPSSpinBox->setValue(animationMinFPS);
			animationDurationSpinBox->setValue(animationMaxDuration);
			animationFrameDurationSpinBox->setValue(animationMaxFrameDuration);
		}
		else if(texturedAnimation->GetFPS() > animationMaxFPS)
		{ 
			animationFPSSpinBox->setValue(animationMaxFPS);
			animationDurationSpinBox->setValue(animationMinDuration);
			animationFrameDurationSpinBox->setValue(animationMinFrameDuration);
		}
		else
		{
			animationFPSSpinBox->setValue(texturedAnimation->GetFPS());
			animationDurationSpinBox->setValue(texturedAnimation->GetPlayingDurationInSec());
			animationFrameDurationSpinBox->setValue(texturedAnimation->GetPlayingFrameDurationInSec());
		}
		SetHome();
		hitboxNames.Clear();
		engine->DestroyPoints();
		DestroyAllShapes();

		if(atlas->IsExist())
		{
			for(int32 i = 0; i < atlas->GetAmountOfFrames(); i++)
			{ 
				Hitbox* hitbox = atlas->GetHitbox(i);
				hitboxNames.AddToTail(QString()); 

				if(hitbox)
				{
					hitboxNames.GetTail()->key = hitbox->GetName().c_str();

					animationCurrentFrameSpinBox->blockSignals(true);
					animationCurrentFrameSpinBox->setValue(i + 1);
					animationCurrentFrameSpinBox->blockSignals(false);

					engine->SetHitbox(hitbox);
					 
					hitboxNameLineEdit->setText(hitbox->GetName().c_str());
				}
			}
			animationCurrentFrameSpinBox->setValue(texturedAnimation->GetCurrentFrame() + 1);
		}

		engine->GetScene()->AddToRenderList(engine->GetSprite());
		engine->GetScene()->AddToUpdateList(texturedAnimation);
		
		okButton->setEnabled(true);
	}
	else
	{
		okButton->setEnabled(false);
	}
}


void TextureCreator::AtlasNameIsChanged(QString _name)
{
	atlas->Rename(_name.toStdString());
}



void TextureCreator::AtlasFrameNameIsChanged(QString _name)
{
	atlas->RenameFrame(animationCurrentFrameSpinBox->value() - 1, _name.toStdString());
}



void TextureCreator::CreateMenu(void)
{
	menuBar = new QMenuBar();

	QVBoxLayout* layout = new QVBoxLayout();
    layout->setMenuBar(menuBar);
    setLayout(layout);

	CreateViewMenu();
	CreatePointsMenu();
	CreateLinesMenu();
	CreateSpriteMenu();
	CreateCameraMenu();
	CreateVectorLerpMenu();
	CreateGizmoMenu();
	CreateGridMenu();
}




void TextureCreator::CreateViewMenu(void)
{
	viewMenu = menuBar->addMenu("View");

	CreateDecorationsMenu();

	CreateBackgroundMenu();
} 



void TextureCreator::CreateBackgroundMenu(void)
{
	backgroundMenu = new QMenu(viewMenu);
	backgroundMenu->setTitle("Background");
	viewMenu->addMenu(backgroundMenu);

	CreateBackgroundColorAct();
}


void TextureCreator::CreateBackgroundColorAct(void)
{
	backgroundColorAct = new QAction("Color", backgroundMenu);
	connect(backgroundColorAct, SIGNAL(triggered()), SLOT(ChangeBackgroundColor()));
	backgroundMenu->addAction(backgroundColorAct);
}


void TextureCreator::CreatePointsAntiAliasingAct(void)
{ 
	pointsAntiAliasingAct = new QAction("Anti-aliasing", pointsMenu);
	pointsAntiAliasingAct->setCheckable(true);
	pointsAntiAliasingAct->setChecked(GetPointsAntiAliasingFromFile());
	pointsMenu->addAction(pointsAntiAliasingAct);
	
	engine->SetSmoothEnable(Renderer::POINT, pointsAntiAliasingAct->isChecked());
	connect(pointsAntiAliasingAct, SIGNAL(triggered()), SLOT(ChangePointsAntiAliasing()));
}



bool TextureCreator::GetPointsAntiAliasingFromFile(void)
{
	bool value;

	if(File::_GetBoolKey("engine.config", true, "POINTS", "Anti-aliasing", value))
	{
		return value;
	}
	return false;
}




void TextureCreator::CreateDecorationsMenu(void)
{
	decorationsMenu = new QMenu(viewMenu);
	decorationsMenu->setTitle("Decorations");
	viewMenu->addMenu(decorationsMenu);

	CreateImportDecorationAct();

	CreateClearDecorationsAct();
}


void TextureCreator::CreateImportDecorationAct(void)
{ 
	importDecorationAct = new QAction("Import", decorationsMenu);
	decorationsMenu->addAction(importDecorationAct);
	connect(importDecorationAct, SIGNAL(triggered()), SLOT(ImportDecoration()));
}


void TextureCreator::ImportDecoration(void)
{
	QFileDialog dialog(this, "Choose graphics objects");
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(QString("*.") + AbstractObject::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + AbstractObject::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		for(int i = 0; i < assets.size(); ++i)
		{
			StringANSI name = File::_GetBaseName(assets.at(i).toStdString());

			engine->LoadDecoration(name);
		}
	}
}



void TextureCreator::CreateClearDecorationsAct(void)
{
	clearDecorationsAct = new QAction("Clear", decorationsMenu);
	decorationsMenu->addAction(clearDecorationsAct);
	connect(clearDecorationsAct, SIGNAL(triggered()), SLOT(ClearDecorations()));
}



void TextureCreator::CreateSaveDecorationsAsAct(void)
{
	saveDecorationsAsAct = new QAction("Save as...", decorationsMenu);
	decorationsMenu->addAction(saveDecorationsAsAct);
	connect(saveDecorationsAsAct, SIGNAL(triggered()), SLOT(SaveDecorationsAs()));
}


void TextureCreator::SaveDecorationsAs(void)
{
	if(texture->IsExist())
	{
		if(engine->GetDecorations().IsNotEmpty())
		{
			QString pathFile = QFileDialog::getSaveFileName(this,
													"Save decoration as...",
													"image",
													QString("*png"));

			if(!pathFile.isEmpty())
			{
				Scene* scene3d = new Scene();
				scene3d->SetColor(RGBA(RGBA::INVISIBLE_COLOR));
				scene3d->BindCamera(engine->GetCamera());
				for(MagicList<GraphicsObject>::AddressDerived* it = engine->GetDecorations().GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					scene3d->AddToRenderList(it->GetNode());
				}
				Texture2D* finalTexture = new Texture2D();
				finalTexture->CreateQuadTextureMap(Vector2(texture->GetImageSize().x, texture->GetImageSize().y), RGBA(RGBA::INVISIBLE_COLOR));
				scene3d->BindTexture(finalTexture);
				bool zBufferEnable = Renderer::_GetPicked()->IsZBufferEnabled();
				Renderer::_GetPicked()->SetZBufferEnable(false);
				
				scene3d->Draw();
				Renderer::_GetPicked()->SetZBufferEnable(zBufferEnable);
				PixelImage img;

				if(finalTexture->GetImage(img))
				{	
					img.SaveImageToPNGFile(pathFile.toStdString() + ".png");
				}
	
				delete scene3d;
			}
		}
	}
	else
	{
		ErrorDialog("Error", "Texture is not defined", 145, 40);
	}
}




void TextureCreator::ClearDecorations(void)
{
	engine->ClearDecorations();
}


 

void TextureCreator::ChangePointsAntiAliasing(void)
{
	bool value = pointsAntiAliasingAct->isChecked();

	engine->SetSmoothEnable(Renderer::POINT, value);

	File::_SetBoolKey("engine.config", true, "POINTS", "Anti-aliasing", value);
}




bool TextureCreator::GetLinesAntiAliasingFromFile(void)
{
	bool value;

	if(File::_GetBoolKey("engine.config", true, "LINES", "Anti-aliasing", value))
	{
		return value;
	}
	return false;
}


void TextureCreator::ChangeLinesAntiAliasing(void)
{
	bool value = linesAntiAliasingAct->isChecked();

	engine->SetSmoothEnable(Renderer::LINE, value);

	File::_SetBoolKey("engine.config", true, "LINES", "Anti-aliasing", value);
}




void TextureCreator::CreateCameraMenu(void)
{ 
	cameraMenu = menuBar->addMenu("Camera");

	CreateSaveCameraAsAct();
} 


void TextureCreator::CreateSaveCameraAsAct(void)
{ 
	cameraSaveAsAct = new QAction("Save as...", cameraMenu);
	connect(cameraSaveAsAct, SIGNAL(triggered()), SLOT(SaveCameraAs()));
	cameraMenu->addAction(cameraSaveAsAct);
}
 

void TextureCreator::SaveCameraAs(void)
{
	QString name = "camera";

	QString pathFile = QFileDialog::getSaveFileName(this,
													"Save camera as...",
													(AssetLibrary::_GetDir() + Camera3D::_GetDir()).c_str() + name,
													QString("*") + Camera3D::_GetFileSuffix().c_str());

	if(!pathFile.isEmpty())
	{	
		QString name = File::_GetBaseName(pathFile.toStdString()).c_str();
		if(File::_IsExist(AssetLibrary::_GetDir() + Camera3D::_GetDir() + name.toStdString() + "." + Camera3D::_GetFileSuffix()))
		{ 
			WarningDialog warning("Warning", "Camera '" + name + "' is already exist. Rewrite file?", 200, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				return;
			}
		}

		engine->GetCamera()->SaveAsToFile(name.toStdString());
	}
}




void TextureCreator::CreateSpriteMenu(void)
{
	spriteMenu = menuBar->addMenu("Sprite");

	CreateQuadMenu();

	CreateSaveSpriteAsAct();
} 


void TextureCreator::CreateSaveSpriteAsAct(void)
{ 
	spriteSaveAsAct = new QAction("Save as...", spriteMenu);
	connect(spriteSaveAsAct, SIGNAL(triggered()), SLOT(SaveSpriteAs()));
	spriteMenu->addAction(spriteSaveAsAct);
}



void TextureCreator::SaveSpriteAs(void)
{
	QString name = "sprite";

	QString pathFile = QFileDialog::getSaveFileName(this,
													"Save sprite as...",
													(AssetLibrary::_GetDir() + AbstractObject::_GetDir()).c_str() + name,
													QString("*") + AbstractObject::_GetFileSuffix().c_str());

	if(!pathFile.isEmpty())
	{	
		QString name = File::_GetBaseName(pathFile.toStdString()).c_str();
		if(File::_IsExist(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + name.toStdString() + "." + AbstractObject::_GetFileSuffix()))
		{ 
			WarningDialog warning("Warning", "Object '" + name + "' is already exist. Rewrite file?", 200, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				return;
			}
		}
 
		if(engine->GetShapes().IsNotEmpty())
		{ 
			if(editorMode == TEXTURE_EDITOR)
			{ 
				Hitbox* hitbox = new Hitbox();  
				hitbox->Rename(hitboxNameLineEdit->text().toStdString());
				for(int i = 0; i < engine->GetShapes().GetSize(); i++)
				{
					TextureShapeListWidget::ItemShape* it = dynamic_cast<TextureShapeListWidget::ItemShape*>(shapeList->topLevelItem(i));

					if(it)
					{ 
						StringANSI shapeName = it->text(TextureShapeListWidget::ItemShape::NAME_PROPERTY).toStdString();
						int32 collisionGroupIndex = it->text(TextureShapeListWidget::ItemShape::COLLISION_GROUP_PROPERTY).toInt();
						bool collisionEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_ENABLE_PROPERTY) == "true" ? true : false;
						bool collisionPolygonsInfoEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_POLYGONS_INFO_PROPERTY) == "true" ? true : false;
						Mesh* mesh = engine->GetShapes().Get(i);
						mesh->SetBoundingVolumeType(engine->GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString()));
						
						if(mesh)
						{
							if(AssetLibrary::_IsAssetExist(mesh->GetName(), AssetLibrary::MESH_ASSET))
							{
								AssetLibrary::_GetAssets().AddObject(mesh);
							}
							hitbox->AddShape(mesh, shapeName, collisionGroupIndex, collisionEnable, collisionPolygonsInfoEnable);
						} 
					} 
				}

				if(AssetLibrary::_IsAssetExist(hitbox->GetName(), AssetLibrary::HITBOX_ASSET))
				{
					AssetLibrary::_GetAssets().AddObject(hitbox);
				}
							
				texture->BindHitbox(hitbox);
			} 
			else if(editorMode == TEXTURED_ANIMATION_EDITOR)
			{
				if(atlas->IsExist())
				{
					for(int j = 0; j < atlas->GetAmountOfFrames(); j++)
					{
						Hitbox* hitbox = new Hitbox();
						hitbox->Rename(hitboxNames.Get(j)->key.toStdString());
						for(int i = 0; i < engine->GetShapes().GetSize(); i++)
						{
							TextureViewer::GraphicsSurfaceEx* object = engine->GetObjects().Get(i);

							if(object && object->GetFrameIndex() == j)
							{
								TextureShapeListWidget::ItemShape* it = dynamic_cast<TextureShapeListWidget::ItemShape*>(shapeList->topLevelItem(i));

								if(it)
								{
									StringANSI shapeName = it->text(TextureShapeListWidget::ItemShape::NAME_PROPERTY).toStdString();
									int32 collisionGroupIndex = it->text(TextureShapeListWidget::ItemShape::COLLISION_GROUP_PROPERTY).toInt();
									bool collisionEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_ENABLE_PROPERTY) == "true" ? true : false;
									bool collisionPolygonsInfoEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_POLYGONS_INFO_PROPERTY) == "true" ? true : false;
									Mesh* mesh = engine->GetShapes().Get(i);
									mesh->SetBoundingVolumeType(engine->GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString()));
									
									if(mesh)
									{
										if(AssetLibrary::_IsAssetExist(mesh->GetName(), AssetLibrary::MESH_ASSET))
										{
											AssetLibrary::_GetAssets().AddObject(mesh);
										}
										hitbox->AddShape(mesh, shapeName, collisionGroupIndex, collisionEnable, collisionPolygonsInfoEnable);
									} 
								} 
							}
						}
			
						if(hitbox->GetShapes().IsNotEmpty())
						{ 
							if(AssetLibrary::_IsAssetExist(hitbox->GetName(), AssetLibrary::HITBOX_ASSET))
							{
								AssetLibrary::_GetAssets().AddObject(hitbox);
							}
							
							texturedAnimation->GetAtlas()->BindHitbox(j, hitbox);
						}
						else
						{
							delete hitbox;
						}
					}
				}
			}
		}

		if(editorMode == TEXTURE_EDITOR)
		{
			if(AssetLibrary::_IsAssetExist(texture->GetName(), AssetLibrary::TEXTURE_2D_ASSET))
			{
				AssetLibrary::_GetAssets().AddObject(texture);
			}
		}
		else if(editorMode == TEXTURED_ANIMATION_EDITOR)
		{
			if(AssetLibrary::_IsAssetExist(texturedAnimation->GetName(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
			{
				AssetLibrary::_GetAssets().AddObject(texturedAnimation);
			}
		} 

		engine->GetSprite()->SaveAsToFile(name.toStdString());
	} 
}



void TextureCreator::CreateQuadMenu(void)
{
	quadMenu = new QMenu(spriteMenu);
	quadMenu->setTitle("Quad");
	spriteMenu->addMenu(quadMenu);

	CreateQuadShowAct();

	CreateQuadImportAct();

	CreateSaveQuadAsAct();
}


void TextureCreator::CreateQuadShowAct(void)
{ 
	quadShowAct = new QAction("Show", quadMenu);
	quadShowAct->setCheckable(true);
	quadShowAct->setChecked(true);
	connect(quadShowAct, SIGNAL(triggered()), SLOT(ChangeQuadShow()));
	quadMenu->addAction(quadShowAct);
}



void TextureCreator::ChangeQuadShow(void)
{
	quadShowEnable = !quadShowEnable;
}



void TextureCreator::CreateSaveQuadAsAct(void)
{ 
	quadSaveAsAct = new QAction("Save as...", quadMenu);
	connect(quadSaveAsAct, SIGNAL(triggered()), SLOT(SaveQuadAs()));
	quadMenu->addAction(quadSaveAsAct);
}



void TextureCreator::CreateQuadImportAct(void)
{ 
	quadImportAct = new QAction("Import", quadMenu);
	connect(quadImportAct, SIGNAL(triggered()), SLOT(QuadImport()));
	quadMenu->addAction(quadImportAct);
}


void TextureCreator::QuadImport(void)
{
	QFileDialog dialog(this, "Choose mesh");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + Mesh::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + Mesh::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		StringANSI name = File::_GetBaseName(assets.at(0).toStdString());

		engine->GetSprite()->BindMesh(Mesh::_LoadFromFile(name));
	}
}


void TextureCreator::SaveVectorLerpFloatLerpAs(void)
{
	QString name = vectorLerpFloatLerpNameLineEdit->text();

	QString pathFile = QFileDialog::getSaveFileName(this,
													"Save float lerp as...",
													(AssetLibrary::_GetDir() + FloatLerp::_GetDir()).c_str() + name,
													QString("*") + FloatLerp::_GetFileSuffix().c_str());

	if(!pathFile.isEmpty())
	{	
		StringANSI name = File::_GetBaseName(pathFile.toStdString());

		engine->GetVectorLerp()->GetFloatLerp()->SaveAsToFile(name);
	}
}


void TextureCreator::SaveQuadAs(void)
{
	if((editorMode == TEXTURE_EDITOR && texture->IsExist()) || (editorMode == TEXTURED_ANIMATION_EDITOR && texturedAnimation->IsExist()))
	{
		QString name = "quad";

		QString pathFile = QFileDialog::getSaveFileName(this,
														"Save mesh as...",
														(AssetLibrary::_GetDir() + Mesh::_GetDir()).c_str() + name,
														QString("*") + Mesh::_GetFileSuffix().c_str());

		if(!pathFile.isEmpty())
		{	
			QString name = File::_GetBaseName(pathFile.toStdString()).c_str();
			if(File::_IsExist(AssetLibrary::_GetDir() + Mesh::_GetDir() + name.toStdString() + "." + Mesh::_GetFileSuffix()))
			{ 
				WarningDialog warning("Warning", "Mesh '" + name + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					return;
				}
			}

			engine->GetSprite()->GetMesh()->SaveAsToFile(name.toStdString());
		}
	}
	else
	{
		ErrorDialog("Error", "Mesh is not defined", 145, 40);
	}
}



void TextureCreator::CreatePointsMenu(void)
{
	pointsMenu = menuBar->addMenu("Points");

	CreatePointsShowAct();

	CreatePointsImportAct();

	CreateSavePointsAsAct();

	CreatePointsAntiAliasingAct();
}


void TextureCreator::CreatePointsSizeAct(void)
{ 
	verticesSizeAct = new QAction("Size", pointsMenu);
	connect(verticesSizeAct, SIGNAL(triggered()), SLOT(ChangePointsSize()));
	pointsMenu->addAction(verticesSizeAct);
}



void TextureCreator::CreatePointsImportAct(void)
{ 
	verticesShowAct = new QAction("Import", pointsMenu);
	connect(verticesShowAct, SIGNAL(triggered()), SLOT(ImportPoints()));
	pointsMenu->addAction(verticesShowAct);
}

 

void TextureCreator::CreateSavePointsAsAct(void)
{ 
	verticesSaveAsAct = new QAction("Save as...", pointsMenu);
	connect(verticesSaveAsAct, SIGNAL(triggered()), SLOT(SavePointsAs()));
	pointsMenu->addAction(verticesSaveAsAct);
}



void TextureCreator::CreatePointsShowAct(void)
{ 
	verticesShowAct = new QAction("Show", pointsMenu);
	verticesShowAct->setCheckable(true);
	verticesShowAct->setChecked(true);
	connect(verticesShowAct, SIGNAL(triggered()), SLOT(ChangePointsShow()));
	pointsMenu->addAction(verticesShowAct);
}


void TextureCreator::ChangePointsShow(void)
{
	verticesShowEnable = !verticesShowEnable;
}



void TextureCreator::CreateVectorLerpMenu(void)
{
	vectorLerpMenu = menuBar->addMenu("Vector lerp");

	CreateVectorLerpShowCurveAct();
}



void TextureCreator::CreateVectorLerpShowCurveAct(void)
{ 
	vectorLerpShowCurveAct = new QAction("Show curve", vectorLerpMenu);
	vectorLerpShowCurveAct->setCheckable(true);
	vectorLerpShowCurveAct->setChecked(true); 
	connect(vectorLerpShowCurveAct, SIGNAL(triggered()), SLOT(ChangeVectorLerpShowCurve()));
	vectorLerpMenu->addAction(vectorLerpShowCurveAct);
}


void TextureCreator::ChangeVectorLerpShowCurve(void)
{
	vectorLerpShowCurveEnable = !vectorLerpShowCurveEnable;
}


void TextureCreator::CreateGridMenu(void)
{
	gridMenu = menuBar->addMenu("Grid");

	CreateSpaceUnitsAct();
		
	CreateGridShowAct();
}




void TextureCreator::CreateGridShowAct(void)
{ 
	gridShowAct = new QAction("Show", gridMenu);
	gridShowAct->setCheckable(true);
	connect(gridShowAct, SIGNAL(triggered()), SLOT(ChangeGridShow()));
	gridMenu->addAction(gridShowAct);
}


void TextureCreator::ChangeGridShow(void)
{
	gridShowEnable = !gridShowEnable;
}


void TextureCreator::CreateSpaceUnitsAct(void)
{ 
	spaceUnitsAct = new QAction("Space units", gridMenu);
	connect(spaceUnitsAct, SIGNAL(triggered()), SLOT(ChangeSpaceUnits()));
	gridMenu->addAction(spaceUnitsAct);
}


void TextureCreator::ChangePointsSize(void)
{ 
	PointsSizeDialog dialog(this);

	if(dialog.exec() == QDialog::Accepted)
	{}
}



void TextureCreator::ChangeSpaceUnits(void)
{
	SpaceUnitsDialog dialog(this);

	if(dialog.exec() == QDialog::Accepted)
	{
		engine->SetPixelsPerMeter(dialog.GetPixelsPerMeter());
		
		if(editorMode == TEXTURE_EDITOR)
		{
			if(texture->IsExist())
			{
				engine->GetSprite()->GetMesh()->CreateQuad(Quad(Vector3(), texture->GetImageSize() / engine->GetPixelsPerMeter()));
			}
		}
		else if(editorMode == TEXTURED_ANIMATION_EDITOR)
		{
			if(texturedAnimation->IsExist())
			{
				engine->GetSprite()->GetMesh()->CreateQuad(Quad(Vector3(), texturedAnimation->GetCurrentFrameSize() / engine->GetPixelsPerMeter()));
			}
		}

		SetHome();
	}
}



void TextureCreator::CreateGizmoMenu(void)
{
	gizmoMenu = menuBar->addMenu("Gizmo");

	CreateGizmoShowAct();
}


void TextureCreator::CreateGizmoShowAct(void)
{ 
	gizmoShowAct = new QAction("Show", gizmoMenu);
	gizmoShowAct->setCheckable(true);
	gizmoShowAct->setChecked(true);
	connect(gizmoShowAct, SIGNAL(triggered()), SLOT(ChangeGizmoShow()));
	gizmoMenu->addAction(gizmoShowAct);
}


void TextureCreator::ChangeGizmoShow(void)
{
	gizmoXYShowEnable = !gizmoXYShowEnable;
}



void TextureCreator::CreateLinesMenu(void)
{
	linesMenu = menuBar->addMenu("Lines");
 
	CreateLinesShowAct();

	CreateLinesAntiAliasingAct();
}



void TextureCreator::CreateLinesAntiAliasingAct(void)
{ 
	linesAntiAliasingAct = new QAction("Anti-aliasing", linesMenu);
	linesAntiAliasingAct->setCheckable(true);
	linesAntiAliasingAct->setChecked(GetLinesAntiAliasingFromFile());
	linesMenu->addAction(linesAntiAliasingAct);

	engine->SetSmoothEnable(Renderer::LINE, linesAntiAliasingAct->isChecked());
	connect(linesAntiAliasingAct, SIGNAL(triggered()), SLOT(ChangeLinesAntiAliasing()));
}


void TextureCreator::CreateLinesWidthAct(void)
{ 
	edgesWidthAct = new QAction("Width", linesMenu);
	connect(edgesWidthAct, SIGNAL(triggered()), SLOT(ChangeLinesWidth()));
	linesMenu->addAction(edgesWidthAct);
}



void TextureCreator::ChangeLinesWidth(void)
{ 
	LinesWidthDialog dialog(this);

	if(dialog.exec() == QDialog::Accepted)
	{}
}



void TextureCreator::CreateLinesShowAct(void)
{ 
	edgesShowAct = new QAction("Show", linesMenu);
	edgesShowAct->setCheckable(true);
	edgesShowAct->setChecked(true);
	connect(edgesShowAct, SIGNAL(triggered()), SLOT(ChangeLinesShow()));
	linesMenu->addAction(edgesShowAct);
}


void TextureCreator::ChangeLinesShow(void)
{
	edgesShowEnable = !edgesShowEnable;
}


 
bool TextureCreator::IsViewportFocus(void)const
{
	return viewport->hasFocus();
}


bool TextureCreator::IsHitboxEnabled(void)const
{
	return hitboxEnableCheckBox->isChecked();
}



bool TextureCreator::IsQuadShowEnabled(void)const
{
	return quadShowEnable;
}


bool TextureCreator::IsVectorLerpShowCurveEnabled(void)const
{
	return vectorLerpShowCurveEnable;
}


bool TextureCreator::IsGizmoXYShowEnabled(void)const
{
	return gizmoXYShowEnable;
}



bool TextureCreator::IsGridShowEnabled(void)const
{
	return gridShowEnable;
}


bool TextureCreator::IsPointsShowEnabled(void)const
{
	return verticesShowEnable;
}


bool TextureCreator::IsLinesShowEnabled(void)const
{
	return edgesShowEnable;
}



bool TextureCreator::IsShapeLocked(void)const
{
	return lockCheckBox->isChecked();
}


QString TextureCreator::GetCurrentBuildMode(void)const
{
	return buildModeComboBox->itemText(buildModeComboBox->currentIndex());
}



void TextureCreator::SetShapeLock(int _enable)
{
	if(_enable)
	{
		engine->RelockSelectedShape();
	}
	else
	{
		int32 index = shapeList->GetSelectedShapeIndex();
		
		if(index != -1)
		{
			engine->UnlockShape(index);
			if(brushComboBox->itemText(brushComboBox->currentIndex()) == "CIRCLE" ||
			   brushComboBox->itemText(brushComboBox->currentIndex()) == "QUAD")
			{
				brushComboBox->setCurrentIndex(0);
			}
		}
	}
}


void TextureCreator::DestroyShape(int32 _index)
{
	delete shapeList->takeTopLevelItem(_index);
					
	delete engine->GetShapes().Get(_index);
	delete engine->GetObjects().Get(_index);
}


void TextureCreator::ChangeSelectShape(void)
{
	lockCheckBox->blockSignals(true);

	lockCheckBox->setEnabled(false);
	lockCheckBox->setChecked(false);
	for(int32 i = 0; i < engine->GetObjects().GetSize(); i++)
	{
		engine->GetObjects().Get(i)->SetCurrent(false);
	}


	int32 index = shapeList->GetSelectedShapeIndex();

	if(index != -1)
	{
		engine->GetObjects().Get(index)->SetCurrent(true);

		if(editorMode == TEXTURED_ANIMATION_EDITOR)
		{
			animationCurrentFrameSpinBox->setValue(engine->GetObjects().Get(index)->GetFrameIndex() + 1);
		}

		lockCheckBox->setEnabled(true);
		lockCheckBox->setChecked(true);
		 
		engine->RelockSelectedShape();
	}  
	else
	{ 
		engine->DestroyPoints();
	}

	lockCheckBox->blockSignals(false);
}


void TextureCreator::FreezeLastSelectedShape(void)
{
	if(engine->GetEditingObject() == NIL)
	{
		shapeList->ResetSelectedShape();
	}
}


void TextureCreator::DestroyAllShapes(void)
{
	engine->GetShapes().Clear(MagicContainer::DELETE_NODE);
	engine->GetObjects().Clear(MagicContainer::DELETE_NODE);
	shapeList->clear();
}


TextureShapeListWidget::ItemShape* TextureCreator::GetSelectedShape(void)
{
	return shapeList->GetSelectedShape();
}


void TextureCreator::RemoveSelectedShape(void)
{
	if(shapeList->GetSelectedShape())
	{
		int index = shapeList->GetSelectedShapeIndex();
		shapeList->RemoveSelectedShape();

		delete engine->GetObjects().Get(index);
		delete engine->GetShapes().Get(index);
	}
	else if(engine->GetSelectedPoints().IsNotEmpty())
	{
		engine->DestroySelectedPoints();
	}
}


void TextureCreator::ImportVectorLerpFloatLerp(void)
{
	QFileDialog dialog(this, "Choose float lerp");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + FloatLerp::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + FloatLerp::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		StringANSI name = File::_GetBaseName(assets.at(0).toStdString());

		engine->LoadVectorLerpFloatLerp(name);
	}
}


void TextureCreator::ImportHitbox(void)
{
	QFileDialog dialog(this, "Choose hitbox");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + Hitbox::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + Hitbox::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		StringANSI name = File::_GetBaseName(assets.at(0).toStdString());

		Hitbox* hitbox = Hitbox::_LoadFromFile(name);

		engine->SetHitbox(hitbox);

		delete hitbox;
 
		hitboxNameLineEdit->setText(name.c_str());
	}
}



void TextureCreator::ImportVectorLerp(void)
{
	QFileDialog dialog(this, "Choose vector lerp");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + VectorLerp::_GetFileSuffix().c_str());
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory((AssetLibrary::_GetDir() + VectorLerp::_GetDir()).c_str());


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		StringANSI name = File::_GetBaseName(assets.at(0).toStdString());
		
		engine->LoadVectorLerp(name);

		vectorLerpModeComboBox->blockSignals(true);
		vectorLerpSegmentsSpinBox->blockSignals(true);
		vectorLerpNameLineEdit->blockSignals(true);
		vectorLerpNameLineEdit->setText(name.c_str());
		vectorLerpCurrentXSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().x);
		vectorLerpCurrentYSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().y);
		vectorLerpCurrentZSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().z);
		vectorLerpDeltaXSpinBox->setValue(engine->GetVectorLerp()->GetDelta().x);
		vectorLerpDeltaYSpinBox->setValue(engine->GetVectorLerp()->GetDelta().y);
		vectorLerpDeltaZSpinBox->setValue(engine->GetVectorLerp()->GetDelta().z);
		vectorLerpSegmentsSpinBox->setValue(engine->GetVectorLerp()->GetAmountOfSegments());
		vectorLerpModeComboBox->setCurrentIndex(GetVectorLerpMode(engine->GetVectorLerp()->GetLerpMode()));
		

		vectorLerpModeComboBox->blockSignals(false);
		vectorLerpSegmentsSpinBox->blockSignals(false);
	    vectorLerpNameLineEdit->blockSignals(false);
	}
}


void TextureCreator::SetHitbox(QString _name)
{
	Hitbox* hitbox = Hitbox::_LoadFromFile(_name.toStdString());

	engine->SetHitbox(hitbox);

	delete hitbox;

	hitboxNameLineEdit->setText(_name);

	setWindowTitle("Hitbox Editor");
}



void TextureCreator::SetVectorLerp(QString _name)
{
	engine->LoadVectorLerp(_name.toStdString());
  
	vectorLerpModeComboBox->blockSignals(true);
	vectorLerpSegmentsSpinBox->blockSignals(true);
	vectorLerpNameLineEdit->blockSignals(true);
	vectorLerpNameLineEdit->setText(_name);
	vectorLerpCurrentXSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().x);
	vectorLerpCurrentYSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().y);
	vectorLerpCurrentZSpinBox->setValue(engine->GetVectorLerp()->GetCurrent().z);
	vectorLerpDeltaXSpinBox->setValue(engine->GetVectorLerp()->GetDelta().x);
	vectorLerpDeltaYSpinBox->setValue(engine->GetVectorLerp()->GetDelta().y);
	vectorLerpDeltaZSpinBox->setValue(engine->GetVectorLerp()->GetDelta().z);
	vectorLerpSegmentsSpinBox->setValue(engine->GetVectorLerp()->GetAmountOfSegments());
	vectorLerpModeComboBox->setCurrentIndex(GetVectorLerpMode(engine->GetVectorLerp()->GetLerpMode()));
	

	vectorLerpModeComboBox->blockSignals(false);
	vectorLerpSegmentsSpinBox->blockSignals(false);
    vectorLerpNameLineEdit->blockSignals(false);


	setWindowTitle("Vector Lerp Editor");
}



void TextureCreator::SetFloatLerp(QString _name)
{
	engine->LoadVectorLerpFloatLerp(_name.toStdString());
   
	setWindowTitle("Float Lerp Editor");
}



void TextureCreator::SetAtlas(QString _name)
{
	ReloadAtlas(_name);

	setWindowTitle("Atlas Editor");
}


void TextureCreator::SetAtlasEditor(void)
{
	editorMode = TEXTURED_ANIMATION_EDITOR;

	loadTextureButton->hide();
	nameLabel->move(10, 10+20);
	nameLineEdit->move(50, 10+20);
    texturedAnimationPropertyScrollArea = new QScrollArea(propertiesTab);
    texturedAnimationPropertyScrollArea->setGeometry(QRect(5, 5, 386+30, 291+100+85+25-40+60+30));
	texturedAnimationPropertyScrollAreaWidget = new QWidget(texturedAnimationPropertyScrollArea);
    texturedAnimationPropertyScrollAreaWidget->setGeometry(QRect(0, 0, 386-20+30, 291+100+85+25-45+60+30+30+30));
	texturedAnimationPropertyScrollArea->setWidget(texturedAnimationPropertyScrollAreaWidget);
	animationGroupBox = new QGroupBox(texturedAnimationPropertyScrollAreaWidget);
	animationGroupBox->setGeometry(QRect(10, 10, 386-10-10+5+7, 111+50+150+100+100-160-100-70-40+70-10-5));
    animationGroupBox->setTitle("Abstract animation:");
	animationFPSLabel = new QLabel(animationGroupBox);
	animationFPSLabel->setGeometry(QRect(10+5, 20, 31, 21));
	animationFPSLabel->setText("FPS:");

	animationFPSSpinBox = new FloatSpinBox(animationGroupBox);
	animationFPSSpinBox->setGeometry(QRect(90+10+5-10, 20, 81+10, 22));
	animationFPSSpinBox->setEnabled(false);
	connect(animationFPSSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AnimationFPSIsChanged(double)));
	animationFPSSpinBox->setDecimals(4);
	animationDurationLabel = new QLabel(animationGroupBox);
	animationDurationLabel->setGeometry(QRect(10+5, 50, 71, 21));
	animationDurationLabel->setText("Duration:");


	animationDurationSpinBox = new FloatSpinBox(animationGroupBox);
	animationDurationSpinBox->setGeometry(QRect(90+10+5-10, 50, 81+10, 22));
	animationDurationSpinBox->setEnabled(false);
	animationDurationSpinBox->setDecimals(4);
	connect(animationDurationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AnimationDurationIsChanged(double)));
	animationFrameDurationLabel = new QLabel(animationGroupBox);
	animationFrameDurationLabel->setGeometry(QRect(10+5, 80, 81, 21));
	animationFrameDurationLabel->setText("Frame duration:");
       
	animationFrameDurationSpinBox = new FloatSpinBox(animationGroupBox);
	animationFrameDurationSpinBox->setGeometry(QRect(90+10+5-10, 80, 81+10, 22));
	animationFrameDurationSpinBox->setEnabled(false);
	animationFrameDurationSpinBox->setDecimals(4);
	connect(animationFrameDurationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AnimationFrameDurationIsChanged(double)));
	animationElapsedTimeLabel = new QLabel(animationGroupBox);
    animationElapsedTimeLabel->setGeometry(QRect(10+5, 110, 91, 21));
	animationElapsedTimeLabel->setText("Elapsed time:");

    animationElapsedTimeSpinBox = new FloatSpinBox(animationGroupBox);
    animationElapsedTimeSpinBox->setGeometry(QRect(90+10+5-10, 110, 81+10, 22));
	animationElapsedTimeSpinBox->setDecimals(4);
	animationElapsedTimeSpinBox->setEnabled(false);
	connect(animationElapsedTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AnimationElapsedTimeIsChanged(double)));
	animationPlayingModeGroupBox = new QGroupBox(animationGroupBox);
    animationPlayingModeGroupBox->setGeometry(QRect(150+25+15+10-10+5, 20-5, 241-40-20-15+2+7, 131-30+7+8));
	animationPlayingModeGroupBox->setTitle("Playing mode:");
	QButtonGroup* group1 = new QButtonGroup(animationPlayingModeGroupBox);
	QButtonGroup* group2 = new QButtonGroup(animationPlayingModeGroupBox);
	QButtonGroup* group3 = new QButtonGroup(animationPlayingModeGroupBox);
	animationForwardRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationForwardRadioButton->setGeometry(QRect(30-20, 30-20+7+6, 71, 21));
	animationForwardRadioButton->setText("Forward");
	connect(animationForwardRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationForwardPlayingModeEnable(bool)));
	group1->addButton(animationForwardRadioButton);
	animationBackwardRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationBackwardRadioButton->setGeometry(QRect(140-40-10+3, 30-20+7+6, 71, 21));
	animationBackwardRadioButton->setText("Backward");
    connect(animationBackwardRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationBackwardPlayingModeEnable(bool)));
	group1->addButton(animationBackwardRadioButton);
	animationPingPongRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationPingPongRadioButton->setGeometry(QRect(30-20, 60-20+7+6, 71, 21));
	animationPingPongRadioButton->setText("Ping pong");
    connect(animationPingPongRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationPingPongPlayingModeEnable(bool)));
	group2->addButton(animationPingPongRadioButton);
	animationOnewayRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationOnewayRadioButton->setGeometry(QRect(140-40-10+3, 60-20+7+6, 71, 21));
	animationOnewayRadioButton->setText("Oneway");
    connect(animationOnewayRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationOnewayPlayingModeEnable(bool)));
	group2->addButton(animationOnewayRadioButton);
	animationSingleShotRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationSingleShotRadioButton->setGeometry(QRect(140-40-10+3, 90-20+7+6, 81, 21));
	animationSingleShotRadioButton->setText("Single shot");
 	connect(animationSingleShotRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationSingleShotPlayingModeEnable(bool)));
	group3->addButton(animationSingleShotRadioButton);
	animationLoopRadioButton = new QRadioButton(animationPlayingModeGroupBox);
    animationLoopRadioButton->setGeometry(QRect(30-20, 90-20+7+6, 71, 21));
	animationLoopRadioButton->setText("Loop");
    connect(animationLoopRadioButton, SIGNAL(toggled(bool)), this, SLOT(AnimationLoopPlayingModeEnable(bool)));
	group3->addButton(animationLoopRadioButton);
    

	animationForwardRadioButton->setChecked(true);
	animationBackwardRadioButton->setChecked(false);
	animationOnewayRadioButton->setChecked(true);
	animationPingPongRadioButton->setChecked(false);
	animationSingleShotRadioButton->setChecked(true);
	animationLoopRadioButton->setChecked(false);
    animationCurrentFrameLine = new QFrame(animationGroupBox);
    animationCurrentFrameLine->setGeometry(QRect(10+5, 140-5, 120+150+50+20-2+15, 20));
    animationCurrentFrameLine->setFrameShape(QFrame::HLine);
    animationCurrentFrameLine->setFrameShadow(QFrame::Sunken);
	animationCurrentFrameLabel = new QLabel(animationGroupBox);
    animationCurrentFrameLabel->setGeometry(QRect(10+5, 170-10-4, 71, 31));
    animationCurrentFrameLabel->setText("Current frame:"); 

	animationCurrentFrameSpinBox = new QSpinBox(animationGroupBox);
    animationCurrentFrameSpinBox->setGeometry(QRect(90+10+5-10, 170-10, 51, 21));
	animationCurrentFrameSpinBox->setRange(1, 1);
	animationCurrentFrameSpinBox->setToolTip(QString::number(0) + " - " + QString::number(0));
	animationCurrentFrameSpinBox->setEnabled(false);
	connect(animationCurrentFrameSpinBox, SIGNAL(valueChanged(int)), this, SLOT(AnimationCurrentFrameIsChanged(int)));
	animationPlayButton = new QPushButton(animationGroupBox);
	animationPlayButton->setGeometry(QRect(10+80+5+12+75+10, 170-10, 71, 21));
	animationPlayButton->setText("Play");
	connect(animationPlayButton, SIGNAL(clicked()), this, SLOT(PlayAnimation()));
	animationStopButton = new QPushButton(animationGroupBox);
	animationStopButton->setGeometry(QRect(10+80+5+12+75+80+10, 170-10, 71, 21));
	animationStopButton->setText("Stop");
	connect(animationStopButton, SIGNAL(clicked()), this, SLOT(StopAnimation()));
	texturedAnimationGroupBox = new QGroupBox(texturedAnimationPropertyScrollAreaWidget);
	texturedAnimationGroupBox->setGeometry(QRect(10, 200-40-20+10+10+70-20, 386-10-10+5+7, 60-5+30));
	texturedAnimationGroupBox->setTitle("Textured animation:");
    texturedAnimationPositionOffsetEnableLabel = new QLabel(texturedAnimationGroupBox);
    texturedAnimationPositionOffsetEnableLabel->setGeometry(15, 10+10, 40+60+15, 21);
	texturedAnimationPositionOffsetEnableLabel->setText("Position offset enable:");

	texturedAnimationPositionOffsetEnableCheckBox = new QCheckBox(texturedAnimationGroupBox); 
    texturedAnimationPositionOffsetEnableCheckBox->setGeometry(QRect(10+40+60+15+10-3+5, 10+10, 21, 21));
	connect(texturedAnimationPositionOffsetEnableCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TexturedAnimationPositionOffsetEnable(int)));
	texturedAnimationPositionOffsetTemplateButton = new QPushButton(texturedAnimationGroupBox);
    texturedAnimationPositionOffsetTemplateButton->setGeometry(QRect(10+40+60+15+15-3+20+10+10, 20, 71, 21));
	texturedAnimationPositionOffsetTemplateButton->setText("Template");
	texturedAnimationPositionOffsetTemplateButton->setEnabled(false);
    animationShowPreviousFrameLabel = new QLabel(texturedAnimationGroupBox);
    animationShowPreviousFrameLabel->setGeometry(15, 50, 40+60+15, 21);
	animationShowPreviousFrameLabel->setText("Show previous position:");

	animationShowPreviousFrameCheckBox = new QCheckBox(texturedAnimationGroupBox); 
    animationShowPreviousFrameCheckBox->setGeometry(QRect(10+40+60+15+10-3+5, 50, 21, 21));
	animationShowPreviousFrameCheckBox->setChecked(true);
	connect(animationShowPreviousFrameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AnimationShowPreviousPositionOffset(int)));
	atlasGroupBox = new QGroupBox(texturedAnimationPropertyScrollAreaWidget);
	atlasGroupBox->setGeometry(QRect(10, 200-40-20+10+10+70+30+10+30, 386-10-10+5+7, 111+50+150+100+100-200-23-7+3));
	atlasGroupBox->setTitle("Atlas:");
	atlasNameLabel = new QLabel(atlasGroupBox);
	atlasNameLabel->setGeometry(QRect(20-10+5, 20, 41, 21));
	atlasNameLabel->setText("Name:");

	atlasNameLineEdit = new QLineEdit(atlasGroupBox);
	atlasNameLineEdit->setGeometry(QRect(70-5-10, 20, 161, 20));
	atlasNameLineEdit->setMaxLength(nameMaxLength);
	atlasNameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
    connect(atlasNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(AtlasNameIsChanged(QString)));
	animationCreateAtlasButton = new QPushButton(atlasGroupBox);
    animationCreateAtlasButton->setGeometry(QRect(70+161+20-5-10-10, 20, 61-5+10, 22));
	connect(animationCreateAtlasButton, SIGNAL(clicked()), this, SLOT(AnimationCreateAtlas())); 
	animationCreateAtlasButton->setText("Create");
	animationImportAtlasButton = new QPushButton(atlasGroupBox);
    animationImportAtlasButton->setGeometry(QRect(70+161+20-5+60+10-10-10-10+5+10, 20, 61-5+10, 22)); 
	animationImportAtlasButton->setText("Import");  
	animationImportAtlasButton->setToolTip("Import atlas");
	connect(animationImportAtlasButton, SIGNAL(clicked()), this, SLOT(ImportAtlas()));
	animationSaveAtlasAsButton = new QPushButton(atlasGroupBox);
    animationSaveAtlasAsButton->setGeometry(QRect(70+161+20-5+60+10-10-10-10+5+10, 20+30, 61-5+10, 22)); 
	animationSaveAtlasAsButton->setText("Save as...");  
	animationSaveAtlasAsButton->setToolTip("Save atlas as...");
	connect(animationSaveAtlasAsButton, SIGNAL(clicked()), this, SLOT(SaveAtlasAs()));
	atlasFilteringLabel = new QLabel(atlasGroupBox);
    atlasFilteringLabel->setGeometry(QRect(15, 50, 91, 21));
	atlasFilteringLabel->setText("Texture filtering:");

	atlasFilteringComboBox = new QComboBox(atlasGroupBox);
    atlasFilteringComboBox->setGeometry(QRect(10+80+5+12, 46+5, 108, 21));
	atlasFilteringComboBox->addItem("POINT");
	atlasFilteringComboBox->addItem("BILINEAR");
	atlasFilteringComboBox->addItem("TRILINEAR");
	atlasFilteringComboBox->addItem("ANISOTROPY");
	atlasFilteringComboBox->setCurrentIndex(0);
	connect(atlasFilteringComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(AtlasFilteringIsChanged(QString)));
    atlasAmountOfAnimFrameLabel = new QLabel(atlasGroupBox);
    atlasAmountOfAnimFrameLabel->setGeometry(QRect(15, 80, 121+10, 21));
	atlasAmountOfAnimFrameLabel->setText("Amount of frames: 0/0");
	atlasFrameGroupBox = new QGroupBox(atlasGroupBox);
	atlasFrameGroupBox->setGeometry(QRect(15-3, 130+10-30, 150+100+70+10+10+10+7, 190-20+30-15-25+5));
	atlasFrameGroupBox->setTitle("Frame:");
	atlasFrameNameLabel = new QLabel(atlasFrameGroupBox);
	atlasFrameNameLabel->setGeometry(QRect(10, 20, 41, 21));
	atlasFrameNameLabel->setText("Name:");

	atlasFrameNameLineEdit = new QLineEdit(atlasFrameGroupBox);
	atlasFrameNameLineEdit->setGeometry(QRect(60-5-5, 20, 161-10, 20));
	atlasFrameNameLineEdit->setMaxLength(nameMaxLength);
	atlasFrameNameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	atlasFrameNameLineEdit->setEnabled(false);
    connect(atlasFrameNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(AtlasFrameNameIsChanged(QString)));
	
	
	sizeMin = 1;
	sizeMax = Texture2D::_GetTextureMaxSize();
	atlasFrameWidthLabel = new QLabel(atlasFrameGroupBox);
    atlasFrameWidthLabel->setGeometry(QRect(10, 50, 41+30, 21));
	atlasFrameWidthLabel->setText("Width:"); 

	atlasFrameWidthSpinBox = new QSpinBox(atlasFrameGroupBox);
    atlasFrameWidthSpinBox->setGeometry(QRect(60-5-5, 50, 71, 22));
	atlasFrameWidthSpinBox->setRange(sizeMin, sizeMax);
	atlasFrameWidthSpinBox->setToolTip(QString::number(sizeMin) + " - " + QString::number(sizeMax));
	atlasFrameWidthSpinBox->setEnabled(false);
	atlasFrameHeightLabel = new QLabel(atlasFrameGroupBox);
    atlasFrameHeightLabel->setGeometry(QRect(10, 80, 41+30, 21));
	atlasFrameHeightLabel->setText("Height:");

    atlasFrameHeightSpinBox = new QSpinBox(atlasFrameGroupBox);
    atlasFrameHeightSpinBox->setGeometry(QRect(60-5-5, 80, 71, 22));
	atlasFrameHeightSpinBox->setEnabled(false);
	atlasFrameHeightSpinBox->setRange(sizeMin, sizeMax);
	atlasFrameHeightSpinBox->setToolTip(QString::number(sizeMin) + " - " + QString::number(sizeMax));
    texturedAnimationPositionOffsetLine = new QFrame(atlasFrameGroupBox);
    texturedAnimationPositionOffsetLine->setGeometry(QRect(10, 110+10-30-5+20-5+30-20-5-30+30, 120+150+50+20-2, 20));
    texturedAnimationPositionOffsetLine->setFrameShape(QFrame::HLine);
    texturedAnimationPositionOffsetLine->setFrameShadow(QFrame::Sunken);
	atlasPositionOffsetLabel = new QLabel(atlasFrameGroupBox);
    atlasPositionOffsetLabel->setGeometry(QRect(10, 110+10-30-5+50-5-30+30, 21+90, 21));
    atlasPositionOffsetLabel->setText("Position offset X/Y/Z:"); 

	atlasXPositionOffsetSpinBox = new FloatSpinBox(atlasFrameGroupBox);
    atlasXPositionOffsetSpinBox->setGeometry(QRect(20+5+95, 110+10-30-5+50-5, 71+10-10, 22));
	atlasXPositionOffsetSpinBox->setDecimals(4);
	atlasXPositionOffsetSpinBox->setRange(Numerical<float>::_LowerValue(), Numerical<float>::_UpperValue());
	atlasXPositionOffsetSpinBox->setToolTip(QString::number(Numerical<float>::_LowerValue()) + " - " + QString::number(Numerical<float>::_UpperValue()));
	atlasXPositionOffsetSpinBox->setEnabled(false);
	connect(atlasXPositionOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AtlasXPositionOffsetIsChanged(double)));


	atlasYPositionOffsetSpinBox = new FloatSpinBox(atlasFrameGroupBox);
    atlasYPositionOffsetSpinBox->setGeometry(QRect(120+10+10+70-10-3, 110+10-30-5+50-5-30+30, 71+10-10, 22));
	atlasYPositionOffsetSpinBox->setDecimals(4);
	atlasYPositionOffsetSpinBox->setRange(Numerical<float>::_LowerValue(), Numerical<float>::_UpperValue());
	atlasYPositionOffsetSpinBox->setToolTip(QString::number(Numerical<float>::_LowerValue()) + " - " + QString::number(Numerical<float>::_UpperValue()));
	atlasYPositionOffsetSpinBox->setEnabled(false);
	connect(atlasYPositionOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AtlasYPositionOffsetIsChanged(double)));


	atlasZPositionOffsetSpinBox = new FloatSpinBox(atlasFrameGroupBox);
    atlasZPositionOffsetSpinBox->setGeometry(QRect(220+15+10+10+45-10-10-3-3, 110+10-30-5+50-5-30+30, 71+10-10, 22));
	atlasZPositionOffsetSpinBox->setDecimals(4);
	atlasZPositionOffsetSpinBox->setRange(Numerical<float>::_LowerValue(), Numerical<float>::_UpperValue());
	atlasZPositionOffsetSpinBox->setToolTip(QString::number(Numerical<float>::_LowerValue()) + " - " + QString::number(Numerical<float>::_UpperValue()));
	atlasZPositionOffsetSpinBox->setEnabled(false);
	connect(atlasZPositionOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AtlasZPositionOffsetIsChanged(double)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	setWindowTitle("Atlas Creator");
}


int32 TextureCreator::GetVectorLerpMode(int32 _mode)
{
	if(_mode == VectorLerp::BAZIER)
	{
		return 0;
	}
	else if(_mode == VectorLerp::LAGRANGE)
	{
		return 1;
	}
	else if(_mode == VectorLerp::RING)
	{
		return 2;
	}
	return 0;
}



void TextureCreator::SaveVectorLerpAs(void)
{
	if(engine->GetVectorLerp()->GetControlVectors().IsNotEmpty())
	{
		QString name = vectorLerpNameLineEdit->text();

		QString pathFile = QFileDialog::getSaveFileName(this,
														"Save vector lerp as...",
														(AssetLibrary::_GetDir() + VectorLerp::_GetDir()).c_str() + name,
														QString("*") + VectorLerp::_GetFileSuffix().c_str());

		if(!pathFile.isEmpty())
		{	
			QString name = File::_GetBaseName(pathFile.toStdString()).c_str();
			if(File::_IsExist(AssetLibrary::_GetDir() + VectorLerp::_GetDir() + name.toStdString() + "." + VectorLerp::_GetFileSuffix()))
			{ 
				WarningDialog warning("Warning", "Vector lerp '" + name + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					return;
				}
			}

			if(AssetLibrary::_IsAssetExist(engine->GetVectorLerp()->GetFloatLerp()->GetName(), AssetLibrary::FLOAT_LERP_ASSET))
			{
				AssetLibrary::_GetAssets().AddObject(engine->GetVectorLerp()->GetFloatLerp());
			}
			engine->GetVectorLerp()->SaveAsToFile(name.toStdString());
		}
	}
	else
	{
		ErrorDialog("Error", "Vector lerp is not defined", 170, 40);
	}
}


void TextureCreator::SaveHitboxAs(void)
{ 
	if(shapeList->topLevelItemCount() && engine->GetShapes().IsNotEmpty())
	{
		QString hitboxName = hitboxNameLineEdit->text();

		QString pathFile = QFileDialog::getSaveFileName(this,
														"Save hitbox as...",
														(AssetLibrary::_GetDir() + Hitbox::_GetDir()).c_str() + hitboxName,
														QString("*") + Hitbox::_GetFileSuffix().c_str());

		if(!pathFile.isEmpty())
		{	
			QString name = File::_GetBaseName(pathFile.toStdString()).c_str();
			if(File::_IsExist(AssetLibrary::_GetDir() + Hitbox::_GetDir() + name.toStdString() + "." + Hitbox::_GetFileSuffix()))
			{ 
				WarningDialog warning("Warning", "Hitbox '" + name + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					return;
				}
			} 

			if(engine->GetShapes().IsNotEmpty())
			{
				if(editorMode == TEXTURE_EDITOR)
				{
					Hitbox* hitbox = new Hitbox(); 
					hitbox->Rename(name.toStdString());
					for(int i = 0; i < engine->GetShapes().GetSize(); i++)
					{
						TextureShapeListWidget::ItemShape* it = dynamic_cast<TextureShapeListWidget::ItemShape*>(shapeList->topLevelItem(i));

						if(it)
						{ 
							StringANSI shapeName = it->text(TextureShapeListWidget::ItemShape::NAME_PROPERTY).toStdString();
							int32 collisionGroupIndex = it->text(TextureShapeListWidget::ItemShape::COLLISION_GROUP_PROPERTY).toInt();
							bool collisionEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_ENABLE_PROPERTY) == "true" ? true : false;
							bool collisionPolygonsInfoEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_POLYGONS_INFO_PROPERTY) == "true" ? true : false;
							Mesh* mesh = engine->GetShapes().Get(i);
							mesh->SetBoundingVolumeType(engine->GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString()));
							
							if(mesh)
							{
								if(AssetLibrary::_IsAssetExist(mesh->GetName(), AssetLibrary::MESH_ASSET))
								{
									AssetLibrary::_GetAssets().AddObject(mesh);
								}
								hitbox->AddShape(mesh, shapeName, collisionGroupIndex, collisionEnable, collisionPolygonsInfoEnable);
							} 
						} 
					}

					if(AssetLibrary::_IsAssetExist(hitbox->GetName(), AssetLibrary::HITBOX_ASSET))
					{
						AssetLibrary::_GetAssets().AddObject(hitbox);
					}
								
					hitbox->SaveToFile();
					delete hitbox;
				}
				else if(editorMode == TEXTURED_ANIMATION_EDITOR)
				{
					if(atlas->IsExist())
					{
						Hitbox* hitbox = new Hitbox(); 
						hitbox->Rename(name.toStdString());
						for(int i = 0; i < engine->GetShapes().GetSize(); i++)
						{
							TextureViewer::GraphicsSurfaceEx* object = engine->GetObjects().Get(i);

							if(object && object->GetFrameIndex() == GetAnimationCurrentFrame())
							{
								TextureShapeListWidget::ItemShape* it = dynamic_cast<TextureShapeListWidget::ItemShape*>(shapeList->topLevelItem(i));

								if(it)
								{
									StringANSI shapeName = it->text(TextureShapeListWidget::ItemShape::NAME_PROPERTY).toStdString();
									int32 collisionGroupIndex = it->text(TextureShapeListWidget::ItemShape::COLLISION_GROUP_PROPERTY).toInt();
									bool collisionEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_ENABLE_PROPERTY) == "true" ? true : false;
									bool collisionPolygonsInfoEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_POLYGONS_INFO_PROPERTY) == "true" ? true : false;
									Mesh* mesh = engine->GetShapes().Get(i);
									mesh->SetBoundingVolumeType(engine->GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString()));
									
									if(mesh)
									{
										if(AssetLibrary::_IsAssetExist(mesh->GetName(), AssetLibrary::MESH_ASSET))
										{
											AssetLibrary::_GetAssets().AddObject(mesh);
										}
										hitbox->AddShape(mesh, shapeName, collisionGroupIndex, collisionEnable, collisionPolygonsInfoEnable);
									} 
								} 
							}
	
							if(hitbox->IsExist())
							{
								if(AssetLibrary::_IsAssetExist(hitbox->GetName(), AssetLibrary::HITBOX_ASSET))
								{
									AssetLibrary::_GetAssets().AddObject(hitbox);
								}
									
								hitbox->SaveToFile();
							}
							
						}
						delete hitbox;
					}
				}
			}
		}
	}
	else
	{
		ErrorDialog("Error", "Hitbox is not defined", 170, 40);
	}
}



void TextureCreator::SelectShape(int32 _index)
{
	shapeList->SelectShape(_index);
}


void TextureCreator::ImportShape(void)
{  
    importShapeDialog = new QFileDialog(this, "Choose shapes");
	importShapeDialog->setAcceptMode(QFileDialog::AcceptOpen);
	QStringList filtersList;
	filtersList.push_back(QString("*.") + Mesh::_GetFileSuffix().c_str());
	filtersList.push_back(QString("*.") + AbstractObject::_GetFileSuffix().c_str());

	importShapeDialog->setNameFilters(filtersList);
	importShapeDialog->setDirectory((AssetLibrary::_GetDir() + Mesh::_GetDir()).c_str());
	connect(importShapeDialog, SIGNAL(filterSelected(const QString&)), this, SLOT(ImportShapeDialogFileFilterIsChanged(const QString&)));

	if(importShapeDialog->exec())
	{ 
		QStringList files = importShapeDialog->selectedFiles();
		QString name = File::_GetBaseName(files.at(0).toStdString()).c_str();

		if(importShapeDialog->selectedFilter() == QString("*.") + Mesh::_GetFileSuffix().c_str())
		{
			engine->CreateMesh(Mesh::_LoadFromFile(name.toStdString()));
		} 
		else if(importShapeDialog->selectedFilter() == QString("*.") + AbstractObject::_GetFileSuffix().c_str())
		{
			engine->LoadShape(name.toStdString());
		}
	}

	delete importShapeDialog;
	importShapeDialog = NIL;
}



void TextureCreator::SaveShapeDialogFileFilterIsChanged(const QString& _filter)
{
	if(saveShapeDialog)
	{
		if(_filter.toStdString() == (StringANSI("*.") + Mesh::_GetFileSuffix()))
		{
			saveShapeDialog->setDirectory((AssetLibrary::_GetDir() + Mesh::_GetDir()).c_str());
		}
		else if(_filter.toStdString() == (StringANSI("*.") + AbstractObject::_GetFileSuffix()))
		{
			saveShapeDialog->setDirectory((AssetLibrary::_GetDir() + AbstractObject::_GetDir()).c_str());
		}
	}
}



void TextureCreator::ImportShapeDialogFileFilterIsChanged(const QString& _filter)
{
	if(importShapeDialog)
	{
		if(_filter.toStdString() == (StringANSI("*.") + Mesh::_GetFileSuffix()))
		{
			importShapeDialog->setDirectory((AssetLibrary::_GetDir() + Mesh::_GetDir()).c_str());
		}
		else if(_filter.toStdString() == (StringANSI("*.") + AbstractObject::_GetFileSuffix()))
		{
			importShapeDialog->setDirectory((AssetLibrary::_GetDir() + AbstractObject::_GetDir()).c_str());
		}
	}
}


void TextureCreator::SaveShapeAs(void)
{
	if(shapeList->GetSelectedShape())
	{
		QString name = shapeList->GetSelectedShapeName();

		saveShapeDialog = new QFileDialog(this, "Save shape as...");
		saveShapeDialog->setAcceptMode(QFileDialog::AcceptSave);
		QStringList filtersList;
		filtersList.push_back(QString("*.") + Mesh::_GetFileSuffix().c_str());
		filtersList.push_back(QString("*.") + AbstractObject::_GetFileSuffix().c_str());

		saveShapeDialog->setNameFilters(filtersList);
		saveShapeDialog->setDirectory((AssetLibrary::_GetDir() + Mesh::_GetDir()).c_str());
		saveShapeDialog->selectFile(name);
		connect(saveShapeDialog, SIGNAL(filterSelected(const QString&)), this, SLOT(SaveShapeDialogFileFilterIsChanged(const QString&)));

		if(saveShapeDialog->exec())
		{ 
			QStringList files = saveShapeDialog->selectedFiles();
			QString name = File::_GetBaseName(files.at(0).toStdString()).c_str();

			if(saveShapeDialog->selectedFilter() == QString("*.") + Mesh::_GetFileSuffix().c_str())
			{
				if(File::_IsExist(AssetLibrary::_GetDir() + Mesh::_GetDir() + name.toStdString() + "." + Mesh::_GetFileSuffix()))
				{ 
					WarningDialog warning("Warning", "Mesh '" + name + "' is already exist. Rewrite file?", 200, 90);
					
					if(warning.exec() == QDialog::Rejected)
					{
						return;
					}
				}
				engine->HitboxSaveShapeAsMesh(name.toStdString()); 
			}
			else if(saveShapeDialog->selectedFilter() == QString("*.") + AbstractObject::_GetFileSuffix().c_str())
			{
				if(File::_IsExist(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + name.toStdString() + "." + AbstractObject::_GetFileSuffix()))
				{
					WarningDialog warning("Warning", "Object '" + name + "' is already exist. Rewrite file?", 200, 90);
					
					if(warning.exec() == QDialog::Rejected)
					{
						return;
					}
				}
				engine->HitboxSaveShapeAsObject(name.toStdString()); 
			}
		}

		delete saveShapeDialog;
		saveShapeDialog = NIL;
	}
}



QString TextureCreator::GetCurrentBrush(void)const
{
	return brushComboBox->itemText(brushComboBox->currentIndex());
}



void TextureCreator::CreateShape(void)
{
	shapeNumber++;

	QString name = "shape" + QString::number(shapeNumber);
	
	shapeList->AddShape(name);
}



void TextureCreator::CreateShape(QString _name, int _collisionGroupIndex, bool _collisionEnable, bool _collisionPolygonsInfoEnable, int _boundingVolumeType)
{
	shapeList->AddShape(_name, _collisionGroupIndex, _collisionEnable, _collisionPolygonsInfoEnable, _boundingVolumeType);
}



void TextureCreator::HitboxIsChanged(int _enable)
{
	if(editorMode == TEXTURE_EDITOR)
	{
		texture->SetHitboxEnable(_enable);
	}
	else if(editorMode == TEXTURED_ANIMATION_EDITOR)
	{
		texturedAnimation->SetHitboxEnable(_enable);
	}
}


int TextureCreator::GetEditorMode(void)const
{
	return editorMode;
}


void TextureCreator::CreateEngine(void)
{
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.x = viewport->width();
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.y = viewport->height();
	def.rendererDef.realTimeWorkerDef.windowDef.position.x = viewport->x();
	def.rendererDef.realTimeWorkerDef.windowDef.position.y = viewport->y();
	def.rendererDef.realTimeWorkerDef.windowDef.clientWindow = viewport->GetHWND();
	def.rendererDef.realTimeWorkerDef.unsuspended = true; 
	def.rendererDef.zBufferEnable = false;
	
	def.rendererDef.realTimeWorkerDef.windowDef.title = "texture viewer";

	engine = new TextureViewer(this);
	
	if(engine->Create(def))
	{
		intervalTimer.start(1.0f/60.0f);
		
		EnginePlay();
	}
}



void TextureCreator::EnginePlay(void)
{
	connect(&intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}
 


void TextureCreator::CreateViewport(void)
{ 
	viewport = new Viewport(880, 580+30+10, 420+30, 10+20, this);

	connect(viewport, SIGNAL(CursorEnter()), this, SLOT(ViewportCursorEnter())); 
    connect(viewport, SIGNAL(CursorLeave()), this, SLOT(ViewportCursorLeave())); 
}


void TextureCreator::CallFrame(void)
{
	engine->Frame();
}


void TextureCreator::EnginePause(void)
{
	disconnect(&intervalTimer, SIGNAL(timeout()), this, SLOT(CallFrame()));
}


void TextureCreator::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "TextureEditor.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}



void TextureCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}



void TextureCreator::ChangeBackgroundColor(void)
{
	QColor pickedColor = QColorDialog::getColor(QColor(backgroundColor.red, backgroundColor.green, backgroundColor.blue), this, "Color Picker");

	if(pickedColor.isValid())
	{
		backgroundColor.red = pickedColor.red();
		backgroundColor.green = pickedColor.green();
		backgroundColor.blue = pickedColor.blue();
	
		engine->SetBackgroundColor(backgroundColor);
	}
}


float TextureCreator::GetZoom100(void)const
{
	return Numerical<float>::_GetMax(engine->GetSprite()->GetMesh()->GetBoundingQuad().GetSize().x, engine->GetSprite()->GetMesh()->GetBoundingQuad().GetSize().y) + TextureViewer::_GetCamera()->GetFov() * 2.0f;
}


void TextureCreator::SetZoomValue(int _zoom)
{
	currentZoomScaleLabel->setText("Zoom: " + QString::number(_zoom) + "%");
}



void TextureCreator::UpdateCursorPosition(Vector2 _point)
{
	worldCursorPositionLabel->setText("x: " + QString::number((int)_point.x) + "; y: " + QString::number((int)_point.y));
}


void TextureCreator::UpdateAmountOfPoints(int32 _size)
{
	if(_size > 0)
	{
		amountOfPointsLabel->setText("Points: " + QString::number(_size));
		
	}
	else
	{
		amountOfPointsLabel->setText("Points: 0");
	}
}



void TextureCreator::SetHome(void)
{
	if(editorMode == TEXTURED_ANIMATION_EDITOR)
	{
		StopAnimation();
	}
	SetCameraHome(); 
} 


void TextureCreator::SetCameraHome(void)
{
	if((editorMode == TEXTURE_EDITOR && texture->IsExist()) || (editorMode == TEXTURED_ANIMATION_EDITOR && texturedAnimation->IsExist()))
	{
		float maxSize = Numerical<float>::_GetMax(engine->GetSprite()->GetMesh()->GetBoundingQuad().GetSize().x, engine->GetSprite()->GetMesh()->GetBoundingQuad().GetSize().y);
		TextureViewer::_GetCamera()->SetZoomFar(maxSize * engine->GetCameraFarScale());
		TextureViewer::_GetCamera()->SetZoomNear(maxSize * engine->GetCameraNearScale());  
		TextureViewer::_GetCamera()->SetZoomSpeed(maxSize / engine->GetCameraZoomSpeed());
		TextureViewer::_GetCamera()->SetDistanceToViewingPoint(GetZoom100());
	}
	else if(editorMode == TEXTURED_ANIMATION_EDITOR && atlas->IsExist())
	{
		Vector2 size = atlas->GetFrameInfo(0)->GetSizeInPixels() / engine->GetPixelsPerMeter();
		float maxSize = Numerical<float>::_GetMax(size.x, size.y);
		TextureViewer::_GetCamera()->SetZoomFar(maxSize * engine->GetCameraFarScale());
		TextureViewer::_GetCamera()->SetZoomNear(maxSize * engine->GetCameraNearScale());  
		TextureViewer::_GetCamera()->SetZoomSpeed(maxSize / engine->GetCameraZoomSpeed());
		TextureViewer::_GetCamera()->SetDistanceToViewingPoint(GetZoom100());
	}
	else
	{
		TextureViewer::_GetCamera()->SetZoomFar(engine->GetGridSize() * engine->GetPixelsPerMeter() * engine->GetCameraFarScale());
		TextureViewer::_GetCamera()->SetZoomNear(engine->GetGridSize() * engine->GetPixelsPerMeter() * engine->GetCameraNearScale());
		TextureViewer::_GetCamera()->SetZoomSpeed(engine->GetGridSize() * engine->GetPixelsPerMeter() / engine->GetCameraZoomSpeed());
		TextureViewer::_GetCamera()->SetDistanceToViewingPoint(engine->GetGridSize() * engine->GetPixelsPerMeter() + Angle::_RadiansToDegrees(TextureViewer::_GetCamera()->GetFov() * 2));
	}

	TextureViewer::_GetCamera()->SetViewingPoint(Vector3());
	Ratio::_SetKnown<float>(Ratio::WHOLE, TextureViewer::_GetCamera()->GetDistanceToViewingPoint());
	Ratio::_SetKnown<float>(Ratio::PART, GetZoom100());
	SetZoomValue(Ratio::_GetUnknown<float>(Ratio::PART_RATIO));
}



void TextureCreator::NameIsChanged(void)
{
	if(editorMode == TEXTURE_EDITOR)
	{
		texture->Rename(nameLineEdit->text().toStdString());
	}
	else if(editorMode == TEXTURED_ANIMATION_EDITOR)
	{
		texturedAnimation->Rename(nameLineEdit->text().toStdString());
	}
}


void TextureCreator::VectorLerpFloatLerpNameIsChanged(void)
{
	engine->GetVectorLerp()->GetFloatLerp()->Rename(vectorLerpFloatLerpNameLineEdit->text().toStdString());
}


void TextureCreator::VectorLerpNameIsChanged(void)
{
	engine->GetVectorLerp()->Rename(vectorLerpNameLineEdit->text().toStdString());
}


QString TextureCreator::GetObjectName(void)const
{
	return nameLineEdit->text();
}


void TextureCreator::SetNameDisable(bool _disable)
{
	nameLineEdit->setDisabled(_disable);
}


void TextureCreator::SetAtlasNameDisable(bool _disable)
{
	atlasNameLineEdit->setDisabled(_disable);
}


 void TextureCreator::SetVectorLerpNameDisable(bool _disable)
 {
	 vectorLerpNameLineEdit->setDisabled(_disable);
 }



 void TextureCreator::SetFloatLerpNameDisable(bool _disable)
 {
	 vectorLerpFloatLerpNameLineEdit->setDisabled(_disable);
 }


void TextureCreator::SetHitboxNameDisable(bool _disable)
{
	hitboxNameLineEdit->setDisabled(_disable);
}



void TextureCreator::SetObjectName(QString _name)
{
	nameLineEdit->setText(_name);
}


void TextureCreator::SetVectorLerpObjectName(QString _name)
{
	vectorLerpNameLineEdit->setText(_name);
}



void TextureCreator::SetFloatLerpObjectName(QString _name)
{
	vectorLerpFloatLerpNameLineEdit->setText(_name);
}



void TextureCreator::SetHitboxObjectName(QString _name)
{
	hitboxNameLineEdit->setText(_name);
}


void TextureCreator::SetAtlasObjectName(QString _name)
{
	atlasNameLineEdit->setText(_name);
}


Texture2D* TextureCreator::GetTexture(void)
{
	return texture;
}


TexturedAnimation* TextureCreator::GetTexturedAnimation(void)
{
	return texturedAnimation;
}



void TextureCreator::LoadTexture(void)
{
	QString path = QFileDialog::getOpenFileName(this,
			                                    "Choose image",
			                                    startDir,
                                                "*.png");

	if(path.count() == 0)
	{
		return;
	}

	ReloadTexture(path);
}



void TextureCreator::ReloadTexture(QString _path)
{

	engine->DestroyPoints(); 
	DestroyAllShapes(); 

	QFileInfo fileInfo(_path);
	startDir = fileInfo.path();

	engine->GetScene()->GetRenderList().Remove(engine->GetSprite());
	if(texture->Create(Texture2D::QUAD_TEXTURE_MAP, _path.toStdString()))
	{ 
		QString fileName = fileInfo.completeBaseName();
		if(nameLineEdit->isEnabled())
		{
			nameLineEdit->setText(File::_GetName(fileName.toStdString()).c_str());
		}
		textureHeightSpinBox->setValue((int)texture->GetImageSize().y);
		textureWidthSpinBox->setValue((int)texture->GetImageSize().x);
		engine->GetSprite()->BindFrontTexture(texture);
		engine->GetSprite()->GetMesh()->CreateQuad(Quad(Vector3(), texture->GetImageSize() / engine->GetPixelsPerMeter()));
		engine->GetSprite()->SetPosition(Vector3());
		SetHome(); 
		 
		engine->GetScene()->AddToRenderList(engine->GetSprite());
		
		okButton->setEnabled(true);
	}
	else
	{
		okButton->setEnabled(false);
		textureHeightSpinBox->setValue(1);
		textureWidthSpinBox->setValue(1);
	}
}


int TextureCreator::GetAnimationCurrentFrame(void)const
{
	if(editorMode == TEXTURED_ANIMATION_EDITOR)
	{
		if(animationCurrentFrameSpinBox->value() > 0)
		{
			return animationCurrentFrameSpinBox->value() - 1;
		}
		return animationCurrentFrameSpinBox->value();
	}
	return 0;
}



void TextureCreator::SetTexturedAnimation(QString _name)
{

	engine->DestroyPoints();
	DestroyAllShapes();

	engine->GetScene()->GetRenderList().Remove(engine->GetSprite());
	engine->GetScene()->GetUpdateList().Remove(texturedAnimation);
	if(texturedAnimation->LoadFromFile(_name.toStdString()))
	{
		if(nameLineEdit->isEnabled())
		{
			nameLineEdit->setText(texturedAnimation->GetName().c_str());
		}
		if(texturedAnimation->GetAtlas())
		{ 
			delete atlas;
	
			atlas = texturedAnimation->GetAtlas();
			if(atlasNameLineEdit->isEnabled())
			{
				atlasNameLineEdit->setText(texturedAnimation->GetAtlas()->GetName().c_str());
			}
			animationCurrentFrameSpinBox->setRange(1, atlas->GetAmountOfFrames());
			animationCurrentFrameSpinBox->setToolTip(QString::number(1) + " - " + QString::number(atlas->GetAmountOfFrames()));
			animationCurrentFrameSpinBox->setEnabled(true);
			animationMinDuration = 0.1f;
			animationMaxDuration = 60.0f;

			animationMinFrameDuration = animationMinDuration / atlas->GetAmountOfFrames();
			animationMaxFrameDuration = animationMaxDuration / atlas->GetAmountOfFrames();
			
			animationMinFPS = 1.0f / animationMaxFrameDuration; 
			animationMaxFPS = 1.0f / animationMinFrameDuration; 

			animationDurationSpinBox->blockSignals(true);
			animationDurationSpinBox->setSingleStep(animationMaxDuration / (atlas->GetAmountOfFrames() * 2));
			animationDurationSpinBox->setToolTip(QString::number(animationMinDuration) + " - " + QString::number(animationMaxDuration) + " (sec)"); 
			animationDurationSpinBox->setRange(animationMinDuration, animationMaxDuration);
			animationDurationSpinBox->blockSignals(false);
		

			animationFrameDurationSpinBox->blockSignals(true);
			animationFrameDurationSpinBox->setSingleStep(animationMaxFrameDuration / (atlas->GetAmountOfFrames() * 2));
			animationFrameDurationSpinBox->setToolTip(QString::number(animationMinFrameDuration) + " - " + QString::number(animationMaxFrameDuration) + " (sec)"); 
			animationFrameDurationSpinBox->setRange(animationMinFrameDuration, animationMaxFrameDuration);
			animationFrameDurationSpinBox->blockSignals(false);
		

			animationFPSSpinBox->blockSignals(true);
			animationFPSSpinBox->setSingleStep(animationMaxFPS / (atlas->GetAmountOfFrames() * 2));
			animationFPSSpinBox->setRange(animationMinFPS, animationMaxFPS);
			animationFPSSpinBox->setToolTip(QString::number(animationMinFPS) + " - " + QString::number(animationMaxFPS) + " (frames per second)"); 
			animationFPSSpinBox->blockSignals(false);
		

			animationElapsedTimeSpinBox->blockSignals(true);
			animationElapsedTimeSpinBox->setSingleStep(1.0f);
			animationElapsedTimeSpinBox->setToolTip("0.0 - " + QString::number(animationMaxDuration) + " (sec)");
			animationElapsedTimeSpinBox->setRange(0.0f, animationMaxDuration);
			animationElapsedTimeSpinBox->blockSignals(false);
			SetAtlasFiltering(atlas->GetFilteringMode());
			atlasAmountOfAnimFrameLabel->setText("Amount of frames: " + QString::number(atlas->GetAmountOfFrames()) + "/" + QString::number(Atlas2D::_GetMaxFrames()));
		}
		engine->GetSprite()->BindFrontTexturedAnimation(texturedAnimation);
		engine->GetSprite()->GetMesh()->CreateQuad(Quad(Vector3(), texturedAnimation->GetCurrentFrameSize() / engine->GetPixelsPerMeter()));
		engine->GetSprite()->SetPosition(Vector3());
		animationFPSSpinBox->setEnabled(true);
		animationDurationSpinBox->setEnabled(true);
		animationFrameDurationSpinBox->setEnabled(true);
		animationElapsedTimeSpinBox->setEnabled(true);

		atlasFrameNameLineEdit->setEnabled(true);
		atlasXPositionOffsetSpinBox->setEnabled(true);
		atlasYPositionOffsetSpinBox->setEnabled(true);
		atlasZPositionOffsetSpinBox->setEnabled(true);

			
		int32 currentFrameIndex = texturedAnimation->GetCurrentFrame();
		animationElapsedTimeSpinBox->blockSignals(true);
		animationElapsedTimeSpinBox->setValue(texturedAnimation->GetElapsedTimeInSec());
		animationElapsedTimeSpinBox->blockSignals(false);
		texturedAnimationPositionOffsetEnableCheckBox->setChecked(texturedAnimation->IsPositionOffsetEnabled());
		hitboxEnableCheckBox->setChecked(texturedAnimation->IsHitboxEnabled());
		hitboxNames.Clear();
		engine->DestroyPoints();
		DestroyAllShapes();
		for(int32 i = 0; i < atlas->GetAmountOfFrames(); i++)
		{ 
			Hitbox* hitbox = atlas->GetHitbox(i);
			hitboxNames.AddToTail(QString());

			if(hitbox)
			{
				hitboxNames.GetTail()->key = hitbox->GetName().c_str();

				animationCurrentFrameSpinBox->blockSignals(true);
				animationCurrentFrameSpinBox->setValue(i + 1);
				animationCurrentFrameSpinBox->blockSignals(false);

				engine->SetHitbox(hitbox);
				 
				hitboxNameLineEdit->setText(hitbox->GetName().c_str());

				delete hitbox;
			}
		}
		animationCurrentFrameSpinBox->setValue(currentFrameIndex + 1);
		AnimationUpdateCurrentFrame(currentFrameIndex);
		animationForwardRadioButton->blockSignals(true);
		animationBackwardRadioButton->blockSignals(true);
		animationPingPongRadioButton->blockSignals(true);
		animationOnewayRadioButton->blockSignals(true);
		animationLoopRadioButton->blockSignals(true);
		animationSingleShotRadioButton->blockSignals(true);

		animationForwardRadioButton->setChecked(texturedAnimation->GetPlayingMode() & PlayingObject::FORWARD_PLAYING);
		animationBackwardRadioButton->setChecked(texturedAnimation->GetPlayingMode() & PlayingObject::BACKWARD_PLAYING);
		animationPingPongRadioButton->setChecked(texturedAnimation->GetPlayingMode() & PlayingObject::PING_PONG_PLAYING);
		animationOnewayRadioButton->setChecked(texturedAnimation->GetPlayingMode() & PlayingObject::ONEWAY_PLAYING);
		animationLoopRadioButton->setChecked(texturedAnimation->GetPlayingMode() & PlayingObject::LOOP_PLAYING);
		animationSingleShotRadioButton->setChecked(texturedAnimation->GetPlayingMode() & PlayingObject::SINGLE_SHOT_PLAYING);

		animationForwardRadioButton->blockSignals(false);
		animationBackwardRadioButton->blockSignals(false);
		animationPingPongRadioButton->blockSignals(false);
		animationOnewayRadioButton->blockSignals(false);
		animationLoopRadioButton->blockSignals(false);
		animationSingleShotRadioButton->blockSignals(false);
		if(texturedAnimation->GetFPS() < animationMinFPS)
		{ 
			animationFPSSpinBox->setValue(animationMinFPS);
			animationDurationSpinBox->setValue(animationMaxDuration);
			animationFrameDurationSpinBox->setValue(animationMaxFrameDuration);
		}
		else if(texturedAnimation->GetFPS() > animationMaxFPS)
		{ 
			animationFPSSpinBox->setValue(animationMaxFPS);
			animationDurationSpinBox->setValue(animationMinDuration);
			animationFrameDurationSpinBox->setValue(animationMinFrameDuration);
		}
		else
		{
			animationFPSSpinBox->setValue(texturedAnimation->GetFPS());
			animationDurationSpinBox->setValue(texturedAnimation->GetPlayingDurationInSec());
			animationFrameDurationSpinBox->setValue(texturedAnimation->GetPlayingFrameDurationInSec());
		}
		SetCameraHome(); 
		 
		engine->GetScene()->AddToRenderList(engine->GetSprite());
		engine->GetScene()->AddToUpdateList(texturedAnimation);


		okButton->setEnabled(true);
	}
	else
	{
		okButton->setEnabled(false);
	}
}



void TextureCreator::SetTexture(QString _name)
{

	engine->DestroyPoints();
	DestroyAllShapes();

	engine->GetScene()->GetRenderList().Remove(engine->GetSprite());
	if(texture->LoadFromFile(_name.toStdString()))
	{
		if(nameLineEdit->isEnabled())
		{
			nameLineEdit->setText(texture->GetName().c_str());
		}
		textureHeightSpinBox->setValue((int)texture->GetImageSize().y);
		textureWidthSpinBox->setValue((int)texture->GetImageSize().x);
		SetTextureFiltering(texture->GetFilteringMode());
		hitboxEnableCheckBox->setChecked(texture->IsHitboxEnabled());
		engine->GetSprite()->BindFrontTexture(texture);
		engine->GetSprite()->GetMesh()->CreateQuad(Quad(Vector3(), texture->GetImageSize() / engine->GetPixelsPerMeter()));
		engine->GetSprite()->SetPosition(Vector3());
		SetHome();
		
		engine->GetScene()->AddToRenderList(engine->GetSprite());
		if(texture->GetHitbox())
		{
			engine->DestroyPoints();
			DestroyAllShapes();

			engine->SetHitbox(texture->GetHitbox());
			
			hitboxNameLineEdit->setText(texture->GetHitbox()->GetName().c_str());

			delete texture->GetHitbox();
		}

		okButton->setEnabled(true);
	}
	else
	{
		okButton->setEnabled(false);
	}
} 


void TextureCreator::SaveTexture(void)
{
	if(engine->GetShapes().IsNotEmpty())
	{
		Hitbox* hitbox = new Hitbox();
		hitbox->Rename(hitboxNameLineEdit->text().toStdString());
		for(int i = 0; i < engine->GetShapes().GetSize(); i++)
		{
			TextureShapeListWidget::ItemShape* it = dynamic_cast<TextureShapeListWidget::ItemShape*>(shapeList->topLevelItem(i));

			if(it)
			{
				StringANSI shapeName = it->text(TextureShapeListWidget::ItemShape::NAME_PROPERTY).toStdString();
				int32 collisionGroupIndex = it->text(TextureShapeListWidget::ItemShape::COLLISION_GROUP_PROPERTY).toInt();
				bool collisionEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_ENABLE_PROPERTY) == "true" ? true : false;
				bool collisionPolygonsInfoEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_POLYGONS_INFO_PROPERTY) == "true" ? true : false;
				Mesh* mesh = engine->GetShapes().Get(i);
				mesh->SetBoundingVolumeType(engine->GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString()));
				
				if(mesh)
				{
					if(AssetLibrary::_IsAssetExist(mesh->GetName(), AssetLibrary::MESH_ASSET))
					{
						AssetLibrary::_GetAssets().AddObject(mesh);
					}
					hitbox->AddShape(mesh, shapeName, collisionGroupIndex, collisionEnable, collisionPolygonsInfoEnable);
				}
			} 
		}
			
		if(AssetLibrary::_IsAssetExist(hitbox->GetName(), AssetLibrary::HITBOX_ASSET))
		{
			AssetLibrary::_GetAssets().AddObject(hitbox);
		}

		texture->BindHitbox(hitbox);
	}  

	texture->SaveToFile();
}



void TextureCreator::SaveTexturedAnimation(void)
{
	for(int j = 0; j < atlas->GetAmountOfFrames(); j++)
	{
		Hitbox* hitbox = new Hitbox();
		hitbox->Rename(hitboxNames.Get(j)->key.toStdString());
		for(int i = 0; i < engine->GetShapes().GetSize(); i++)
		{
			TextureViewer::GraphicsSurfaceEx* object = engine->GetObjects().Get(i);

			if(object && object->GetFrameIndex() == j)
			{
				TextureShapeListWidget::ItemShape* it = dynamic_cast<TextureShapeListWidget::ItemShape*>(shapeList->topLevelItem(i));

				if(it)
				{
					StringANSI shapeName = it->text(TextureShapeListWidget::ItemShape::NAME_PROPERTY).toStdString();
					int32 collisionGroupIndex = it->text(TextureShapeListWidget::ItemShape::COLLISION_GROUP_PROPERTY).toInt();
					bool collisionEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_ENABLE_PROPERTY) == "true" ? true : false;
					bool collisionPolygonsInfoEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_POLYGONS_INFO_PROPERTY) == "true" ? true : false;
					Mesh* mesh = engine->GetShapes().Get(i);
					mesh->SetBoundingVolumeType(engine->GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString()));
					
					if(mesh)
					{
						if(AssetLibrary::_IsAssetExist(mesh->GetName(), AssetLibrary::MESH_ASSET))
						{
							AssetLibrary::_GetAssets().AddObject(mesh);
						}
						hitbox->AddShape(mesh, shapeName, collisionGroupIndex, collisionEnable, collisionPolygonsInfoEnable);
					} 
				} 
			}
		}

		if(hitbox->GetShapes().IsNotEmpty())
		{
			if(AssetLibrary::_IsAssetExist(hitbox->GetName(), AssetLibrary::HITBOX_ASSET))
			{
				AssetLibrary::_GetAssets().AddObject(hitbox);
			}
			
			atlas->BindHitbox(j, hitbox);
		}
		else
		{
			delete hitbox;
		}
	}

	if(AssetLibrary::_IsAssetExist(atlas->GetName(), AssetLibrary::ATLAS_2D_ASSET))
	{
		AssetLibrary::_GetAssets().AddObject(atlas);
	}


	texturedAnimation->SaveToFile();
}



void TextureCreator::closeEvent(QCloseEvent *_event)
{
	EnginePause();
}



void TextureCreator::TextureIsOk(void)
{
	int32 errorCode;
	if(nameLineEdit->text().isEmpty())
	{
		ErrorDialog("Error", "Texture name is not defined", 155, 40);
		return;
	}
	if((errorCode = BooleanExprParser::_IsNameValid(nameLineEdit->text().toStdString())) >= 0)
	{
		ErrorDialog("Error", "Texture name is not valid: '" + QString(nameLineEdit->text().at(errorCode)) + "'", 190, 40);
		return;
	}

	if(textureWidthSpinBox->value() > sizeMax)
	{ 
		ErrorDialog("Error", "Image width is overflow " + QString::number(sizeMax).toAscii(), 150, 40);
		return;
	}

	if(textureHeightSpinBox->value() > sizeMax)
	{
		ErrorDialog("Error", "Image height is overflow " + QString::number(sizeMax).toAscii(), 150, 40);
		return;
	}

	if(!fastCreatingEnable)
	{
		int assetType;
		if(File::_IsExist(AssetLibrary::_GetDir() + Texture2D::_GetDir() + StringANSI(nameLineEdit->text().toStdString()) + "." + Texture2D::_GetFileSuffix()))
		{ 
			WarningDialog warning("Warning", "Texture '" + nameLineEdit->text() + "' is already exist. Rewrite file?", 200, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				return;
			}
		}
		else if((assetType = AssetLibrary::_IsAssetExist(texture->GetName())) != AssetLibrary::UNKNOWN_ASSET)
		{ 
			WarningDialog warning("Warning", "Name '" + nameLineEdit->text() + "' is already reserved (" + nameLineEdit->text() + "." + QString(AssetLibrary::_GetFileSuffixOfAssetType(assetType).c_str()) + "). Ignore?", 280, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				return;
			}
		} 
		
		SaveTexture();

		EnginePause();
		delete texture;
		texture = NIL;
	}
	else
	{ 
		if(AssetLibrary::_GetAssets().AddObject(texture) == NIL)
		{
			ErrorDialog("Error", "Name '" + nameLineEdit->text() + "' is already reserved", 195, 40);
			return;
		}  
	}
 
	accept();
}



void TextureCreator::TexturedAnimationIsOk(void)
{
	int32 errorCode;
	if(nameLineEdit->text().isEmpty())
	{
		ErrorDialog("Error", "Textured animation name is not defined", 155, 40);
		return;
	}
	if((errorCode = BooleanExprParser::_IsNameValid(nameLineEdit->text().toStdString())) >= 0)
	{
		ErrorDialog("Error", "Textured animation name is not valid: '" + QString(nameLineEdit->text().at(errorCode)) + "'", 190, 40);
		return;
	}


	if(!fastCreatingEnable)
	{
		int assetType;
		if(File::_IsExist(AssetLibrary::_GetDir() + TexturedAnimation::_GetDir() + StringANSI(nameLineEdit->text().toStdString()) + "." + TexturedAnimation::_GetFileSuffix()))
		{ 
			WarningDialog warning("Warning", "Textured animation '" + nameLineEdit->text() + "' is already exist. Rewrite file?", 200, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				return;
			}
		} 
		else if((assetType = AssetLibrary::_IsAssetExist(texturedAnimation->GetName())) != AssetLibrary::UNKNOWN_ASSET)
		{
			WarningDialog warning("Warning", "Name '" + nameLineEdit->text() + "' is already reserved (" + nameLineEdit->text() + "." + QString(AssetLibrary::_GetFileSuffixOfAssetType(assetType).c_str()) + "). Ignore?", 280, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				return;
			}
		} 
		
		SaveTexturedAnimation();

		EnginePause();
		delete texturedAnimation;
		texturedAnimation = NIL;
	}
	else
	{ 
		if(AssetLibrary::_GetAssets().AddObject(texturedAnimation) == NIL)
		{
			ErrorDialog("Error", "Name '" + nameLineEdit->text() + "' is already reserved", 195, 40);
			return;
		} 
	}

	accept();
}



