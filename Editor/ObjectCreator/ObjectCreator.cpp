
#include "ObjectCreator.h"
#include <Engine/Output/File/File.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Editor/WarningDialog/WarningDialog.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h>
#include <Engine/Core/Parser/List/FloatList/FloatList.h>
#include <Engine/Core/Parser/List/BooleanList/BooleanList.h>
#include <Engine/Core/Parser/List/StringList/StringList.h>
#include <Engine/Core/Parser/List/Vector2List/Vector2List.h>
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h>
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h>
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Editor/FloatLerpCreator/FloatLerpCreator.h>
#include <Editor/QuaternionLerpCreator/QuaternionLerpCreator.h>
#include <Editor/PlayerControlCreator/PlayerControlCreator.h>
#include <Editor/TextureCreator/TextureCreator.h>
#include <Editor/TriggerCreator/VariableCreator/VariableCreator.h>
#include <Editor/TriggerCreator/ActionCreator/ActionCreator.h>
#include <Editor/TriggerCreator/EventCreator/EventCreator.h>
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Parser/List/ColorableObjectList/ColorableObjectList.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsStrip/GraphicsStrip.h>
#include <Engine/Core/Time/Timer/Timer.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>
#include <Engine/Core/Parser/Set/Set.h>
#include <Engine/Core/Parser/List/SetList/SetList.h>



ObjectCreator::ObjectCreator(QWidget* parent): QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), fastCreatingEnable(false)
{
	propertiesGroupBox = new QGroupBox(this);
    propertiesGroupBox->setGeometry(QRect(30, 100-60, 311+250+250-77+15+30+30+10+10+10+20+20+60+200, 311+41+60+60+70+40+10));
    propertiesGroupBox->setTitle("Properties:");
	logicsGroupBox = new QGroupBox(propertiesGroupBox);
    logicsGroupBox->setGeometry(QRect(10, 20+10, 141+10+30, 251+41+60+60+70+40+40-250));
    logicsGroupBox->setTitle("Logics:");
	logicsScrollArea = new QScrollArea(logicsGroupBox);
    logicsScrollArea->setGeometry(QRect(5, 15, 141+10+30-10, 251+41+60+60+70+40+40-10-10-250));
    logicsScrollArea->setWidgetResizable(true);
	logicsScrollAreaWidget = new QWidget(logicsScrollArea);
    logicsScrollAreaWidget->setGeometry(QRect(0, 0, 141+10+30-10, 251+41+60+60+70+40+40-10-10-250));
	logicsScrollArea->setWidget(logicsScrollAreaWidget);
    logicsVLayout = new QVBoxLayout(logicsScrollAreaWidget);
	logicsVLayout->setSpacing(5);
	
	logicsScrollAreaWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	logicsScrollAreaWidget->setLayout(logicsVLayout);
	graphicsGroupBox = new QGroupBox(propertiesGroupBox);
    graphicsGroupBox->setGeometry(QRect(160+10+30, 20+10, 141+10+30, 251+41+60+60+70+40+40-250-100));
	graphicsGroupBox->setTitle("Graphics:");
	graphicsScrollArea = new QScrollArea(graphicsGroupBox);
    graphicsScrollArea->setGeometry(QRect(5, 15, 141+10+30-10, 251+41+60+60+70+40+40-250-10-10-100));
    graphicsScrollArea->setWidgetResizable(true);
	graphicsScrollAreaWidget = new QWidget();
    graphicsScrollAreaWidget->setGeometry(QRect(0, 0, 141+10+30-10, 251+41+60+60+70+40+40-10-10-100));
	graphicsScrollArea->setWidget(graphicsScrollAreaWidget);
    graphicsVLayout = new QVBoxLayout(graphicsScrollAreaWidget);
	graphicsVLayout->setSpacing(5);
	
	graphicsScrollAreaWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	graphicsScrollAreaWidget->setLayout(graphicsVLayout);
	assetsGroupBox = new QGroupBox(propertiesGroupBox);
    assetsGroupBox->setGeometry(QRect(160+10+30+160+30, 20+10, 141+10+30, 251+41+60+60+70+40+40));
    assetsGroupBox->setTitle("Assets:");
	assetsScrollArea = new QScrollArea(assetsGroupBox);
    assetsScrollArea->setGeometry(QRect(5, 15, 141+10+30-10, 251+41+60+60+70+40+40-10-10));
    assetsScrollArea->setWidgetResizable(true);
	assetsScrollAreaWidget = new QWidget();
    assetsScrollAreaWidget->setGeometry(QRect(0, 0, 141+10+30-10, 251+41+60+60+70+40+40-10-10));
	assetsScrollArea->setWidget(assetsScrollAreaWidget);
    assetsVLayout = new QVBoxLayout(assetsScrollAreaWidget);
	assetsVLayout->setSpacing(5);
	
	assetsScrollAreaWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	assetsScrollAreaWidget->setLayout(assetsVLayout);
	supportedEventsListLabel = new QLabel(propertiesGroupBox); 
	supportedEventsListLabel->setGeometry(QRect(160+141+10+40+10+10+15+10+10+20+60+200, 20-10, 31+80, 21));
	supportedEventsListLabel->setText("supported events:");
	supportedEventsList = new TriggerListWidget(propertiesGroupBox);
	supportedEventsList->setGeometry(QRect(160+141+10+10+10+60+200, 20+20-10, 200+15+30+10+20, 251+41+60+60-20+70+40+40+10));
	
	{
		QList<QPixmap> frames;
		frames.push_back(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ObjectCreator/event.png"));
		frames.push_back(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ObjectCreator/event_warning.png"));
		supportedEventsList->SetFrames(frames);
	}
	supportedActionsListLabel = new QLabel(propertiesGroupBox); 
	supportedActionsListLabel->setGeometry(QRect(160+141+10+40+200+10+10+15+30+10+15+10+20+20+60+200, 20-10, 31+80, 21));
	supportedActionsListLabel->setText("supported actions:");
	supportedActionsList = new TriggerListWidget(propertiesGroupBox);
	supportedActionsList->setGeometry(QRect(160+141+10+200+10+30+10+10+20+60+200, 20+20-10, 200+15+30+10+20, 251+41+60+60-20+70+40+40+10));
	
	{
		QList<QPixmap> frames;
		frames.push_back(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ObjectCreator/action.png"));
		frames.push_back(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "ObjectCreator/action_warning.png"));
		supportedActionsList->SetFrames(frames);
	}
	timerCheckBox = new QCheckBox(assetsGroupBox);
    timerCheckBox->setText("Timer");
	connect(timerCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TimerChecked(int))); 
	assetsVLayout->addWidget(timerCheckBox);
	abstractListCheckBox = new QCheckBox(assetsGroupBox);
    abstractListCheckBox->setText("AbstractList");
	connect(abstractListCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AbstractListChecked(int))); 
	assetsVLayout->addWidget(abstractListCheckBox);
	vectorLerpCheckBox = new QCheckBox(assetsGroupBox);
	vectorLerpCheckBox->setText("VectorLerp");
	connect(vectorLerpCheckBox, SIGNAL(stateChanged(int)), this, SLOT(VectorLerpChecked(int)));  
	assetsVLayout->addWidget(vectorLerpCheckBox);
	floatLerpCheckBox = new QCheckBox(assetsGroupBox);
	floatLerpCheckBox->setText("FloatLerp");
	connect(floatLerpCheckBox, SIGNAL(stateChanged(int)), this, SLOT(FloatLerpChecked(int))); 
	assetsVLayout->addWidget(floatLerpCheckBox);
	quaternionLerpCheckBox = new QCheckBox(assetsGroupBox);
	quaternionLerpCheckBox->setText("QuaternionLerp");
	connect(quaternionLerpCheckBox, SIGNAL(stateChanged(int)), this, SLOT(QuaternionLerpChecked(int)));  
	assetsVLayout->addWidget(quaternionLerpCheckBox);
	objectSceneCheckBox = new QCheckBox(assetsScrollAreaWidget);
	objectSceneCheckBox->setText("Layer");
	connect(objectSceneCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ObjectSceneChecked(int))); 
	assetsVLayout->addWidget(objectSceneCheckBox);
	logicSceneCheckBox = new QCheckBox(assetsGroupBox);
	logicSceneCheckBox->setText("LogicScene");
	connect(logicSceneCheckBox, SIGNAL(stateChanged(int)), this, SLOT(LogicSceneChecked(int))); 
	assetsVLayout->addWidget(logicSceneCheckBox);
    graphicsSceneCheckBox = new QCheckBox(assetsGroupBox);
	graphicsSceneCheckBox->setText("GraphicsScene");
	connect(graphicsSceneCheckBox, SIGNAL(stateChanged(int)), this, SLOT(GraphicsSceneChecked(int))); 
	assetsVLayout->addWidget(graphicsSceneCheckBox);
    tiledSpace3DCheckBox = new QCheckBox(assetsGroupBox);
	tiledSpace3DCheckBox->setText("TiledSpace");
	connect(tiledSpace3DCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TiledSpace3DChecked(int))); 
	assetsVLayout->addWidget(tiledSpace3DCheckBox);
	sceneCheckBox = new QCheckBox(assetsGroupBox);
    sceneCheckBox->setText("Scene");
	connect(sceneCheckBox, SIGNAL(stateChanged(int)), this, SLOT(SceneChecked(int)));
	assetsVLayout->addWidget(sceneCheckBox);
	abstractAnimationCheckBox = new QCheckBox(assetsGroupBox);
	abstractAnimationCheckBox->setText("AbstractAnimation");
	abstractAnimationCheckBox->setEnabled(false);
	connect(abstractAnimationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AbstractAnimationChecked(int)));  
	assetsVLayout->addWidget(abstractAnimationCheckBox);
	texturedAnimationCheckBox = new QCheckBox(assetsGroupBox);
    texturedAnimationCheckBox->setText("TexturedAnimation");
	connect(texturedAnimationCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TexturedAnimationChecked(int))); 
	assetsVLayout->addWidget(texturedAnimationCheckBox);
	playerControlCheckBox = new QCheckBox(assetsGroupBox);
	playerControlCheckBox->setText("PlayerControl");
	connect(playerControlCheckBox, SIGNAL(stateChanged(int)), this, SLOT(PlayerControlChecked(int))); 
	assetsVLayout->addWidget(playerControlCheckBox);
	triggerCheckBox = new QCheckBox(assetsGroupBox);
	triggerCheckBox->setText("Trigger"); 
	connect(triggerCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TriggerChecked(int)));  
	assetsVLayout->addWidget(triggerCheckBox);
	eventCheckBox = new QCheckBox(assetsGroupBox);
	eventCheckBox->setText("Event");
	connect(eventCheckBox, SIGNAL(stateChanged(int)), this, SLOT(EventChecked(int)));  
	assetsVLayout->addWidget(eventCheckBox);
	actionCheckBox = new QCheckBox(assetsGroupBox);
	actionCheckBox->setText("Action");
	connect(actionCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ActionChecked(int)));  
	assetsVLayout->addWidget(actionCheckBox);
	meshCheckBox = new QCheckBox(assetsGroupBox);
	meshCheckBox->setText("Mesh");
	connect(meshCheckBox, SIGNAL(stateChanged(int)), this, SLOT(MeshChecked(int)));  
	assetsVLayout->addWidget(meshCheckBox);
	hitboxCheckBox = new QCheckBox(assetsGroupBox);
	hitboxCheckBox->setText("Hitbox");
	connect(hitboxCheckBox, SIGNAL(stateChanged(int)), this, SLOT(HitboxChecked(int)));  
	assetsVLayout->addWidget(hitboxCheckBox);
	atlasCheckBox = new QCheckBox(assetsGroupBox);
    atlasCheckBox->setText("Atlas");
	connect(atlasCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AtlasChecked(int))); 
	assetsVLayout->addWidget(atlasCheckBox);
	textureCheckBox = new QCheckBox(assetsGroupBox);
    textureCheckBox->setText("Texture");
	connect(textureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TextureChecked(int))); 
	assetsVLayout->addWidget(textureCheckBox);
	cameraCheckBox = new QCheckBox(assetsGroupBox);
	cameraCheckBox->setText("Camera");
	connect(cameraCheckBox, SIGNAL(stateChanged(int)), this, SLOT(CameraChecked(int)));  
	assetsVLayout->addWidget(cameraCheckBox);
	abstractVariableCheckBox = new QCheckBox(assetsGroupBox);
	abstractVariableCheckBox->setText("AbstractVariable");
	abstractVariableCheckBox->setEnabled(false);
	connect(abstractVariableCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AbstractVariableChecked(int)));  
	assetsVLayout->addWidget(abstractVariableCheckBox);
	variableIntCheckBox = new QCheckBox(assetsGroupBox);
	variableIntCheckBox->setText("Variable<Integer>");
	connect(variableIntCheckBox, SIGNAL(stateChanged(int)), this, SLOT(VariableIntChecked(int))); 
	assetsVLayout->addWidget(variableIntCheckBox);
	variableFloatCheckBox = new QCheckBox(assetsGroupBox);
	variableFloatCheckBox->setText("Variable<Float>");
	connect(variableFloatCheckBox, SIGNAL(stateChanged(int)), this, SLOT(VariableFloatChecked(int))); 
	assetsVLayout->addWidget(variableFloatCheckBox);
	variableBoolCheckBox = new QCheckBox(assetsGroupBox);
	variableBoolCheckBox->setText("Variable<Boolean>");
	connect(variableBoolCheckBox, SIGNAL(stateChanged(int)), this, SLOT(VariableBoolChecked(int))); 
	assetsVLayout->addWidget(variableBoolCheckBox);
	variableStringCheckBox = new QCheckBox(assetsGroupBox);
	variableStringCheckBox->setText("Variable<String>");
	connect(variableStringCheckBox, SIGNAL(stateChanged(int)), this, SLOT(VariableStringChecked(int))); 
	assetsVLayout->addWidget(variableStringCheckBox);
	variableVector2CheckBox = new QCheckBox(assetsGroupBox);
	variableVector2CheckBox->setText("Variable<Vector2>");
	connect(variableVector2CheckBox, SIGNAL(stateChanged(int)), this, SLOT(VariableVector2Checked(int))); 
	assetsVLayout->addWidget(variableVector2CheckBox);
	variableVector3CheckBox = new QCheckBox(assetsGroupBox);
	variableVector3CheckBox->setText("Variable<Vector3>");
	connect(variableVector3CheckBox, SIGNAL(stateChanged(int)), this, SLOT(VariableVector3Checked(int))); 
	assetsVLayout->addWidget(variableVector3CheckBox);
	variableQuaternionCheckBox = new QCheckBox(assetsGroupBox);
	variableQuaternionCheckBox->setText("Variable<Quaternion>");
	connect(variableQuaternionCheckBox, SIGNAL(stateChanged(int)), this, SLOT(VariableQuaternionChecked(int)));  
	assetsVLayout->addWidget(variableQuaternionCheckBox);
	abstractObjectCheckBox = new QCheckBox(logicsGroupBox);
	abstractObjectCheckBox->setText("AbstractObject");
	connect(abstractObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AbstractObjectChecked(int)));
	logicsVLayout->addWidget(abstractObjectCheckBox);
	planeCheckBox = new QCheckBox(logicsGroupBox);
    planeCheckBox->setText("Plane"); 
	connect(planeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(PlaneChecked(int))); 
	logicsVLayout->addWidget(planeCheckBox);
	abstractSurfaceCheckBox = new QCheckBox(logicsGroupBox);
	abstractSurfaceCheckBox->setText("AbstractSurface");
	abstractSurfaceCheckBox->setEnabled(false);
	connect(abstractSurfaceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(AbstractSurfaceChecked(int))); 
	logicsVLayout->addWidget(abstractSurfaceCheckBox);
	polygonalSurfaceCheckBox = new QCheckBox(logicsGroupBox);
    polygonalSurfaceCheckBox->setText("PolygonalSurface");
	polygonalSurfaceCheckBox->setEnabled(false);
	connect(polygonalSurfaceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(PolygonalSurfaceChecked(int))); 
	logicsVLayout->addWidget(polygonalSurfaceCheckBox);
	setCheckBox = new QCheckBox(logicsGroupBox);
    setCheckBox->setText("Set"); 
	connect(setCheckBox, SIGNAL(stateChanged(int)), this, SLOT(SetChecked(int))); 
	logicsVLayout->addWidget(setCheckBox);
	logicObjectCheckBox = new QCheckBox(logicsGroupBox);
	logicObjectCheckBox->setText("LogicObject");
	logicObjectCheckBox->setEnabled(false);
	connect(logicObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(LogicObjectChecked(int))); 
	logicsVLayout->addWidget(logicObjectCheckBox);
	playingObjectCheckBox = new QCheckBox(logicsGroupBox);
	playingObjectCheckBox->setText("PlayingObject");
	playingObjectCheckBox->setEnabled(false);
	connect(playingObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(PlayingObjectChecked(int))); 
	logicsVLayout->addWidget(playingObjectCheckBox);
	translatableObjectCheckBox = new QCheckBox(logicsGroupBox);
	translatableObjectCheckBox->setText("TranslatableObject");
	translatableObjectCheckBox->setEnabled(false); 
	connect(translatableObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TranslatableObjectChecked(int)));
	logicsVLayout->addWidget(translatableObjectCheckBox);
	scalableObjectCheckBox = new QCheckBox(logicsGroupBox);
	scalableObjectCheckBox->setText("ScalableObject");
	scalableObjectCheckBox->setEnabled(false); 
	connect(scalableObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ScalableObjectChecked(int))); 
	logicsVLayout->addWidget(scalableObjectCheckBox);
	rotatableObjectCheckBox = new QCheckBox(logicsGroupBox);
	rotatableObjectCheckBox->setText("RotatableObject");
	rotatableObjectCheckBox->setEnabled(false); 
	connect(rotatableObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(RotatableObjectChecked(int))); 
	logicsVLayout->addWidget(rotatableObjectCheckBox);
	lerpingObjectCheckBox = new QCheckBox(logicsGroupBox);
    lerpingObjectCheckBox->setText("LerpingObject");
	lerpingObjectCheckBox->setEnabled(false);
	connect(lerpingObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(LerpingObjectChecked(int)));
	logicsVLayout->addWidget(lerpingObjectCheckBox);
	activatableObjectCheckBox = new QCheckBox(logicsGroupBox);
    activatableObjectCheckBox->setText("ActivatableObject");
	activatableObjectCheckBox->setEnabled(false);
	connect(activatableObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ActivatableObjectChecked(int)));
	logicsVLayout->addWidget(activatableObjectCheckBox);
	graphicsSurfaceCheckBox = new QCheckBox(graphicsGroupBox);
    graphicsSurfaceCheckBox->setText("GraphicsSurface");
	connect(graphicsSurfaceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(GraphicsSurfaceChecked(int))); 
	graphicsVLayout->addWidget(graphicsSurfaceCheckBox);
	texturingSurfaceCheckBox = new QCheckBox(graphicsGroupBox);
    texturingSurfaceCheckBox->setText("TexturingSurface");
	connect(texturingSurfaceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TexturingSurfaceChecked(int))); 
	graphicsVLayout->addWidget(texturingSurfaceCheckBox);
	colorableObjectCheckBox = new QCheckBox(graphicsGroupBox);
	colorableObjectCheckBox->setText("ColorableObject");
	colorableObjectCheckBox->setEnabled(false); 
	connect(colorableObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ColorableObjectChecked(int))); 
	graphicsVLayout->addWidget(colorableObjectCheckBox);
	graphicsObjectCheckBox = new QCheckBox(graphicsGroupBox);
	graphicsObjectCheckBox->setText("GraphicsObject");
	graphicsObjectCheckBox->setEnabled(false); 
	connect(graphicsObjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(GraphicsObjectChecked(int)));
	graphicsVLayout->addWidget(graphicsObjectCheckBox);
	graphicsContainerCheckBox = new QCheckBox(graphicsGroupBox);
	graphicsContainerCheckBox->setText("GraphicsContainer");
	graphicsContainerCheckBox->setEnabled(false); 
	connect(graphicsContainerCheckBox, SIGNAL(stateChanged(int)), this, SLOT(GraphicsContainerChecked(int)));
	graphicsVLayout->addWidget(graphicsContainerCheckBox);
    graphicsStripCheckBox = new QCheckBox(graphicsGroupBox);
	graphicsStripCheckBox->setText("GraphicsStrip");
	graphicsStripCheckBox->setEnabled(true);
	connect(graphicsStripCheckBox, SIGNAL(stateChanged(int)), this, SLOT(GraphicsStripChecked(int)));
	graphicsVLayout->addWidget(graphicsStripCheckBox);
	nameLabel = new QLabel(this);
    nameLabel->setGeometry(QRect(30, 10, 41, 21));
	nameLabel->setText("Name:");

	nameMaxLength = 256;
	
	nameLineEdit = new QLineEdit(this);
    nameLineEdit->setGeometry(QRect(70, 10, 151+30, 20));
	nameLineEdit->setMaxLength(nameMaxLength);
	nameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	okButton = new QPushButton(this);
    okButton->setGeometry(QRect(250+250+250-90+10+10+30+20+20+20+60+200, 450-60-20+41+60+60+70+40+10, 91, 31));
    okButton->setText("Ok");
	okButton->setFocusPolicy(Qt::NoFocus);
	connect(okButton, SIGNAL(clicked()), this, SLOT(IsOk()));
	cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(140+250+250-90+10+10+30+20+20+20+60+200, 450-60-20+41+60+60+70+40+10, 91, 31));
    cancelButton->setText("Cancel");
	cancelButton->setFocusPolicy(Qt::NoFocus);
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setFocus();
	helpButton = new QToolButton(this);
	helpButton->setGeometry(QRect(140-50+250+250-90+6+10+10+30+20+20+20+60+200, 450-60-20+4+41+60+60-3+70+40+10, 32, 32));
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));


	setWindowTitle("Object Creator");
    setFixedSize(371+250+250-77+15+30+30+10+10+20+20+60+200, 508-60-25+41+60+60+70+40+10);
} 



ObjectCreator::~ObjectCreator(void)
{}



void ObjectCreator::SetObjectName(QString _name)
{
	nameLineEdit->setText(_name);
}


QString ObjectCreator::GetObjectName(void)const
{
	return nameLineEdit->text();
}


void ObjectCreator::SetNameDisable(bool _disable)
{
	nameLineEdit->setDisabled(_disable);
}



void ObjectCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}



void ObjectCreator::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "ObjectEditor.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}



QList<int64> ObjectCreator::GetActiveDisabledUnselectedProperties(void)
{ 

	QList<int64> properties;
	if(!abstractObjectCheckBox->isEnabled() && !abstractObjectCheckBox->isChecked())
	{
		properties.push_back(AbstractObject::ABSTRACT_OBJECT_CLASS);
	}
	if(!sceneCheckBox->isEnabled() && !sceneCheckBox->isChecked())
	{
		properties.push_back(Scene::SCENE_CLASS);
	}
	if(!objectSceneCheckBox->isEnabled() && !objectSceneCheckBox->isChecked())
	{
		properties.push_back(ObjectScene::OBJECT_SCENE_CLASS);
	}
	if(!cameraCheckBox->isEnabled() && !cameraCheckBox->isChecked())
	{
		properties.push_back(Camera3D::CAMERA_3D_CLASS);
	}
	if(!vectorLerpCheckBox->isEnabled() && !vectorLerpCheckBox->isChecked())
	{
		properties.push_back(VectorLerp::VECTOR_LERP_CLASS);
	}
	if(!logicSceneCheckBox->isEnabled() && !logicSceneCheckBox->isChecked())
	{
		properties.push_back(LogicScene::LOGIC_SCENE_CLASS);
	}
    if(!graphicsSceneCheckBox->isEnabled() && !graphicsSceneCheckBox->isChecked())
	{
		properties.push_back(GraphicsScene::GRAPHICS_SCENE_CLASS);
	}
    if(!tiledSpace3DCheckBox->isEnabled() && !tiledSpace3DCheckBox->isChecked())
	{
		properties.push_back(TiledSpace3D::TILED_SPACE_3D_CLASS);
	}
	if(!timerCheckBox->isEnabled() && !timerCheckBox->isChecked())
	{
		properties.push_back(Timer::TIMER_CLASS);
	}
	if(!abstractListCheckBox->isEnabled() && !abstractListCheckBox->isChecked())
	{
		properties.push_back(AbstractList::ABSTRACT_LIST_CLASS);
	}
	if(!planeCheckBox->isEnabled() && !planeCheckBox->isChecked())
	{
		properties.push_back(Plane::PLANE_CLASS);
	}
	if(!setCheckBox->isEnabled() && !setCheckBox->isChecked())
	{
		properties.push_back(Set::SET_CLASS);
	}
	if(!graphicsStripCheckBox->isEnabled() && !graphicsStripCheckBox->isChecked())
	{
		properties.push_back(GraphicsStrip::GRAPHICS_STRIP_CLASS);
	}
	if(!texturingSurfaceCheckBox->isEnabled() && !texturingSurfaceCheckBox->isChecked())
	{
		properties.push_back(TexturingSurface::TEXTURING_SURFACE_CLASS);
	}
	if(!graphicsSurfaceCheckBox->isEnabled() && !graphicsSurfaceCheckBox->isChecked())
	{
		properties.push_back(GraphicsSurface::GRAPHICS_SURFACE_CLASS);
	}
	if(!playerControlCheckBox->isEnabled() && !playerControlCheckBox->isChecked())
	{
		properties.push_back(PlayerControl::PLAYER_CONTROL_CLASS);
	}
	if(!eventCheckBox->isEnabled() && !eventCheckBox->isChecked())
	{
		properties.push_back(AbstractEvent::EVENT_CLASS);
	}
	if(!actionCheckBox->isEnabled() && !actionCheckBox->isChecked())
	{
		properties.push_back(AbstractAction::ACTION_CLASS);
	}
	if(!meshCheckBox->isEnabled() && !meshCheckBox->isChecked())
	{
		properties.push_back(Mesh::MESH_CLASS);
	}
	if(!hitboxCheckBox->isEnabled() && !hitboxCheckBox->isChecked())
	{
		properties.push_back(Hitbox::HITBOX_CLASS);
	}
	if(!triggerCheckBox->isEnabled() && !triggerCheckBox->isChecked())
	{
		properties.push_back(Trigger::TRIGGER_CLASS);
	}
	if(!floatLerpCheckBox->isEnabled() && !floatLerpCheckBox->isChecked())
	{
		properties.push_back(FloatLerp::FLOAT_LERP_CLASS);
	}
	if(!quaternionLerpCheckBox->isEnabled() && !quaternionLerpCheckBox->isChecked())
	{
		properties.push_back(QuaternionLerp::QUATERNION_LERP_CLASS);
	}
	if(!variableVector3CheckBox->isEnabled() && !variableVector3CheckBox->isChecked())
	{
		properties.push_back(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS);
	}
	if(!variableVector2CheckBox->isEnabled() && !variableVector2CheckBox->isChecked())
	{
		properties.push_back(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS);
	}
	if(!variableIntCheckBox->isEnabled() && !variableIntCheckBox->isChecked())
	{
		properties.push_back(Variable<int32>::INTEGER_VARIABLE_CLASS);
	}
	if(!variableFloatCheckBox->isEnabled() && !variableFloatCheckBox->isChecked())
	{
		properties.push_back(Variable<float>::FLOAT_VARIABLE_CLASS);
	}
	if(!variableBoolCheckBox->isEnabled() && !variableBoolCheckBox->isChecked())
	{
		properties.push_back(Variable<bool>::BOOLEAN_VARIABLE_CLASS);
	}
	if(!variableStringCheckBox->isEnabled() && !variableStringCheckBox->isChecked())
	{
		properties.push_back(Variable<StringANSI>::STRING_VARIABLE_CLASS);
	}
	if(!variableQuaternionCheckBox->isEnabled() && !variableQuaternionCheckBox->isChecked())
	{
		properties.push_back(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS);
	}
	if(!atlasCheckBox->isEnabled() && !atlasCheckBox->isChecked())
	{
		properties.push_back(Atlas2D::ATLAS_2D_CLASS);
	}
	if(!texturedAnimationCheckBox->isEnabled() && !texturedAnimationCheckBox->isChecked())
	{
		properties.push_back(TexturedAnimation::TEXTURED_ANIMATION_CLASS);
	}
	if(!textureCheckBox->isEnabled() && !textureCheckBox->isChecked())
	{
		properties.push_back(Texture2D::TEXTURE_2D_CLASS);
	}
	if(!translatableObjectCheckBox->isEnabled() && !translatableObjectCheckBox->isChecked())
	{
		properties.push_back(TranslatableObject::TRANSLATABLE_OBJECT_CLASS);
	}
	if(!scalableObjectCheckBox->isEnabled() && !scalableObjectCheckBox->isChecked())
	{
		properties.push_back(ScalableObject::SCALABLE_OBJECT_CLASS);
	}
	if(!rotatableObjectCheckBox->isEnabled() && !rotatableObjectCheckBox->isChecked())
	{
		properties.push_back(RotatableObject::ROTATABLE_OBJECT_CLASS);
	}
	if(!colorableObjectCheckBox->isEnabled() && !colorableObjectCheckBox->isChecked())
	{
		properties.push_back(ColorableObject::COLORABLE_OBJECT_CLASS);
	} 

    return properties;
}

 

QList<int64> ObjectCreator::GetEnabledUnselectedProperties(void)
{ 
 
	QList<int64> properties;
	if(abstractObjectCheckBox->isEnabled() && !abstractObjectCheckBox->isChecked())
	{
		properties.push_back(AbstractObject::ABSTRACT_OBJECT_CLASS);
	}
	if(sceneCheckBox->isEnabled() && !sceneCheckBox->isChecked())
	{
		properties.push_back(Scene::SCENE_CLASS);
	}
	if(objectSceneCheckBox->isEnabled() && !objectSceneCheckBox->isChecked())
	{
		properties.push_back(ObjectScene::OBJECT_SCENE_CLASS);
	}
	if(cameraCheckBox->isEnabled() && !cameraCheckBox->isChecked())
	{
		properties.push_back(Camera3D::CAMERA_3D_CLASS);
	}
	if(vectorLerpCheckBox->isEnabled() && !vectorLerpCheckBox->isChecked())
	{
		properties.push_back(VectorLerp::VECTOR_LERP_CLASS);
	}
	if(logicSceneCheckBox->isEnabled() && !logicSceneCheckBox->isChecked())
	{
		properties.push_back(LogicScene::LOGIC_SCENE_CLASS);
	}
    if(graphicsSceneCheckBox->isEnabled() && !graphicsSceneCheckBox->isChecked())
	{
		properties.push_back(GraphicsScene::GRAPHICS_SCENE_CLASS);
	}
    if(tiledSpace3DCheckBox->isEnabled() && !tiledSpace3DCheckBox->isChecked())
	{
		properties.push_back(TiledSpace3D::TILED_SPACE_3D_CLASS);
	}
	if(timerCheckBox->isEnabled() && !timerCheckBox->isChecked())
	{
		properties.push_back(Timer::TIMER_CLASS);
	}
	if(abstractListCheckBox->isEnabled() && !abstractListCheckBox->isChecked())
	{
		properties.push_back(AbstractList::ABSTRACT_LIST_CLASS);
	}
	if(planeCheckBox->isEnabled() && !planeCheckBox->isChecked())
	{
		properties.push_back(Plane::PLANE_CLASS);
	}
	if(setCheckBox->isEnabled() && !setCheckBox->isChecked())
	{
		properties.push_back(Set::SET_CLASS);
	}
	if(graphicsStripCheckBox->isEnabled() && !graphicsStripCheckBox->isChecked())
	{
		properties.push_back(GraphicsStrip::GRAPHICS_STRIP_CLASS);
	}
	if(texturingSurfaceCheckBox->isEnabled() && !texturingSurfaceCheckBox->isChecked())
	{
		properties.push_back(TexturingSurface::TEXTURING_SURFACE_CLASS);
	}
	if(graphicsSurfaceCheckBox->isEnabled() && !graphicsSurfaceCheckBox->isChecked())
	{
		properties.push_back(GraphicsSurface::GRAPHICS_SURFACE_CLASS);
	}
	if(playerControlCheckBox->isEnabled() && !playerControlCheckBox->isChecked())
	{
		properties.push_back(PlayerControl::PLAYER_CONTROL_CLASS);
	}
	if(eventCheckBox->isEnabled() && !eventCheckBox->isChecked())
	{
		properties.push_back(AbstractEvent::EVENT_CLASS);
	}
	if(actionCheckBox->isEnabled() && !actionCheckBox->isChecked())
	{
		properties.push_back(AbstractAction::ACTION_CLASS);
	}
	if(meshCheckBox->isEnabled() && !meshCheckBox->isChecked())
	{
		properties.push_back(Mesh::MESH_CLASS);
	}
	if(hitboxCheckBox->isEnabled() && !hitboxCheckBox->isChecked())
	{
		properties.push_back(Hitbox::HITBOX_CLASS);
	}
	if(triggerCheckBox->isEnabled() && !triggerCheckBox->isChecked())
	{
		properties.push_back(Trigger::TRIGGER_CLASS);
	}
	if(floatLerpCheckBox->isEnabled() && !floatLerpCheckBox->isChecked())
	{
		properties.push_back(FloatLerp::FLOAT_LERP_CLASS);
	}
	if(quaternionLerpCheckBox->isEnabled() && !quaternionLerpCheckBox->isChecked())
	{
		properties.push_back(QuaternionLerp::QUATERNION_LERP_CLASS);
	}
	if(variableVector3CheckBox->isEnabled() && !variableVector3CheckBox->isChecked())
	{
		properties.push_back(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS);
	}
	if(variableVector2CheckBox->isEnabled() && !variableVector2CheckBox->isChecked())
	{
		properties.push_back(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS);
	}
	if(variableIntCheckBox->isEnabled() && !variableIntCheckBox->isChecked())
	{
		properties.push_back(Variable<int32>::INTEGER_VARIABLE_CLASS);
	}
	if(variableFloatCheckBox->isEnabled() && !variableFloatCheckBox->isChecked())
	{
		properties.push_back(Variable<float>::FLOAT_VARIABLE_CLASS);
	}
	if(variableBoolCheckBox->isEnabled() && !variableBoolCheckBox->isChecked())
	{
		properties.push_back(Variable<bool>::BOOLEAN_VARIABLE_CLASS);
	}
	if(variableStringCheckBox->isEnabled() && !variableStringCheckBox->isChecked())
	{
		properties.push_back(Variable<StringANSI>::STRING_VARIABLE_CLASS);
	}
	if(variableQuaternionCheckBox->isEnabled() && !variableQuaternionCheckBox->isChecked())
	{
		properties.push_back(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS);
	}
	if(atlasCheckBox->isEnabled() && !atlasCheckBox->isChecked())
	{
		properties.push_back(Atlas2D::ATLAS_2D_CLASS);
	}
	if(texturedAnimationCheckBox->isEnabled() && !texturedAnimationCheckBox->isChecked())
	{
		properties.push_back(TexturedAnimation::TEXTURED_ANIMATION_CLASS);
	}
	if(textureCheckBox->isEnabled() && !textureCheckBox->isChecked())
	{
		properties.push_back(Texture2D::TEXTURE_2D_CLASS);
	}
	if(translatableObjectCheckBox->isEnabled() && !translatableObjectCheckBox->isChecked())
	{
		properties.push_back(TranslatableObject::TRANSLATABLE_OBJECT_CLASS);
	}
	if(scalableObjectCheckBox->isEnabled() && !scalableObjectCheckBox->isChecked())
	{
		properties.push_back(ScalableObject::SCALABLE_OBJECT_CLASS);
	}
	if(rotatableObjectCheckBox->isEnabled() && !rotatableObjectCheckBox->isChecked())
	{
		properties.push_back(RotatableObject::ROTATABLE_OBJECT_CLASS);
	}
	if(colorableObjectCheckBox->isEnabled() && !colorableObjectCheckBox->isChecked())
	{
		properties.push_back(ColorableObject::COLORABLE_OBJECT_CLASS);
	} 

    return properties;
}


QList<int64> ObjectCreator::GetActiveAndPassiveSelectedProperties(void)
{ 

	QList<int64> properties;
	if(abstractObjectCheckBox->isChecked())
	{
		properties.push_back(AbstractObject::ABSTRACT_OBJECT_CLASS);
	}
	if(sceneCheckBox->isChecked())
	{
		properties.push_back(Scene::SCENE_CLASS);
	}
	if(objectSceneCheckBox->isChecked())
	{
		properties.push_back(ObjectScene::OBJECT_SCENE_CLASS);
	}
	if(cameraCheckBox->isChecked())
	{
		properties.push_back(Camera3D::CAMERA_3D_CLASS);
	}
	if(vectorLerpCheckBox->isChecked())
	{
		properties.push_back(VectorLerp::VECTOR_LERP_CLASS);
	}
	if(logicSceneCheckBox->isChecked())
	{
		properties.push_back(LogicScene::LOGIC_SCENE_CLASS);
	}
    if(graphicsSceneCheckBox->isChecked())
	{
		properties.push_back(GraphicsScene::GRAPHICS_SCENE_CLASS);
	}
    if(tiledSpace3DCheckBox->isChecked())
	{
		properties.push_back(TiledSpace3D::TILED_SPACE_3D_CLASS);
	}
	if(timerCheckBox->isChecked())
	{
		properties.push_back(Timer::TIMER_CLASS);
	}
	if(abstractListCheckBox->isChecked())
	{
		properties.push_back(AbstractList::ABSTRACT_LIST_CLASS);
	}
	if(planeCheckBox->isChecked())
	{
		properties.push_back(Plane::PLANE_CLASS);
	}
	if(setCheckBox->isChecked())
	{
		properties.push_back(Set::SET_CLASS);
	}
	if(graphicsStripCheckBox->isChecked())
	{
		properties.push_back(GraphicsStrip::GRAPHICS_STRIP_CLASS);
	}
	if(texturingSurfaceCheckBox->isChecked())
	{
		properties.push_back(TexturingSurface::TEXTURING_SURFACE_CLASS);
	}
	if(graphicsSurfaceCheckBox->isChecked())
	{
		properties.push_back(GraphicsSurface::GRAPHICS_SURFACE_CLASS);
	}
	if(playerControlCheckBox->isChecked())
	{
		properties.push_back(PlayerControl::PLAYER_CONTROL_CLASS);
	}
	if(eventCheckBox->isChecked())
	{
		properties.push_back(AbstractEvent::EVENT_CLASS);
	}
	if(actionCheckBox->isChecked())
	{
		properties.push_back(AbstractAction::ACTION_CLASS);
	}
	if(meshCheckBox->isChecked())
	{
		properties.push_back(Mesh::MESH_CLASS);
	}
	if(hitboxCheckBox->isChecked())
	{
		properties.push_back(Hitbox::HITBOX_CLASS);
	}
	if(triggerCheckBox->isChecked())
	{
		properties.push_back(Trigger::TRIGGER_CLASS);
	}
	if(floatLerpCheckBox->isChecked())
	{
		properties.push_back(FloatLerp::FLOAT_LERP_CLASS);
	}
	if(quaternionLerpCheckBox->isChecked())
	{
		properties.push_back(QuaternionLerp::QUATERNION_LERP_CLASS);
	}
	if(polygonalSurfaceCheckBox->isChecked())
	{
		properties.push_back(PolygonalSurface::POLYGONAL_SURFACE_CLASS);
	}
	if(cameraCheckBox->isChecked())
	{
		properties.push_back(Camera3D::CAMERA_3D_CLASS);
	}
	if(vectorLerpCheckBox->isChecked())
	{ 
		properties.push_back(VectorLerp::VECTOR_LERP_CLASS);
	}
	if(logicObjectCheckBox->isChecked())
	{
		properties.push_back(LogicObject::LOGIC_OBJECT_CLASS);
	}
	if(lerpingObjectCheckBox->isChecked())
	{
		properties.push_back(LerpingObject::LERPING_OBJECT_CLASS);
	}
	if(activatableObjectCheckBox->isChecked())
	{
		properties.push_back(ActivatableObject::ACTIVATABLE_OBJECT_CLASS);
	}
	if(abstractVariableCheckBox->isChecked())
	{
		properties.push_back(AbstractVariable::ABSTRACT_VARIABLE_CLASS);
	}
	if(playingObjectCheckBox->isChecked())
	{
		properties.push_back(PlayingObject::PLAYING_OBJECT_CLASS);
	}
	if(graphicsObjectCheckBox->isChecked())
	{
		properties.push_back(GraphicsObject::GRAPHICS_OBJECT_CLASS);
	}
	if(graphicsContainerCheckBox->isChecked())
	{
		properties.push_back(GraphicsContainer::GRAPHICS_CONTAINER_CLASS);
	}
	if(colorableObjectCheckBox->isChecked())
	{
		properties.push_back(ColorableObject::COLORABLE_OBJECT_CLASS);
	}
	if(translatableObjectCheckBox->isChecked())
	{
		properties.push_back(TranslatableObject::TRANSLATABLE_OBJECT_CLASS);
	}
	if(scalableObjectCheckBox->isChecked())
	{
		properties.push_back(ScalableObject::SCALABLE_OBJECT_CLASS);
	}
	if(rotatableObjectCheckBox->isChecked())
	{
		properties.push_back(RotatableObject::ROTATABLE_OBJECT_CLASS);
	}
	if(abstractSurfaceCheckBox->isChecked())
	{
		properties.push_back(AbstractSurface::ABSTRACT_SURFACE_CLASS);
	}
	if(variableVector3CheckBox->isChecked())
	{
		properties.push_back(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS);
	}
	if(variableVector2CheckBox->isChecked())
	{
		properties.push_back(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS);
	}
	if(variableIntCheckBox->isChecked())
	{
		properties.push_back(Variable<int32>::INTEGER_VARIABLE_CLASS);
	}
	if(variableFloatCheckBox->isChecked())
	{
		properties.push_back(Variable<float>::FLOAT_VARIABLE_CLASS);
	}
	if(variableBoolCheckBox->isChecked())
	{
		properties.push_back(Variable<bool>::BOOLEAN_VARIABLE_CLASS);
	}
	if(variableStringCheckBox->isChecked())
	{
		properties.push_back(Variable<StringANSI>::STRING_VARIABLE_CLASS);
	}
	if(variableQuaternionCheckBox->isChecked())
	{
		properties.push_back(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS);
	}
	if(atlasCheckBox->isChecked())
	{
		properties.push_back(Atlas2D::ATLAS_2D_CLASS);
	}
	if(abstractAnimationCheckBox->isChecked())
	{ 
		properties.push_back(AbstractAnimation::ABSTRACT_ANIMATION_CLASS);
	}
	if(texturedAnimationCheckBox->isChecked())
	{
		properties.push_back(TexturedAnimation::TEXTURED_ANIMATION_CLASS);
	}
	if(textureCheckBox->isChecked())
	{
		properties.push_back(Texture2D::TEXTURE_2D_CLASS);
	}

    return properties;
}



QList<int64> ObjectCreator::GetActiveSelectedProperties(void)
{ 

	QList<int64> properties;
	if(abstractObjectCheckBox->isChecked())
	{
		properties.push_back(AbstractObject::ABSTRACT_OBJECT_CLASS);
	}
	if(sceneCheckBox->isChecked())
	{
		properties.push_back(Scene::SCENE_CLASS);
	}
	if(objectSceneCheckBox->isChecked())
	{
		properties.push_back(ObjectScene::OBJECT_SCENE_CLASS);
	}
	if(cameraCheckBox->isChecked())
	{
		properties.push_back(Camera3D::CAMERA_3D_CLASS);
	}
	if(vectorLerpCheckBox->isChecked())
	{
		properties.push_back(VectorLerp::VECTOR_LERP_CLASS);
	}
	if(logicSceneCheckBox->isChecked())
	{
		properties.push_back(LogicScene::LOGIC_SCENE_CLASS);
	}
    if(graphicsSceneCheckBox->isChecked())
	{
		properties.push_back(GraphicsScene::GRAPHICS_SCENE_CLASS);
	}
    if(tiledSpace3DCheckBox->isChecked())
	{
		properties.push_back(TiledSpace3D::TILED_SPACE_3D_CLASS);
	}
	if(timerCheckBox->isChecked())
	{
		properties.push_back(Timer::TIMER_CLASS);
	}
	if(abstractListCheckBox->isChecked())
	{
		properties.push_back(AbstractList::ABSTRACT_LIST_CLASS);
	}
	if(planeCheckBox->isChecked())
	{
		properties.push_back(Plane::PLANE_CLASS);
	}
	if(setCheckBox->isChecked())
	{
		properties.push_back(Set::SET_CLASS);
	}
	if(graphicsStripCheckBox->isChecked())
	{
		properties.push_back(GraphicsStrip::GRAPHICS_STRIP_CLASS);
	}
	if(texturingSurfaceCheckBox->isChecked())
	{
		properties.push_back(TexturingSurface::TEXTURING_SURFACE_CLASS);
	}
	if(graphicsSurfaceCheckBox->isChecked())
	{
		properties.push_back(GraphicsSurface::GRAPHICS_SURFACE_CLASS);
	}
	if(playerControlCheckBox->isChecked())
	{
		properties.push_back(PlayerControl::PLAYER_CONTROL_CLASS);
	}
	if(eventCheckBox->isChecked())
	{
		properties.push_back(AbstractEvent::EVENT_CLASS);
	}
	if(actionCheckBox->isChecked())
	{
		properties.push_back(AbstractAction::ACTION_CLASS);
	}
	if(meshCheckBox->isChecked())
	{
		properties.push_back(Mesh::MESH_CLASS);
	}
	if(hitboxCheckBox->isChecked())
	{
		properties.push_back(Hitbox::HITBOX_CLASS);
	}
	if(triggerCheckBox->isChecked())
	{
		properties.push_back(Trigger::TRIGGER_CLASS);
	}
	if(floatLerpCheckBox->isChecked())
	{
		properties.push_back(FloatLerp::FLOAT_LERP_CLASS);
	}
	if(quaternionLerpCheckBox->isChecked())
	{
		properties.push_back(QuaternionLerp::QUATERNION_LERP_CLASS);
	}
	if(variableVector3CheckBox->isChecked())
	{
		properties.push_back(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS);
	}
	if(variableVector2CheckBox->isChecked())
	{
		properties.push_back(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS);
	}
	if(variableIntCheckBox->isChecked())
	{
		properties.push_back(Variable<int32>::INTEGER_VARIABLE_CLASS);
	}
	if(variableFloatCheckBox->isChecked())
	{
		properties.push_back(Variable<float>::FLOAT_VARIABLE_CLASS);
	}
	if(variableBoolCheckBox->isChecked())
	{
		properties.push_back(Variable<bool>::BOOLEAN_VARIABLE_CLASS);
	}
	if(variableStringCheckBox->isChecked())
	{
		properties.push_back(Variable<StringANSI>::STRING_VARIABLE_CLASS);
	}
	if(variableQuaternionCheckBox->isChecked())
	{
		properties.push_back(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS);
	}
	if(atlasCheckBox->isChecked())
	{
		properties.push_back(Atlas2D::ATLAS_2D_CLASS);
	}
	if(texturedAnimationCheckBox->isChecked())
	{
		properties.push_back(TexturedAnimation::TEXTURED_ANIMATION_CLASS);
	}
	if(textureCheckBox->isChecked())
	{
		properties.push_back(Texture2D::TEXTURE_2D_CLASS);
	}
	if(translatableObjectCheckBox->isChecked() && translatableObjectCheckBox->isEnabled())
	{
		properties.push_back(TranslatableObject::TRANSLATABLE_OBJECT_CLASS);
	}
	if(scalableObjectCheckBox->isChecked() && scalableObjectCheckBox->isEnabled())
	{
		properties.push_back(ScalableObject::SCALABLE_OBJECT_CLASS);
	}
	if(rotatableObjectCheckBox->isChecked() && rotatableObjectCheckBox->isEnabled())
	{
		properties.push_back(RotatableObject::ROTATABLE_OBJECT_CLASS);
	}
	if(colorableObjectCheckBox->isChecked() && colorableObjectCheckBox->isEnabled())
	{
		properties.push_back(ColorableObject::COLORABLE_OBJECT_CLASS);
	}

    return properties;
}

 


void ObjectCreator::SelectCheckBox(int64 _property, bool _enable)
{
	if(_property == AbstractObject::ABSTRACT_OBJECT_CLASS)
	{
		abstractObjectCheckBox->setChecked(_enable);
	}
	else if(_property == Scene::SCENE_CLASS)
	{
		sceneCheckBox->setChecked(_enable);
	}
	else if(_property == ObjectScene::OBJECT_SCENE_CLASS)
	{
		objectSceneCheckBox->setChecked(_enable);
	}
	else if(_property == Camera3D::CAMERA_3D_CLASS)
	{
		cameraCheckBox->setChecked(_enable);
	}
	else if(_property == VectorLerp::VECTOR_LERP_CLASS)
	{
		vectorLerpCheckBox->setChecked(_enable);
	}
	else if(_property == LogicScene::LOGIC_SCENE_CLASS)
	{
		logicSceneCheckBox->setChecked(_enable);
	}
    else if(_property == GraphicsScene::GRAPHICS_SCENE_CLASS)
	{
		graphicsSceneCheckBox->setChecked(_enable);
	}
    else if(_property == TiledSpace3D::TILED_SPACE_3D_CLASS)
	{
		tiledSpace3DCheckBox->setChecked(_enable);
	}
	else if(_property == Timer::TIMER_CLASS)
	{
		timerCheckBox->setChecked(_enable);
	}
	else if(_property == AbstractList::ABSTRACT_LIST_CLASS)
	{
		abstractListCheckBox->setChecked(_enable);
	}
	else if(_property == Plane::PLANE_CLASS)
	{
		planeCheckBox->setChecked(_enable);
	}
	else if(_property == Set::SET_CLASS)
	{
		setCheckBox->setChecked(_enable);
	}
	else if(_property == GraphicsStrip::GRAPHICS_STRIP_CLASS)
	{
		graphicsStripCheckBox->setChecked(_enable);
	}
	else if(_property == TexturingSurface::TEXTURING_SURFACE_CLASS)
	{
		texturingSurfaceCheckBox->setChecked(_enable);
	}
	else if(_property == GraphicsSurface::GRAPHICS_SURFACE_CLASS)
	{
		graphicsSurfaceCheckBox->setChecked(_enable);
	}
	else if(_property == PlayerControl::PLAYER_CONTROL_CLASS)
	{
		playerControlCheckBox->setChecked(_enable);
	}
	else if(_property == AbstractEvent::EVENT_CLASS)
	{
		eventCheckBox->setChecked(_enable);
	}
	else if(_property == AbstractAction::ACTION_CLASS)
	{
		actionCheckBox->setChecked(_enable);
	}
	else if(_property == Mesh::MESH_CLASS)
	{
		meshCheckBox->setChecked(_enable);
	}
	else if(_property == Hitbox::HITBOX_CLASS)
	{
		hitboxCheckBox->setChecked(_enable);
	}
	else if(_property == Trigger::TRIGGER_CLASS)
	{
		triggerCheckBox->setChecked(_enable);
	}
	else if(_property == FloatLerp::FLOAT_LERP_CLASS)
	{
		floatLerpCheckBox->setChecked(_enable);
	}
	else if(_property == QuaternionLerp::QUATERNION_LERP_CLASS)
	{
		quaternionLerpCheckBox->setChecked(_enable);
	}
	else if(_property == Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)
	{
		variableVector3CheckBox->setChecked(_enable);
	}
	else if(_property == Variable<Vector2>::VECTOR_2_VARIABLE_CLASS)
	{
		variableVector2CheckBox->setChecked(_enable);
	}
	else if(_property == Variable<int32>::INTEGER_VARIABLE_CLASS)
	{
		variableIntCheckBox->setChecked(_enable);
	}
	else if(_property == Variable<float>::FLOAT_VARIABLE_CLASS)
	{
		variableFloatCheckBox->setChecked(_enable);
	}
	else if(_property == Variable<bool>::BOOLEAN_VARIABLE_CLASS)
	{
		variableBoolCheckBox->setChecked(_enable);
	}
	else if(_property == Variable<StringANSI>::STRING_VARIABLE_CLASS)
	{
		variableStringCheckBox->setChecked(_enable);
	}
	else if(_property == Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)
	{
		variableQuaternionCheckBox->setChecked(_enable);
	}
	else if(_property == Atlas2D::ATLAS_2D_CLASS)
	{
		atlasCheckBox->setChecked(_enable);
	}
	else if(_property == TexturedAnimation::TEXTURED_ANIMATION_CLASS)
	{
		texturedAnimationCheckBox->setChecked(_enable);
	}
	else if(_property == Texture2D::TEXTURE_2D_CLASS)
	{
		textureCheckBox->setChecked(_enable);
	}
	else if(_property == TranslatableObject::TRANSLATABLE_OBJECT_CLASS)
	{
		if(translatableObjectCheckBox->isEnabled())
		{
			translatableObjectCheckBox->setChecked(_enable);
		}
	}
	else if(_property == ScalableObject::SCALABLE_OBJECT_CLASS)
	{
		if(scalableObjectCheckBox->isEnabled())
		{
			scalableObjectCheckBox->setChecked(_enable);
		}
	}
	else if(_property == RotatableObject::ROTATABLE_OBJECT_CLASS)
	{
		if(rotatableObjectCheckBox->isEnabled())
		{
			rotatableObjectCheckBox->setChecked(_enable);
		}
	}
	else if(_property == ColorableObject::COLORABLE_OBJECT_CLASS)
	{
		if(colorableObjectCheckBox->isEnabled())
		{
			colorableObjectCheckBox->setChecked(_enable);
		}
	}
}
 

void ObjectCreator::DisableCheckBox(int64 _property, bool _disable)
{
	if(_property == AbstractObject::ABSTRACT_OBJECT_CLASS)
	{
		abstractObjectCheckBox->setDisabled(_disable);
	}
	else if(_property == Scene::SCENE_CLASS)
	{
		sceneCheckBox->setDisabled(_disable);
	}
	else if(_property == ObjectScene::OBJECT_SCENE_CLASS)
	{
		objectSceneCheckBox->setDisabled(_disable);
	}
	else if(_property == Camera3D::CAMERA_3D_CLASS)
	{
		cameraCheckBox->setDisabled(_disable);
	}
	else if(_property == VectorLerp::VECTOR_LERP_CLASS)
	{
		vectorLerpCheckBox->setDisabled(_disable);
	}
	else if(_property == LogicScene::LOGIC_SCENE_CLASS)
	{
		logicSceneCheckBox->setDisabled(_disable);
	}
    else if(_property == GraphicsScene::GRAPHICS_SCENE_CLASS)
	{
		graphicsSceneCheckBox->setDisabled(_disable);
	}
    else if(_property == TiledSpace3D::TILED_SPACE_3D_CLASS)
	{
		tiledSpace3DCheckBox->setDisabled(_disable);
	}
	else if(_property == Timer::TIMER_CLASS)
	{
		timerCheckBox->setDisabled(_disable);
	}
	else if(_property == AbstractList::ABSTRACT_LIST_CLASS)
	{
		abstractListCheckBox->setDisabled(_disable);
	}
	else if(_property == Plane::PLANE_CLASS)
	{
		planeCheckBox->setDisabled(_disable);
	}
	else if(_property == Set::SET_CLASS)
	{
		setCheckBox->setDisabled(_disable);
	}
	else if(_property == GraphicsStrip::GRAPHICS_STRIP_CLASS)
	{
		graphicsStripCheckBox->setDisabled(_disable);
	}
	else if(_property == TexturingSurface::TEXTURING_SURFACE_CLASS)
	{
		texturingSurfaceCheckBox->setDisabled(_disable);
	}
	else if(_property == GraphicsSurface::GRAPHICS_SURFACE_CLASS)
	{
		graphicsSurfaceCheckBox->setDisabled(_disable);
	}
	else if(_property == PlayerControl::PLAYER_CONTROL_CLASS)
	{
		playerControlCheckBox->setDisabled(_disable);
	}
	else if(_property == AbstractEvent::EVENT_CLASS)
	{
		eventCheckBox->setDisabled(_disable);
	}
	else if(_property == AbstractAction::ACTION_CLASS)
	{
		actionCheckBox->setDisabled(_disable);
	}
	else if(_property == Mesh::MESH_CLASS)
	{
		meshCheckBox->setDisabled(_disable);
	}
	else if(_property == Hitbox::HITBOX_CLASS)
	{
		hitboxCheckBox->setDisabled(_disable);
	}
	else if(_property == Trigger::TRIGGER_CLASS)
	{
		triggerCheckBox->setDisabled(_disable);
	}
	else if(_property == FloatLerp::FLOAT_LERP_CLASS)
	{
		floatLerpCheckBox->setDisabled(_disable);
	}
	else if(_property == QuaternionLerp::QUATERNION_LERP_CLASS)
	{
		quaternionLerpCheckBox->setDisabled(_disable);
	}
	else if(_property == Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)
	{
		variableVector3CheckBox->setDisabled(_disable);
	}
	else if(_property == Variable<Vector2>::VECTOR_2_VARIABLE_CLASS)
	{
		variableVector2CheckBox->setDisabled(_disable);
	}
	else if(_property == Variable<int32>::INTEGER_VARIABLE_CLASS)
	{
		variableIntCheckBox->setDisabled(_disable);
	}
	else if(_property == Variable<float>::FLOAT_VARIABLE_CLASS)
	{
		variableFloatCheckBox->setDisabled(_disable);
	}
	else if(_property == Variable<bool>::BOOLEAN_VARIABLE_CLASS)
	{
		variableBoolCheckBox->setDisabled(_disable);
	}
	else if(_property == Variable<StringANSI>::STRING_VARIABLE_CLASS)
	{
		variableStringCheckBox->setDisabled(_disable);
	}
	else if(_property == Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)
	{
		variableQuaternionCheckBox->setDisabled(_disable);
	}
	else if(_property == Atlas2D::ATLAS_2D_CLASS)
	{
		atlasCheckBox->setDisabled(_disable);
	}
	else if(_property == TexturedAnimation::TEXTURED_ANIMATION_CLASS)
	{
		texturedAnimationCheckBox->setDisabled(_disable);
	}
	else if(_property == Texture2D::TEXTURE_2D_CLASS)
	{
		textureCheckBox->setDisabled(_disable);
	}
	else if(_property == TranslatableObject::TRANSLATABLE_OBJECT_CLASS)
	{
		translatableObjectCheckBox->setDisabled(_disable);
	}
	else if(_property == ScalableObject::SCALABLE_OBJECT_CLASS)
	{
		scalableObjectCheckBox->setDisabled(_disable);
	}
	else if(_property == RotatableObject::ROTATABLE_OBJECT_CLASS)
	{
		rotatableObjectCheckBox->setDisabled(_disable);
	}
	else if(_property == ColorableObject::COLORABLE_OBJECT_CLASS)
	{
		colorableObjectCheckBox->setDisabled(_disable);
	}
} 




void ObjectCreator::DisableIncompatibleProperties(int _enable, int64 _property)
{
	if(_enable)
	{ 
		QList<int64> enabledUnselectedProperties = GetEnabledUnselectedProperties();
		enabledUnselectedProperties.removeAt(enabledUnselectedProperties.indexOf(_property));
  
		QList<int64> selectedProperties = GetActiveSelectedProperties();
		selectedProperties.push_back(_property);
		int64 type = 0;
		for(int i = 0; i < selectedProperties.size(); i++)
		{ 
			type |= selectedProperties.at(i);
		}
		for(int i = 0; i < enabledUnselectedProperties.size(); ++i)
		{
			if(!AbstractObject::_IsSupported(type | enabledUnselectedProperties.at(i)))
			{ 
				DisableCheckBox(enabledUnselectedProperties.at(i), true);
			} 
		}

		QList<int64> disabledProperties = GetActiveDisabledUnselectedProperties();
		for(int i = 0; i < disabledProperties.size(); i++)
		{  
			if(AbstractObject::_IsSupported(type | disabledProperties.at(i)))
			{ 
				DisableCheckBox(disabledProperties.at(i), false);
			}
		} 
	} 
	else
	{  
		QList<int64> selectedProperties = GetActiveSelectedProperties();
		selectedProperties.removeAt(selectedProperties.indexOf(_property));
		int64 type = 0;
		for(int i = 0; i < selectedProperties.size(); i++)
		{
			type |= selectedProperties.at(i);  
		} 

		QList<int64> enabledUnselectedProperties = GetEnabledUnselectedProperties();
		enabledUnselectedProperties.push_back(_property);
		for(int i = 0; i < enabledUnselectedProperties.size(); ++i)  
		{
			if(!AbstractObject::_IsSupported(type | enabledUnselectedProperties.at(i)))
			{ 
				DisableCheckBox(enabledUnselectedProperties.at(i), true);
			}
		}
		for(int i = 0; i < selectedProperties.size(); ++i)
		{
			int64 type = 0;
			for(int j = 0; j < selectedProperties.size(); j++)
			{
				type |= selectedProperties.at(j);
			}

			type &= ~selectedProperties.at(i);
			if(!AbstractObject::_IsSupported(type | selectedProperties.at(i)))
			{   
				SelectCheckBox(selectedProperties.at(i), false);
							
				DisableCheckBox(selectedProperties.at(i), true);
			}
		}
	}
}
 

void ObjectCreator::EnableCompatibleProperties(QList<int64> _selectedProperties)
{ 
	int64 type = 0;
	for(int i = 0; i < _selectedProperties.size(); i++)
	{
		type |= _selectedProperties.at(i);  
	} 

	QList<int64> disabledProperties = GetActiveDisabledUnselectedProperties();
	for(int i = 0; i < disabledProperties.size(); ++i)
	{
		if(AbstractObject::_IsSupported(type | disabledProperties.at(i)))
		{ 
			DisableCheckBox(disabledProperties.at(i), false); 
		}
	}
}


int64 ObjectCreator::GetId(QCheckBox* _checkBox)
{
	if(_checkBox == abstractObjectCheckBox)
	{
		return AbstractObject::ABSTRACT_OBJECT_CLASS;
	}
	else if(_checkBox == sceneCheckBox)
	{
		return Scene::SCENE_CLASS;
	}
	else if(_checkBox == objectSceneCheckBox)
	{
		return ObjectScene::OBJECT_SCENE_CLASS;
	}
	else if(_checkBox == cameraCheckBox)
	{
		return Camera3D::CAMERA_3D_CLASS;
	}
	else if(_checkBox == vectorLerpCheckBox)
	{
		return VectorLerp::VECTOR_LERP_CLASS;
	}
	else if(_checkBox == logicSceneCheckBox)
	{
		return LogicScene::LOGIC_SCENE_CLASS;
	}
    else if(_checkBox == graphicsSceneCheckBox)
	{
		return GraphicsScene::GRAPHICS_SCENE_CLASS;
	}
    else if(_checkBox == tiledSpace3DCheckBox)
	{
		return TiledSpace3D::TILED_SPACE_3D_CLASS;
	}
	else if(_checkBox == timerCheckBox)
	{
		return Timer::TIMER_CLASS;
	}
	else if(_checkBox == abstractListCheckBox)
	{
		return AbstractList::ABSTRACT_LIST_CLASS;
	}
	else if(_checkBox == planeCheckBox)
	{
		return Plane::PLANE_CLASS;
	}
	else if(_checkBox == setCheckBox)
	{
		return Set::SET_CLASS;
	}
	else if(_checkBox == graphicsStripCheckBox)
	{
		return GraphicsStrip::GRAPHICS_STRIP_CLASS;
	}
	else if(_checkBox == texturingSurfaceCheckBox)
	{
		return TexturingSurface::TEXTURING_SURFACE_CLASS;
	}
	else if(_checkBox == graphicsSurfaceCheckBox)
	{
		return GraphicsSurface::GRAPHICS_SURFACE_CLASS;
	}
	else if(_checkBox == playerControlCheckBox)
	{
		return PlayerControl::PLAYER_CONTROL_CLASS;
	}
	else if(_checkBox == eventCheckBox)
	{
		return AbstractEvent::EVENT_CLASS;
	}
	else if(_checkBox == actionCheckBox)
	{
		return AbstractAction::ACTION_CLASS;
	}
   	else if(_checkBox == meshCheckBox)
	{
		return Mesh::MESH_CLASS;
	}
   	else if(_checkBox == hitboxCheckBox)
	{
		return Hitbox::HITBOX_CLASS;
	}
	else if(_checkBox == triggerCheckBox)
	{
		return Trigger::TRIGGER_CLASS;
	}
	else if(_checkBox == floatLerpCheckBox)
	{
		return FloatLerp::FLOAT_LERP_CLASS;
	}
	else if(_checkBox == quaternionLerpCheckBox)
	{
		return QuaternionLerp::QUATERNION_LERP_CLASS;
	}
	else if(_checkBox == variableVector3CheckBox)
	{
		return Variable<Vector3>::VECTOR_3_VARIABLE_CLASS;
	}
	else if(_checkBox == variableVector2CheckBox)
	{
		return Variable<Vector2>::VECTOR_2_VARIABLE_CLASS;
	}
	else if(_checkBox == variableIntCheckBox)
	{
		return Variable<int32>::INTEGER_VARIABLE_CLASS;
	}
	else if(_checkBox == variableFloatCheckBox)
	{
		return Variable<float>::FLOAT_VARIABLE_CLASS;
	}
	else if(_checkBox == variableBoolCheckBox)
	{
		return Variable<bool>::BOOLEAN_VARIABLE_CLASS;
	}
	else if(_checkBox == variableStringCheckBox)
	{
		return Variable<StringANSI>::STRING_VARIABLE_CLASS;
	}
	else if(_checkBox == variableQuaternionCheckBox)
	{
		return Variable<Quaternion>::QUATERNION_VARIABLE_CLASS;
	}
	else if(_checkBox == atlasCheckBox)
	{
		return Atlas2D::ATLAS_2D_CLASS;
	}
	else if(_checkBox == texturedAnimationCheckBox)
	{
		return TexturedAnimation::TEXTURED_ANIMATION_CLASS;
	}
	else if(_checkBox == textureCheckBox)
	{
		return Texture2D::TEXTURE_2D_CLASS;
	}

	return 0;
}


 

void ObjectCreator::ReviewObject(QString _path)
{  
	StringANSI name = File::_GetBaseName(_path.toAscii().data());
	StringANSI suffix = File::_GetSuffix(_path.toAscii().data());
  
	
	if(AssetLibrary::_IsAssetExist(name, AssetLibrary::_GetAssetTypeOfFileSuffix(suffix)))
	{
		nameLineEdit->setText(name.c_str());
		SetNameDisable(false);

		if(suffix == AbstractObject::_GetFileSuffix())
		{ 
			AbstractObject* object = AssetLibrary::_LoadPrivateAsset(name, AssetLibrary::_GetAssetTypeOfFileSuffix(suffix));

			if(object)
			{
				if(object->GetClassProperties() & TexturingSurface::TEXTURING_SURFACE_CLASS)
				{
					SelectCheckBox(TexturingSurface::TEXTURING_SURFACE_CLASS, true);
				}
				if(object->GetClassProperties() & GraphicsSurface::GRAPHICS_SURFACE_CLASS)
				{
					SelectCheckBox(GraphicsSurface::GRAPHICS_SURFACE_CLASS, true);
				} 

				if(object->GetClassProperties() & Plane::PLANE_CLASS)
				{
					SelectCheckBox(Plane::PLANE_CLASS, true);
				} 
				if(object->GetClassProperties() & Set::SET_CLASS)
				{
					SelectCheckBox(Set::SET_CLASS, true);
				}
				if(object->GetClassProperties() & GraphicsStrip::GRAPHICS_STRIP_CLASS)
				{
					SelectCheckBox(GraphicsStrip::GRAPHICS_STRIP_CLASS, true);
				}
				if(object->GetClassProperties() & Timer::TIMER_CLASS)
				{
					SelectCheckBox(Timer::TIMER_CLASS, true);
				}
				if(object->GetClassProperties() & AbstractObject::ABSTRACT_OBJECT_CLASS)
				{
					SelectCheckBox(AbstractObject::ABSTRACT_OBJECT_CLASS, true);
				} 
				if(object->GetClassProperties() & Scene::SCENE_CLASS)
				{
					SelectCheckBox(Scene::SCENE_CLASS, true);
				}
			}
		}
		else if(suffix == ObjectScene::_GetFileSuffix())
		{
			File file(AssetLibrary::_GetDir() + ObjectScene::_GetDir() + name + "." + suffix, File::READ);

			if(file.Open(File::READ))
			{
				int32 version;
				file.Read(version);
				int64 properties;
				file.Read(properties);
				StringANSI name;
				int32 length;
				file.Read(length);
				file.ReadString(name, length);
				AbstractObject::ObjectType type;
				file.Read(type);

				if(!(type.GetProperties() & Scene::SCENE_CLASS))
				{
					if(type.GetProperties() & LogicScene::LOGIC_SCENE_CLASS)
					{
						SelectCheckBox(LogicScene::LOGIC_SCENE_CLASS, true);
					}
					else if(type.GetProperties() & GraphicsScene::GRAPHICS_SCENE_CLASS)
					{
						SelectCheckBox(GraphicsScene::GRAPHICS_SCENE_CLASS, true);
					}
					else if(type.GetProperties() & TiledSpace3D::TILED_SPACE_3D_CLASS)
					{
						SelectCheckBox(TiledSpace3D::TILED_SPACE_3D_CLASS, true);
					}
					else if(type.GetProperties() & ObjectScene::OBJECT_SCENE_CLASS)
					{
						SelectCheckBox(ObjectScene::OBJECT_SCENE_CLASS, true);
					}
				} 
				else
				{
					SelectCheckBox(Scene::SCENE_CLASS, true);
				}
			}
		}
		else if(suffix == AbstractList::_GetFileSuffix())
		{
			File file(AssetLibrary::_GetDir() + AbstractList::_GetDir() + name + "." + suffix, File::READ);

			if(file.Open(File::READ))
			{
				int32 version;
				file.Read(version);
				int32 type;
				file.Read(type);
				StringANSI name;
				int32 length;
				file.Read(length);
				file.ReadString(name, length);

				if(type == List<int32>::INTEGER_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(Variable<int32>::INTEGER_VARIABLE_CLASS, true);
				}
				else if(type == List<float>::FLOAT_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(Variable<float>::FLOAT_VARIABLE_CLASS, true);
				}
				else if(type == List<bool>::BOOLEAN_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(Variable<bool>::BOOLEAN_VARIABLE_CLASS, true);
				}
				else if(type == List<StringANSI>::STRING_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(Variable<StringANSI>::STRING_VARIABLE_CLASS, true);
				}
				else if(type == List<Vector2>::VECTOR_2_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS, true);
				}
				else if(type == List<Vector3>::VECTOR_3_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS, true);
				}
				else if(type == List<Quaternion>::QUATERNION_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS, true);
				}
				else if(type == List<AbstractObject>::OBJECT_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
				}
				else if(type == List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(TranslatableObject::TRANSLATABLE_OBJECT_CLASS, true);
				}
				else if(type == List<ScalableObject>::SCALABLE_OBJECT_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(ScalableObject::SCALABLE_OBJECT_CLASS, true);
				}
				else if(type == List<RotatableObject>::ROTATABLE_OBJECT_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(RotatableObject::ROTATABLE_OBJECT_CLASS, true);
				}
				else if(type == List<ColorableObject>::COLORABLE_OBJECT_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(ColorableObject::COLORABLE_OBJECT_CLASS, true);
				}
				else if(type == List<Set>::SET_TYPE)
				{
					SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
					SelectCheckBox(Set::SET_CLASS, true);
				}
			}
		}
		else if(suffix == AbstractVariable::_GetFileSuffix())
		{ 
			File file(AssetLibrary::_GetDir() + AbstractVariable::_GetDir() + name + "." + suffix, File::READ);

			if(file.Open(File::READ))
			{
				int32 version;
				file.Read(version);
				int32 type;
				file.Read(type);
				StringANSI name;
				int32 length;
				file.Read(length);
				file.ReadString(name, length);


				if(type == Variable<int32>::INTEGER_TYPE)
				{
					SelectCheckBox(Variable<int32>::INTEGER_VARIABLE_CLASS, true);
				}
				else if(type == Variable<float>::FLOAT_TYPE)
				{
					SelectCheckBox(Variable<float>::FLOAT_VARIABLE_CLASS, true);
				}
				else if(type == Variable<bool>::BOOLEAN_TYPE)
				{
					SelectCheckBox(Variable<bool>::BOOLEAN_VARIABLE_CLASS, true);
				}
				else if(type == Variable<StringANSI>::STRING_TYPE)
				{
					SelectCheckBox(Variable<StringANSI>::STRING_VARIABLE_CLASS, true);
				}
				else if(type == Variable<Vector2>::VECTOR_2_TYPE)
				{
					SelectCheckBox(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS, true);
				}
				else if(type == Variable<Vector3>::VECTOR_3_TYPE)
				{
					SelectCheckBox(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS, true);
				}
				else if(type == Variable<Quaternion>::QUATERNION_TYPE)
				{
					SelectCheckBox(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS, true);
				}
			}
		}
		else 
		{ 
			if(suffix == PlayerControl::_GetFileSuffix())
			{
				SelectCheckBox(PlayerControl::PLAYER_CONTROL_CLASS, true);
			}
			else if(suffix == Timer::_GetFileSuffix())
			{
				SelectCheckBox(Timer::TIMER_CLASS, true);
			}
			else if(suffix == Trigger::_GetFileSuffix())
			{
				SelectCheckBox(Trigger::TRIGGER_CLASS, true);
			}
			else if(suffix == AbstractAction::_GetFileSuffix())
			{
				SelectCheckBox(AbstractAction::ACTION_CLASS, true);
			}
			else if(suffix == AbstractEvent::_GetFileSuffix())
			{
				SelectCheckBox(AbstractEvent::EVENT_CLASS, true);
			}
			else if(suffix == TexturedAnimation::_GetFileSuffix())
			{
				SelectCheckBox(TexturedAnimation::TEXTURED_ANIMATION_CLASS, true);
			}
			else if(suffix == Atlas2D::_GetFileSuffix())
			{
				SelectCheckBox(Atlas2D::ATLAS_2D_CLASS, true);
			}
			else if(suffix == Texture2D::_GetFileSuffix())
			{
				SelectCheckBox(Texture2D::TEXTURE_2D_CLASS, true);
			}
			else if(suffix == Mesh::_GetFileSuffix())
			{
				SelectCheckBox(Mesh::MESH_CLASS, true);
			}
			else if(suffix == Hitbox::_GetFileSuffix())
			{
				SelectCheckBox(Hitbox::HITBOX_CLASS, true);
			}
			else if(suffix == Camera3D::_GetFileSuffix())
			{
				SelectCheckBox(Camera3D::CAMERA_3D_CLASS, true);
			}
			else if(suffix == FloatLerp::_GetFileSuffix())
			{
				SelectCheckBox(FloatLerp::FLOAT_LERP_CLASS, true);
			}
			else if(suffix == QuaternionLerp::_GetFileSuffix())
			{
				SelectCheckBox(QuaternionLerp::QUATERNION_LERP_CLASS, true);
			}
			else if(suffix == VectorLerp::_GetFileSuffix())
			{
				SelectCheckBox(VectorLerp::VECTOR_LERP_CLASS, true);
			}
		}
	}

	okButton->hide();
	cancelButton->move(cancelButton->x()+100, cancelButton->y());
	helpButton->move(helpButton->x()+100, helpButton->y());

	setWindowTitle("Object Review");
}




void ObjectCreator::ReviewObject(AbstractObject* _object)
{ 		
	if(_object)
	{
		nameLineEdit->setText(_object->GetName().c_str());
		SetNameDisable(false);

		if(_object->GetClassProperties() & TexturingSurface::TEXTURING_SURFACE_CLASS)
		{
			SelectCheckBox(TexturingSurface::TEXTURING_SURFACE_CLASS, true);
		}
		if(_object->GetClassProperties() & GraphicsSurface::GRAPHICS_SURFACE_CLASS)
		{
			SelectCheckBox(GraphicsSurface::GRAPHICS_SURFACE_CLASS, true);
		}

		if(_object->GetClassProperties() & Plane::PLANE_CLASS)
		{
			SelectCheckBox(Plane::PLANE_CLASS, true);
		}
		if(_object->GetClassProperties() & GraphicsStrip::GRAPHICS_STRIP_CLASS)
		{
			SelectCheckBox(GraphicsStrip::GRAPHICS_STRIP_CLASS, true);
		}
		if(_object->GetClassProperties() & Timer::TIMER_CLASS)
		{  
			SelectCheckBox(Timer::TIMER_CLASS, true);
		}
		if(_object->GetClassProperties() & AbstractObject::ABSTRACT_OBJECT_CLASS)
		{
			SelectCheckBox(AbstractObject::ABSTRACT_OBJECT_CLASS, true);
		}  

		if(!(_object->GetClassProperties() & Scene::SCENE_CLASS))
		{
			if(_object->GetClassProperties() & LogicScene::LOGIC_SCENE_CLASS)
			{
				SelectCheckBox(LogicScene::LOGIC_SCENE_CLASS, true);
			}
			else if(_object->GetClassProperties() & GraphicsScene::GRAPHICS_SCENE_CLASS)
			{
				SelectCheckBox(GraphicsScene::GRAPHICS_SCENE_CLASS, true);
			}
			else if(_object->GetClassProperties() & TiledSpace3D::TILED_SPACE_3D_CLASS)
			{
				SelectCheckBox(TiledSpace3D::TILED_SPACE_3D_CLASS, true);
			}
			else if(_object->GetClassProperties() & ObjectScene::OBJECT_SCENE_CLASS)
			{
				SelectCheckBox(ObjectScene::OBJECT_SCENE_CLASS, true);
			}
		}
		else
		{
			SelectCheckBox(Scene::SCENE_CLASS, true);
		}

		if(_object->GetClassProperties() & List<int32>::INTEGER_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(Variable<int32>::INTEGER_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<float>::FLOAT_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(Variable<float>::FLOAT_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<bool>::BOOLEAN_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(Variable<bool>::BOOLEAN_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<StringANSI>::STRING_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(Variable<StringANSI>::STRING_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<Vector2>::VECTOR_2_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<Vector3>::VECTOR_3_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<Quaternion>::QUATERNION_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<AbstractObject>::OBJECT_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<TranslatableObject>::TRANSLATABLE_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(TranslatableObject::TRANSLATABLE_OBJECT_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<ScalableObject>::SCALABLE_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(ScalableObject::SCALABLE_OBJECT_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<RotatableObject>::ROTATABLE_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(RotatableObject::ROTATABLE_OBJECT_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<ColorableObject>::COLORABLE_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(ColorableObject::COLORABLE_OBJECT_CLASS, true);
		}
		else if(_object->GetClassProperties() & List<Set>::SET_LIST_CLASS)
		{
			SelectCheckBox(AbstractList::ABSTRACT_LIST_CLASS, true);
			SelectCheckBox(Set::SET_CLASS, true);
		}

		if(_object->GetClassProperties() & Variable<int32>::INTEGER_VARIABLE_CLASS)
		{
			SelectCheckBox(Variable<int32>::INTEGER_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & Variable<float>::FLOAT_VARIABLE_CLASS)
		{
			SelectCheckBox(Variable<float>::FLOAT_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & Variable<bool>::BOOLEAN_VARIABLE_CLASS)
		{
			SelectCheckBox(Variable<bool>::BOOLEAN_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & Variable<StringANSI>::STRING_VARIABLE_CLASS)
		{
			SelectCheckBox(Variable<StringANSI>::STRING_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & Variable<Vector2>::VECTOR_2_VARIABLE_CLASS)
		{
			SelectCheckBox(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)
		{
			SelectCheckBox(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS, true);
		}
		else if(_object->GetClassProperties() & Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)
		{
			SelectCheckBox(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS, true);
		}

		if(_object->GetClassProperties() & PlayerControl::PLAYER_CONTROL_CLASS)
		{
			SelectCheckBox(PlayerControl::PLAYER_CONTROL_CLASS, true);
		}
			
		if(_object->GetClassProperties() & Trigger::TRIGGER_CLASS)
		{
			SelectCheckBox(Trigger::TRIGGER_CLASS, true);
		} 
			
		if(_object->GetClassProperties() & AbstractAction::ACTION_CLASS)
		{
			SelectCheckBox(AbstractAction::ACTION_CLASS, true);
		}
			
		if(_object->GetClassProperties() & AbstractEvent::EVENT_CLASS)
		{
			SelectCheckBox(AbstractEvent::EVENT_CLASS, true);
		}
			
		if(_object->GetClassProperties() & TexturedAnimation::TEXTURED_ANIMATION_CLASS)
		{
			SelectCheckBox(TexturedAnimation::TEXTURED_ANIMATION_CLASS, true);
		}
		
		if(_object->GetClassProperties() & Atlas2D::ATLAS_2D_CLASS)
		{
			SelectCheckBox(Atlas2D::ATLAS_2D_CLASS, true);
		}
			
		if(_object->GetClassProperties() & Texture2D::TEXTURE_2D_CLASS)
		{
			SelectCheckBox(Texture2D::TEXTURE_2D_CLASS, true);
		}
			
		if(_object->GetClassProperties() & Mesh::MESH_CLASS)
		{
			SelectCheckBox(Mesh::MESH_CLASS, true);
		}
		
		if(_object->GetClassProperties() & Hitbox::HITBOX_CLASS)
		{
			SelectCheckBox(Hitbox::HITBOX_CLASS, true);
		}

		if(_object->GetClassProperties() & Camera3D::CAMERA_3D_CLASS)
		{
			SelectCheckBox(Camera3D::CAMERA_3D_CLASS, true);
		}
			
		if(_object->GetClassProperties() & FloatLerp::FLOAT_LERP_CLASS)
		{
			SelectCheckBox(FloatLerp::FLOAT_LERP_CLASS, true);
		}
			
		if(_object->GetClassProperties() & QuaternionLerp::QUATERNION_LERP_CLASS)
		{
			SelectCheckBox(QuaternionLerp::QUATERNION_LERP_CLASS, true);
		}
			
		if(_object->GetClassProperties() & VectorLerp::VECTOR_LERP_CLASS)
		{
			SelectCheckBox(VectorLerp::VECTOR_LERP_CLASS, true);
		}
	}

	okButton->hide();
	cancelButton->move(cancelButton->x()+100, cancelButton->y());
	helpButton->move(helpButton->x()+100, helpButton->y());

	setWindowTitle("Object Review");
}



void ObjectCreator::AbstractObjectChecked(int _enable)
{
	UpdateSupportedTriggerList();
}


void ObjectCreator::GraphicsObjectChecked(int _enable)
{
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, colorableObjectCheckBox, selectedProperties, true);
    UpdateCheckbox(_enable, translatableObjectCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, scalableObjectCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, rotatableObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::ColorableObjectChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, ColorableObject::COLORABLE_OBJECT_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::TranslatableObjectChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, TranslatableObject::TRANSLATABLE_OBJECT_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::ScalableObjectChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, ScalableObject::SCALABLE_OBJECT_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::RotatableObjectChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, RotatableObject::ROTATABLE_OBJECT_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::LogicObjectChecked(int _enable)
{
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::PlayingObjectChecked(int _enable)
{
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::LerpingObjectChecked(int _enable)
{
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::ActivatableObjectChecked(int _enable)
{  
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::TriggerChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Trigger::TRIGGER_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, variableBoolCheckBox, selectedProperties);
	UpdateCheckbox(_enable, activatableObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::ActionChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, AbstractAction::ACTION_CLASS);
 
	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::MeshChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Mesh::MESH_CLASS);
 
	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::HitboxChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Hitbox::HITBOX_CLASS);
 
	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::EventChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, AbstractEvent::EVENT_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
    
	UpdateCheckbox(_enable, variableBoolCheckBox, selectedProperties);
	UpdateCheckbox(_enable, activatableObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::VariableQuaternionChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Variable<Quaternion>::QUATERNION_VARIABLE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractVariableCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, rotatableObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::VariableIntChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Variable<int32>::INTEGER_VARIABLE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractVariableCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::VariableFloatChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Variable<float>::FLOAT_VARIABLE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractVariableCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::VariableBoolChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Variable<bool>::BOOLEAN_VARIABLE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractVariableCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::VariableStringChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Variable<StringANSI>::STRING_VARIABLE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractVariableCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::VariableVector2Checked(int _enable)
{
	DisableIncompatibleProperties(_enable, Variable<Vector2>::VECTOR_2_VARIABLE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractVariableCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::VariableVector3Checked(int _enable)
{
	DisableIncompatibleProperties(_enable, Variable<Vector3>::VECTOR_3_VARIABLE_CLASS);
 
	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractVariableCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, translatableObjectCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, scalableObjectCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, colorableObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::AbstractVariableChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, AbstractVariable::ABSTRACT_VARIABLE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::PlayerControlChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, PlayerControl::PLAYER_CONTROL_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::QuaternionLerpChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, QuaternionLerp::QUATERNION_LERP_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, lerpingObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::VectorLerpChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, VectorLerp::VECTOR_LERP_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, lerpingObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::FloatLerpChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, FloatLerp::FLOAT_LERP_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, logicObjectCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, playingObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}




void ObjectCreator::CameraChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Camera3D::CAMERA_3D_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, translatableObjectCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, rotatableObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::ObjectSceneChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, ObjectScene::OBJECT_SCENE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::LogicSceneChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, LogicScene::LOGIC_SCENE_CLASS);
 
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, objectSceneCheckBox, selectedProperties);
	UpdateCheckbox(_enable, logicObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
} 


void ObjectCreator::GraphicsSceneChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, GraphicsScene::GRAPHICS_SCENE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, graphicsContainerCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::TiledSpace3DChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, TiledSpace3D::TILED_SPACE_3D_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, graphicsContainerCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::GraphicsContainerChecked(int _enable)
{
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, objectSceneCheckBox, selectedProperties);
	UpdateCheckbox(_enable, graphicsObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);

 
	UpdateSupportedTriggerList();
}


void ObjectCreator::SceneChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Scene::SCENE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, logicSceneCheckBox, selectedProperties);
	UpdateCheckbox(_enable, graphicsSceneCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::AbstractListChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, AbstractList::ABSTRACT_LIST_CLASS); 

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
} 



void ObjectCreator::TimerChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Timer::TIMER_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, playingObjectCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, logicObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::TexturingSurfaceChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, TexturingSurface::TEXTURING_SURFACE_CLASS);
  
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, graphicsSurfaceCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}
 



void ObjectCreator::GraphicsSurfaceChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, GraphicsSurface::GRAPHICS_SURFACE_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, polygonalSurfaceCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, graphicsObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}





void ObjectCreator::AbstractSurfaceChecked(int _enable)
{
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::PlaneChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Plane::PLANE_CLASS);
  
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractSurfaceCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, translatableObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


 
void ObjectCreator::SetChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Set::SET_CLASS);
  
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::GraphicsStripChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, GraphicsStrip::GRAPHICS_STRIP_CLASS);
  
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, graphicsObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}


void ObjectCreator::PolygonalSurfaceChecked(int _enable)
{
	QList<int64> selectedProperties = GetActiveSelectedProperties();
	UpdateCheckbox(_enable, abstractSurfaceCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
} 



void ObjectCreator::AbstractAnimationChecked(int _enable)
{ 
	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, playingObjectCheckBox, selectedProperties, true);
	UpdateCheckbox(_enable, logicObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);

 
	UpdateSupportedTriggerList();
}




void ObjectCreator::TexturedAnimationChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, TexturedAnimation::TEXTURED_ANIMATION_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractAnimationCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);

 
	UpdateSupportedTriggerList();
}



void ObjectCreator::TextureChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Texture2D::TEXTURE_2D_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties, true);
	EnableCompatibleProperties(selectedProperties);

 
	UpdateSupportedTriggerList();
}



void ObjectCreator::AtlasChecked(int _enable)
{
	DisableIncompatibleProperties(_enable, Atlas2D::ATLAS_2D_CLASS);

	QList<int64> selectedProperties = GetActiveSelectedProperties();
   
	UpdateCheckbox(_enable, abstractObjectCheckBox, selectedProperties);
	EnableCompatibleProperties(selectedProperties);


	UpdateSupportedTriggerList();
}



void ObjectCreator::UpdateSupportedTriggerList(void)
{ 
	supportedEventsList->clear();
	supportedActionsList->clear();


	QList<int64> properties = GetActiveAndPassiveSelectedProperties();

	if(!properties.empty())
	{
		for(int i = 0; i < properties.size(); ++i)
		{
			if(properties.at(i) == AbstractObject::ABSTRACT_OBJECT_CLASS)
			{
				supportedEventsList->AddItem("SEND_MESSAGE");
				supportedEventsList->AddItem("CHANGE_NAME");
				supportedEventsList->AddItem("DESTROY_OBJECT");
				supportedEventsList->AddItem("CREATE_OBJECT");
				supportedEventsList->AddItem("ENABLE_PROPERTY");
				supportedActionsList->AddItem("SEND_MESSAGE");
				supportedActionsList->AddItem("CHANGE_NAME");
				supportedActionsList->AddItem("GET_NAME");
				supportedActionsList->AddItem("DESTROY_OBJECT");
				supportedActionsList->AddItem("CHECK_OBJECT_EXIST");
				supportedActionsList->AddItem("SAVE_TO_FILE");
				supportedActionsList->AddItem("SET_PRIVATE_ASSET");
				supportedActionsList->AddItem("CHECK_PROPERTY");
				supportedActionsList->AddItem("ENABLE_PROPERTY");
			}
			else if(properties.at(i) == PlayingObject::PLAYING_OBJECT_CLASS) 
			{
				supportedEventsList->AddItem("CHANGE_PLAYING_MODE");
				supportedEventsList->AddItem("CHANGE_PLAYING_STATE");
				supportedActionsList->AddItem("CHANGE_PLAYING_STATE");
				supportedActionsList->AddItem("REPLAY");

				if(properties.indexOf(FloatLerp::FLOAT_LERP_CLASS) != -1)
				{
					supportedActionsList->AddItem("CHANGE_PLAYING_MODE");

					TriggerListWidgetItem* it1 = supportedActionsList->AddItem("CHANGE_PLAYING_DURATION", true);
					
					if(it1)
					{
						QString mode1 = "<li>UNITS_PER_SECOND: <font color = \"#00b200\"><b>yes</b></font></li>";
						QString mode2 = "<li>FIXED_TIME: <font color = \"#00b200\"><b>yes</b></font></li>";
						QString mode3 = "<li>MAGNETIC: <font color = \"#ff0000\"><b>no</b></font></li>";
						QString mode4 = "<li>PARABOLA: <font color = \"#ff0000\"><b>no</b></font></li>";
						QString mode5 = "<li>DAMPING: <font color = \"#ff0000\"><b>no</b></font></li>";

						it1->setToolTip(mode1 + mode2 + mode3 + mode4 + mode5);
					}
				}
				else if(properties.indexOf(Timer::TIMER_CLASS) != -1)
				{
					TriggerListWidgetItem* it1 = supportedActionsList->AddItem("CHANGE_PLAYING_MODE", true);
					
					if(it1)
					{
						QString mode1 = "<li>FORWARD_PLAYING: <font color = \"#00b200\"><b>yes</b></font></li>";
						QString mode2 = "<li>BACKWARD_PLAYING: <font color = \"#ff0000\"><b>no</b></font></li>";
						QString mode3 = "<li>PING_PONG_PLAYING: <font color = \"#ff0000\"><b>no</b></font></li>";
						QString mode4 = "<li>ONEWAY_PLAYING: <font color = \"#00b200\"><b>yes</b></font></li>";
						QString mode5 = "<li>LOOP_PLAYING: <font color = \"#00b200\"><b>yes</b></font></li>";
						QString mode6 = "<li>SINGLE_SHOT_PLAYING: <font color = \"#00b200\"><b>yes</b></font></li>";
						
						it1->setToolTip(mode1 + mode2 + mode3 + mode4 + mode5 + mode6);
					}

					supportedActionsList->AddItem("CHANGE_PLAYING_DURATION");
				}
				else
				{  
					supportedActionsList->AddItem("CHANGE_PLAYING_MODE");
					supportedActionsList->AddItem("CHANGE_PLAYING_DURATION");
				}
			} 
			else if(properties.at(i) == Timer::TIMER_CLASS)
			{
				supportedEventsList->AddItem("TIMER_FINISH");
			} 
			else if(properties.at(i) == FloatLerp::FLOAT_LERP_CLASS)
			{
				supportedEventsList->AddItem("FLOAT_LERP_FINISH");
				supportedEventsList->AddItem("FLOAT_LERP_START");
				supportedActionsList->AddItem("CHANGE_FLOAT_LERP_TARGET");
				supportedActionsList->AddItem("CHANGE_FLOAT_LERP_SPEED");

				TriggerListWidgetItem* it1 = supportedActionsList->AddItem("CHANGE_FLOAT_LERP_T", true);
					
				if(it1)
				{
					QString mode1 = "<li>UNITS_PER_SECOND: <font color = \"#00b200\"><b>yes</b></font></li>";
					QString mode2 = "<li>FIXED_TIME: <font color = \"#00b200\"><b>yes</b></font></li>";
					QString mode3 = "<li>MAGNETIC: <font color = \"#ff0000\"><b>no</b></font></li>";
					QString mode4 = "<li>PARABOLA: <font color = \"#ff0000\"><b>no</b></font></li>";
					QString mode5 = "<li>DAMPING: <font color = \"#ff0000\"><b>no</b></font></li>";

					it1->setToolTip(mode1 + mode2 + mode3 + mode4 + mode5);
				}

				supportedActionsList->AddItem("GET_FLOAT_LERP_T");
				supportedActionsList->AddItem("GET_FLOAT_LERP_CURRENT");
				supportedActionsList->AddItem("GET_FLOAT_LERP_DELTA");
				supportedActionsList->AddItem("STREAMING_FLOAT_CURRENT");
				supportedActionsList->AddItem("STREAMING_FLOAT_DELTA");
				supportedActionsList->AddItem("STREAMING_FLOAT_T");
				supportedActionsList->AddItem("CHANGE_FLOAT_LERP_MODE");
			}
			else if(properties.at(i) == VectorLerp::VECTOR_LERP_CLASS)
			{
				supportedActionsList->AddItem("SET_TWO_VECTORS");
				supportedActionsList->AddItem("CHANGE_CONTROL_VECTORS");
				supportedActionsList->AddItem("GET_CONTROL_VECTORS");
				supportedActionsList->AddItem("STREAMING_VECTOR_CURRENT");
				supportedActionsList->AddItem("STREAMING_VECTOR_DELTA");
				supportedActionsList->AddItem("CHANGE_VECTOR_LERP_MODE");
				supportedActionsList->AddItem("GET_DATA_FROM_CONTROL_VECTOR");
				supportedActionsList->AddItem("SET_DATA_TO_CONTROL_VECTOR");
				supportedActionsList->AddItem("GET_VECTOR_LERP_PATH");
				supportedActionsList->AddItem("GET_VECTOR_LERP_T");
			}
			else if(properties.at(i) == QuaternionLerp::QUATERNION_LERP_CLASS)
			{
				supportedActionsList->AddItem("SET_TWO_QUATERNIONS");
				supportedActionsList->AddItem("CHANGE_CONTROL_QUATERNIONS");
				supportedActionsList->AddItem("GET_CONTROL_QUATERNIONS");
				supportedActionsList->AddItem("STREAMING_QUATERNION_CURRENT");
				supportedActionsList->AddItem("STREAMING_QUATERNION_DELTA");
				supportedActionsList->AddItem("GET_DATA_FROM_CONTROL_QUATERNION");
				supportedActionsList->AddItem("SET_DATA_TO_CONTROL_QUATERNION");
				supportedActionsList->AddItem("GET_QUATERNION_LERP_PATH");
				supportedActionsList->AddItem("GET_QUATERNION_LERP_T");
			}
			else if(properties.at(i) == AbstractAnimation::ABSTRACT_ANIMATION_CLASS)
			{
				supportedEventsList->AddItem("CHANGE_ANIMATION_FRAME");
				supportedActionsList->AddItem("GET_AMOUNT_OF_FRAMES");
				supportedActionsList->AddItem("CHANGE_ANIMATION_FRAME");
			}
			else if(properties.at(i) == ActivatableObject::ACTIVATABLE_OBJECT_CLASS)
			{
				supportedActionsList->AddItem("CHANGE_ACTIVATION_LIMIT");
				supportedActionsList->AddItem("CHANGE_DEACTIVATION_MODE");
			}
			else if(properties.at(i) == GraphicsObject::GRAPHICS_OBJECT_CLASS)
			{
				supportedActionsList->AddItem("ENABLE_SHOW");
			}
			else if(properties.at(i) == LogicObject::LOGIC_OBJECT_CLASS)
			{
				supportedActionsList->AddItem("ENABLE_FREEZE");
				supportedActionsList->AddItem("CHANGE_DELTA_SCALE");
			}
			else if(properties.at(i) == TexturedAnimation::TEXTURED_ANIMATION_CLASS)
			{
				supportedEventsList->AddItem("CHANGE_ATLAS");
				supportedEventsList->AddItem("ENABLE_POSITION_OFFSET");
				supportedActionsList->AddItem("CHANGE_ATLAS");
				supportedActionsList->AddItem("GET_FRAME_SIZE");
				supportedActionsList->AddItem("ENABLE_POSITION_OFFSET");
			}
			else if(properties.at(i) == Texture2D::TEXTURE_2D_CLASS)
			{
				supportedActionsList->AddItem("GET_TEXTURE_SIZE");
				supportedActionsList->AddItem("CREATE_BOX_TEXTURE_MAP");
				supportedActionsList->AddItem("CHANGE_BOX_TEXTURE_MAP");
			}
			else if(properties.at(i) == AbstractVariable::ABSTRACT_VARIABLE_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedEventsList->AddItem("CHANGE_VALUE");
					supportedActionsList->AddItem("CHANGE_VALUE");
				}
			}
			else if(properties.at(i) == Variable<float>::FLOAT_VARIABLE_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("GET_RANDOM_VALUE");
					supportedActionsList->AddItem("CHANGE_VALUE_RANGE");
				}
			}
			else if(properties.at(i) == Variable<int32>::INTEGER_VARIABLE_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("GET_RANDOM_VALUE");
					supportedActionsList->AddItem("CHANGE_VALUE_RANGE");
				}
			}
			else if(properties.at(i) == Variable<bool>::BOOLEAN_VARIABLE_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("GET_RANDOM_VALUE");
				}
			}
			else if(properties.at(i) == Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("CHANGE_VECTOR_COMPONENT");
					supportedActionsList->AddItem("GET_VECTOR_COMPONENT");
					supportedActionsList->AddItem("GET_NORMALIZED_VECTOR");
					supportedActionsList->AddItem("GET_VECTOR_LENGTH");
					supportedActionsList->AddItem("GET_VECTOR_ANGLE");
					supportedActionsList->AddItem("VECTOR_TO_SPHERICAL");
				}
			}
			else if(properties.at(i) == Variable<Vector2>::VECTOR_2_VARIABLE_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("CHANGE_VECTOR_COMPONENT");
					supportedActionsList->AddItem("GET_VECTOR_COMPONENT");
					supportedActionsList->AddItem("GET_NORMALIZED_VECTOR");
					supportedActionsList->AddItem("GET_VECTOR_LENGTH");
					supportedActionsList->AddItem("GET_VECTOR_ANGLE");
					supportedActionsList->AddItem("VECTOR_TO_SPHERICAL");
				}
			}
			else if(properties.at(i) == AbstractList::ABSTRACT_LIST_CLASS)
			{
				supportedActionsList->AddItem("RANDOMIZE_LIST");
				supportedActionsList->AddItem("GET_LIST_LENGTH");
				supportedActionsList->AddItem("REMOVE_FROM_LIST");
				supportedActionsList->AddItem("GET_AMOUNT_OF_ITEMS");
				supportedActionsList->AddItem("CLEAR_LIST");
				supportedActionsList->AddItem("CHECK_LIST_EMPTY");
				supportedActionsList->AddItem("ROTATE_LIST");
				supportedActionsList->AddItem("INVERSE_LIST");
				supportedActionsList->AddItem("GET_ITEM_INDEX");
				supportedActionsList->AddItem("SWAP_ITEMS");

				TriggerListWidgetItem* it1 = supportedActionsList->AddItem("ADD_TO_LIST", true);
				
				if(it1)
				{	
					if(properties.indexOf(Variable<int32>::INTEGER_VARIABLE_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListIntegerInfo());
					}
					else if(properties.indexOf(Variable<float>::FLOAT_VARIABLE_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListFloatInfo());
					}
					else if(properties.indexOf(Variable<bool>::BOOLEAN_VARIABLE_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListBooleanInfo());
					}
					else if(properties.indexOf(Variable<StringANSI>::STRING_VARIABLE_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListStringInfo());
					}
					else if(properties.indexOf(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListVector2Info());
					}
					else if(properties.indexOf(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListVector3Info());
					}
					else if(properties.indexOf(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListQuaternionInfo());
					}
					else if(properties.indexOf(TranslatableObject::TRANSLATABLE_OBJECT_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListTranslatableObjectInfo());
					}
					else if(properties.indexOf(ScalableObject::SCALABLE_OBJECT_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListScalableObjectInfo());
					}
					else if(properties.indexOf(RotatableObject::ROTATABLE_OBJECT_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListRotatableObjectInfo());
					}
					else if(properties.indexOf(ColorableObject::COLORABLE_OBJECT_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListColorableObjectInfo());
					}
					else if(properties.indexOf(Set::SET_CLASS) != -1)
					{ 
						it1->setToolTip(GetAddToListSetInfo());
					}
					else
					{
						it1->setToolTip(GetAddToListAbstractObjectInfo());
					}
				}

				TriggerListWidgetItem* it2 = supportedActionsList->AddItem("COPY_LIST", true);
				
				if(it2)
				{
					it2->setToolTip(GetCopyToListInfo());
				}

				TriggerListWidgetItem* it3 = supportedActionsList->AddItem("GET_DATA_FROM_ITEM", true);
				
				if(it3)
				{	
					it3->setToolTip(GetDataFromListInfo());
				}

				TriggerListWidgetItem* it4 = supportedActionsList->AddItem("CHANGE_LIST_VALUE", true);
				
				if(it4)
				{	
					it4->setToolTip(GetChangeListValueInfo());
				}

				TriggerListWidgetItem* it5 = supportedActionsList->AddItem("CHANGE_ITEM_VALUE", true);
				
				if(it5)
				{	
					it5->setToolTip(GetChangeItemValueInfo());
				}
			}
			else if(properties.at(i) == AbstractSurface::ABSTRACT_SURFACE_CLASS)
			{
				supportedActionsList->AddItem("CHECK_LINE_IN_SURFACE");
				supportedActionsList->AddItem("CHANGE_INTERSECTION_MODE");
			} 
			else if(properties.at(i) == PlayerControl::PLAYER_CONTROL_CLASS)
			{
				supportedEventsList->AddItem("PLAYER_CONTROL_FINISH");
				supportedActionsList->AddItem("ADD_INPUT_EVENT");
				supportedActionsList->AddItem("SET_ONE_INPUT_EVENT");
				supportedActionsList->AddItem("CLEAR_INPUT_EVENTS");
				supportedActionsList->AddItem("CHANGE_INPUT_TIMING");
			} 
			else if(properties.at(i) == Trigger::TRIGGER_CLASS)
			{
				supportedEventsList->AddItem("TRIGGER_FINISH");
			} 
			else if(properties.at(i) == Camera3D::CAMERA_3D_CLASS)
			{
				supportedEventsList->AddItem("PICK_CAMERA");
				supportedActionsList->AddItem("PICK_CAMERA");
				supportedActionsList->AddItem("CHANGE_PROJECTION_MODE");
				supportedActionsList->AddItem("CHANGE_MOVEMENT_MODE");
				supportedActionsList->AddItem("CHANGE_ROTATION_MODE");
				supportedActionsList->AddItem("GET_SELECT_RAY");
				supportedActionsList->AddItem("CHANGE_VIEWING_POINT");
				supportedActionsList->AddItem("GET_VIEWING_POINT");
				supportedActionsList->AddItem("CHANGE_DISTANCE_TO_VIEWING_POINT");
				supportedActionsList->AddItem("CAMERA_LOOK_AT");
				supportedActionsList->AddItem("CHECK_FRUSTUM_IN_QUAD");
				supportedActionsList->AddItem("CHANGE_ZOOM_FAR");
				supportedActionsList->AddItem("CHANGE_ZOOM_NEAR");
				supportedActionsList->AddItem("CHANGE_ZOOM_SPEED");
				supportedActionsList->AddItem("ENABLE_ZOOM");
				supportedActionsList->AddItem("GET_CAMERA_LOOK");
				supportedActionsList->AddItem("ENABLE_ROTATION");
				supportedActionsList->AddItem("ENABLE_ROTATION_RANGE");
				supportedActionsList->AddItem("CHANGE_ROTATION_RANGE");
			}
			else if(properties.at(i) == LerpingObject::LERPING_OBJECT_CLASS)
			{
				supportedEventsList->AddItem("CHANGE_FLOAT_LERP");
				supportedActionsList->AddItem("CHANGE_FLOAT_LERP");
			}
			else if(properties.at(i) == Plane::PLANE_CLASS)
			{
				supportedActionsList->AddItem("CHANGE_PLANE_NORMAL");
			} 
			else if(properties.at(i) == GraphicsSurface::GRAPHICS_SURFACE_CLASS)
			{
				supportedActionsList->AddItem("ENABLE_BACK_FACE_CULLING");
				supportedActionsList->AddItem("ENABLE_STATIC_OBJECT");
			}
			else if(properties.at(i) == TexturingSurface::TEXTURING_SURFACE_CLASS)
			{
				supportedEventsList->AddItem("CHANGE_FRONT_TEXTURE");
				supportedEventsList->AddItem("CHANGE_BACK_TEXTURE");
				supportedEventsList->AddItem("CHANGE_FRONT_TEXTURED_ANIMATION");
				supportedEventsList->AddItem("CHANGE_BACK_TEXTURED_ANIMATION");
				supportedActionsList->AddItem("CHANGE_FRONT_TEXTURE");
				supportedActionsList->AddItem("CHANGE_BACK_TEXTURE");
				supportedActionsList->AddItem("CHANGE_FRONT_TEXTURED_ANIMATION");
				supportedActionsList->AddItem("CHANGE_BACK_TEXTURED_ANIMATION");
				supportedActionsList->AddItem("CHECK_LINE_IN_IMAGE");
				supportedActionsList->AddItem("ENABLE_TEXTURE_FLIP");
				supportedActionsList->AddItem("SET_PUBLIC_TEXTURE");
			}
			else if(properties.at(i) == ObjectScene::OBJECT_SCENE_CLASS)
			{
			    supportedEventsList->AddItem("ADD_TO_LAYER");
				supportedEventsList->AddItem("REMOVE_FROM_LAYER");
				supportedActionsList->AddItem("ADD_TO_LAYER");
				supportedActionsList->AddItem("REMOVE_FROM_LAYER");
			}
			else if(properties.at(i) == LogicScene::LOGIC_SCENE_CLASS)
			{
				supportedEventsList->AddItem("ADD_TO_UPDATE_LIST");
				supportedEventsList->AddItem("REMOVE_FROM_UPDATE_LIST");
				supportedActionsList->AddItem("ADD_TO_UPDATE_LIST");
				supportedActionsList->AddItem("REMOVE_FROM_UPDATE_LIST");
			}
			else if(properties.at(i) == GraphicsContainer::GRAPHICS_CONTAINER_CLASS)
			{
				supportedEventsList->AddItem("CHANGE_CAMERA");
				supportedActionsList->AddItem("CHANGE_CAMERA");
				supportedActionsList->AddItem("CHANGE_RENDER_TARGET");
			}
			else if(properties.at(i) == GraphicsScene::GRAPHICS_SCENE_CLASS)
			{
				supportedEventsList->AddItem("ADD_TO_RENDER_LIST");
				supportedEventsList->AddItem("REMOVE_FROM_RENDER_LIST");
				supportedActionsList->AddItem("ADD_TO_RENDER_LIST");
				supportedActionsList->AddItem("REMOVE_FROM_RENDER_LIST");
			} 
			else if(properties.at(i) == TiledSpace3D::TILED_SPACE_3D_CLASS)
			{
				supportedActionsList->AddItem("CHANGE_TILED_SPACE_SIZE");
				supportedActionsList->AddItem("CREATE_BRUSH");
				supportedActionsList->AddItem("REMOVE_BRUSH");
				supportedActionsList->AddItem("GET_TILE_CENTER");
				supportedActionsList->AddItem("TILED_SPACE_FILL");
				supportedActionsList->AddItem("CREATE_SOLID_TILE");
				supportedActionsList->AddItem("CREATE_EMPTY_TILE");
				supportedActionsList->AddItem("SET_PUBLIC_TILE");
				supportedActionsList->AddItem("MOVE_TILE_FROM_TO");
				supportedActionsList->AddItem("COPY_TILE");
				supportedActionsList->AddItem("SWAP_TWO_TILES");
				supportedActionsList->AddItem("CHECK_TILE_EXIST");
				supportedActionsList->AddItem("CHECK_TILE_SOLID");
				supportedActionsList->AddItem("CHECK_TILE_PASS");
				supportedActionsList->AddItem("CHANGE_TILE_PASS");
				supportedActionsList->AddItem("GET_BRUSH_OF_TILE");
			}
			else if(properties.at(i) == Set::SET_CLASS)
			{
				supportedActionsList->AddItem("RESIZE_SET");
				supportedActionsList->AddItem("GET_AMOUNT_OF_SLOTS");
				supportedActionsList->AddItem("GET_DATA_FROM_SLOT");
				supportedActionsList->AddItem("SET_DATA_TO_SLOT");
			}
			else if(properties.at(i) == Scene::SCENE_CLASS)
			{
				supportedEventsList->AddItem("PICK_SCENE");
				supportedActionsList->AddItem("PICK_SCENE");
			} 
			else if(properties.at(i) == GraphicsStrip::GRAPHICS_STRIP_CLASS)
			{
				supportedActionsList->AddItem("CHANGE_STRIP_POINTS"); 
			}
			else if(properties.at(i) == PolygonalSurface::POLYGONAL_SURFACE_CLASS)
			{
				supportedEventsList->AddItem("CHANGE_MESH");
				supportedActionsList->AddItem("CHANGE_HITBOX"); 
				supportedEventsList->AddItem("COLLISION_BETWEEN_OBJECTS");
				supportedActionsList->AddItem("CHANGE_MESH"); 
				supportedActionsList->AddItem("CHECK_LINE_IN_POLYGON"); 
				supportedActionsList->AddItem("CHANGE_HITBOX"); 
				supportedActionsList->AddItem("CHANGE_COLLISION_GROUP"); 
				supportedActionsList->AddItem("ENABLE_COLLISION_CHECKING"); 
				supportedActionsList->AddItem("ENABLE_COLLISION_POLYGONS_INFO"); 
			}
			else if(properties.at(i) == Mesh::MESH_CLASS)
			{
				supportedActionsList->AddItem("CREATE_QUAD");
				supportedActionsList->AddItem("CREATE_BOX");
			}
			else if(properties.at(i) == TranslatableObject::TRANSLATABLE_OBJECT_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("REPLACE_POSITION");
					supportedActionsList->AddItem("DISPLACE_POSITION");
					supportedActionsList->AddItem("GET_POSITION");
					supportedActionsList->AddItem("MOVE_AROUND_PIVOT");
				}
			}
			else if(properties.at(i) == ScalableObject::SCALABLE_OBJECT_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("REPLACE_SCALE");
					supportedActionsList->AddItem("DISPLACE_SCALE");
					supportedActionsList->AddItem("GET_SCALE");
				}
			}
			else if(properties.at(i) == RotatableObject::ROTATABLE_OBJECT_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("REPLACE_ROTATION");
					supportedActionsList->AddItem("DISPLACE_ROTATION");
					supportedActionsList->AddItem("GET_ROTATION");
					supportedActionsList->AddItem("CREATE_ROTATION");
					supportedActionsList->AddItem("CREATE_ROTATION_FROM_TO");
					supportedActionsList->AddItem("ROTATE_AROUND_PIVOT");
				}
			}
			else if(properties.at(i) == ColorableObject::COLORABLE_OBJECT_CLASS)
			{
				if(properties.indexOf(AbstractList::ABSTRACT_LIST_CLASS) == -1)
				{
					supportedActionsList->AddItem("CHANGE_COLOR");
					supportedActionsList->AddItem("GET_COLOR");
					supportedActionsList->AddItem("CHANGE_COLOR_COMPONENT");
					supportedActionsList->AddItem("GET_COLOR_COMPONENT");
				}
			}
		}
	}  
}




void ObjectCreator::UpdateCheckbox(int _enable, QCheckBox* _checkBox, QList<int64> _selectedProperties, bool _freeze)
{
	if(_enable == Qt::Checked)
	{
		_checkBox->setChecked(true);
		_checkBox->setEnabled(false);
	}
	else if(_enable == Qt::Unchecked)
	{
		if(!_selectedProperties.isEmpty())
		{
			int64 type = GetId(_checkBox);
			for(int i = 0; i < _selectedProperties.size(); ++i)
			{
				if(AbstractObject::_IsChild(_selectedProperties.at(i), type))
				{
					return;
				}
			}
		}
		if(_freeze) 
		{
			_checkBox->setChecked(false);
			_checkBox->setEnabled(false);
		}
		else
		{
			_checkBox->setEnabled(true);
		}
	}
}


bool ObjectCreator::IsNameDisabled(void)const
{
	return !nameLineEdit->isEnabled();
}



void ObjectCreator::IsOk(void)
{
	int32 errorCode;
	if(nameLineEdit->text().isEmpty())
	{
		ErrorDialog("Error", "Object name is not defined", 155, 40);
		return;
	}
	if((errorCode = BooleanExprParser::_IsNameValid(nameLineEdit->text().toAscii().data())) >= 0)
	{
		ErrorDialog("Error", "Object name is not valid: '" + QString(nameLineEdit->text().at(errorCode)) + "'", 190, 40);
		return;
	}
	QList<int64> properties = GetActiveSelectedProperties();

	if(properties.empty())
	{
		ErrorDialog("Error", "No properties selected", 150, 50);
		return;
	}
	int64 type = 0;

	for(int i = 0; i < properties.size(); ++i)
	{
		type |= properties.at(i);
	}

	AbstractObject* object = AbstractObject::_CreateObject(AbstractObject::_CreateObjectType(type));

	if(object == NIL)
	{
		if(type & ObjectScene::OBJECT_SCENE_CLASS)
		{  
			if(type & Scene::SCENE_CLASS) 
			{
				object = new Scene();
			}
			else if(type & LogicScene::LOGIC_SCENE_CLASS)
			{
				object = new LogicScene();
			}
			else if(type & GraphicsScene::GRAPHICS_SCENE_CLASS)
			{
				object = new GraphicsScene();
			}
			else if(type & TiledSpace3D::TILED_SPACE_3D_CLASS)
			{
				object = new TiledSpace3D();
			}
			else
			{
				object = new ObjectScene();
			}
		}
		else if(type & Camera3D::CAMERA_3D_CLASS)
		{
			object = new Camera3D(); 
		}
		else if(type & Mesh::MESH_CLASS)
		{
			if(!fastCreatingEnable)
			{ 
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), QString("TextureEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable") + QString(" hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				TextureCreator* dialog = new TextureCreator(this);
				dialog->SetHitboxEditor();
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->SetFastCreating(true);
				
				dialog->SetHitboxObjectName(nameLineEdit->text());

				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		}
		else if(type & Hitbox::HITBOX_CLASS)
		{
			if(!fastCreatingEnable)
			{ 
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), QString("TextureEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable") + QString(" hitbox")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				TextureCreator* dialog = new TextureCreator(this);
				dialog->SetHitboxEditor();
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->SetFastCreating(true);
				
				dialog->SetHitboxObjectName(nameLineEdit->text());

				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		}
		else if(type & VectorLerp::VECTOR_LERP_CLASS)
		{
			if(!fastCreatingEnable)
			{ 
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), QString("TextureEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable") + QString(" vectorLerp")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				TextureCreator* dialog = new TextureCreator(this);
				dialog->SetVectorLerpEditor();
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->SetFastCreating(true);
				
				dialog->SetVectorLerpObjectName(nameLineEdit->text());
 
				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		}
		else if(type & FloatLerp::FLOAT_LERP_CLASS)
		{
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("FloatLerpEditor.exe").toAscii().data(), QString("FloatLerpEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return; 
			}
			else
			{

				FloatLerpCreator* dialog = new FloatLerpCreator(this);
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());

				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		} 
		else if(type & QuaternionLerp::QUATERNION_LERP_CLASS)
		{  
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("QuaternionLerpEditor.exe").toAscii().data(), QString("QuaternionLerpEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				QuaternionLerpCreator* dialog = new QuaternionLerpCreator(this);
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());
	
				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		}
		else if(type & PlayerControl::PLAYER_CONTROL_CLASS)
		{
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("PlayerControlEditor.exe").toAscii().data(), QString("PlayerControlEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{
				
				PlayerControlCreator* dialog = new PlayerControlCreator(this);
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());

				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		} 
		else if(type & TexturedAnimation::TEXTURED_ANIMATION_CLASS)
		{ 
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), QString("TexturedAnimationEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable") + QString(" texturedAnimation")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				TextureCreator* dialog = new TextureCreator(this);
				dialog->SetTexturedAnimationEditor();
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());
			
				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		}
		else if(type & Atlas2D::ATLAS_2D_CLASS)
		{ 
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("TexturedAnimationEditor.exe").toAscii().data(), QString("TexturedAnimationEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable") + QString(" atlas")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				TextureCreator* dialog = new TextureCreator(this);
				dialog->SetAtlasEditor();
				dialog->SetAtlasNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetAtlasEditor();
				
				dialog->SetAtlasObjectName(nameLineEdit->text());
			
				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		} 
		else if(type & Texture2D::TEXTURE_2D_CLASS)
		{ 
			if(!fastCreatingEnable)
			{ 
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("TextureEditor.exe").toAscii().data(), QString("TextureEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable") + QString(" texture")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				TextureCreator* dialog = new TextureCreator(this);
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);

				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());

				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		}
		else if(type & Trigger::TRIGGER_CLASS)
		{ 
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("TriggerEditor.exe").toAscii().data(), QString("TriggerEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				TriggerCreator* dialog = new TriggerCreator(this);
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());
				
				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		}
		else if(type & AbstractEvent::EVENT_CLASS)
		{   
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("EventEditor.exe").toAscii().data(), QString("EventEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				EventCreator* dialog = new EventCreator(this);
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());

				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return; 
			}
		} 
		else if(type & AbstractAction::ACTION_CLASS)
		{
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("ActionEditor.exe").toAscii().data(), QString("ActionEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			} 
			else
			{

				ActionCreator* dialog = new ActionCreator(this);
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());
			
				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return;  
			}
		}
		else if(type & Variable<int32>::INTEGER_VARIABLE_CLASS ||
			    type & Variable<float>::FLOAT_VARIABLE_CLASS ||
				type & Variable<bool>::BOOLEAN_VARIABLE_CLASS ||
				type & Variable<StringANSI>::STRING_VARIABLE_CLASS ||
				type & Variable<Vector2>::VECTOR_2_VARIABLE_CLASS ||
				type & Variable<Vector3>::VECTOR_3_VARIABLE_CLASS ||
				type & Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)
		{
			if(!fastCreatingEnable)
			{
				STARTUPINFOA startInfo;
				ZeroMemory(&startInfo, sizeof(STARTUPINFO));

				PROCESS_INFORMATION procInfo;
				if(CreateProcessA(QString("VariableEditor.exe").toAscii().data(), QString("VariableEditor.exe " +  nameLineEdit->text() + QString(" new") + QString(" nameEnable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
				{
					Editor::_GetProcesses().AddToTail(procInfo.hProcess);
				}
				accept();
				return;
			}
			else
			{

				VariableCreator* dialog = new VariableCreator(this);
				dialog->SetNameDisable(IsNameDisabled());

				dialog->setAttribute(Qt::WA_DeleteOnClose);
				
				dialog->SetFastCreating(true);
				
				dialog->SetObjectName(nameLineEdit->text());

				if(type & Variable<int32>::INTEGER_VARIABLE_CLASS)
				{
					dialog->SetType(Variable<int32>::INTEGER_TYPE);
				}
				else if(type & Variable<float>::FLOAT_VARIABLE_CLASS)
				{
					dialog->SetType(Variable<float>::FLOAT_TYPE);
				}
				else if(type & Variable<bool>::BOOLEAN_VARIABLE_CLASS)
				{
					dialog->SetType(Variable<bool>::BOOLEAN_TYPE);
				}
				else if(type & Variable<StringANSI>::STRING_VARIABLE_CLASS)
				{
					dialog->SetType(Variable<StringANSI>::STRING_TYPE);
				}
				else if(type & Variable<Vector2>::VECTOR_2_VARIABLE_CLASS)
				{
					dialog->SetType(Variable<Vector2>::VECTOR_2_TYPE);
				}
				else if(type & Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)
				{
					dialog->SetType(Variable<Vector3>::VECTOR_3_TYPE);
				}
				else if(type & Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)
				{
					dialog->SetType(Variable<Quaternion>::QUATERNION_TYPE);
				}
			
				if(dialog->exec() == QDialog::Rejected)
				{
					return; 
				}
				accept();
				return;  
			}
		}
	}
	if(object)
	{ 
		object->Rename(nameLineEdit->text().toAscii().data());

		if(!fastCreatingEnable)
		{
			object->SaveToFile();

			delete object;
			object = NIL;
			accept();
			return;
		}
		else
		{
			if(AssetLibrary::_GetAssets().AddObject(object) == NIL)
			{
				ErrorDialog("Error", "Name '" + nameLineEdit->text() + "' is already reserved", 195, 40);
				delete object;
				object = NIL;
				return;
			}
			accept();
			return;
		}
	}
	else { ErrorDialog("Error", "Selected properties are incompatible", 200, 50); } 
}



QString ObjectCreator::GetAddToListAbstractObjectInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>ABSTRACT_OBJECT_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n" 

	"<tr>\n" 
	"<th>SET</th>\n"  
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n" 

	"</table>\n");
}



QString ObjectCreator::GetChangeListValueInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SUPPORTED</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n" 

	"<tr>\n" 
	"<th>SET</th>\n"  
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n" 

	"</table>\n");
}



QString ObjectCreator::GetChangeItemValueInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SUPPORTED</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n" 

	"<tr>\n" 
	"<th>SET</th>\n"  
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n" 

	"</table>\n");
}


QString ObjectCreator::GetAddToListTranslatableObjectInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>TRANSLATABLE_OBJECT_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n" 
	"<th>SET</th>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"  

	"</table>\n");
}



QString ObjectCreator::GetAddToListScalableObjectInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>SCALABLE_OBJECT_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"  
	"</tr>\n" 

	"<tr>\n" 
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n" 

	"</table>\n");
}



QString ObjectCreator::GetAddToListRotatableObjectInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>ROTATABLE_OBJECT_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n" 
	"<th>SET</th>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"</table>\n");
}


QString ObjectCreator::GetAddToListColorableObjectInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>COLORABLE_OBJECT_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"
 
	"<tr>\n"
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"</table>\n");
}
 


QString ObjectCreator::GetAddToListIntegerInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>INTEGER_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"</table>\n");
}  




QString ObjectCreator::GetAddToListSetInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>SET_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n" 
 
	"</table>\n");
}

 
QString ObjectCreator::GetAddToListFloatInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>FLOAT_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n" 

	"</table>\n");
}




QString ObjectCreator::GetAddToListBooleanInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>BOOLEAN_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n"  
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"</table>\n");
}




QString ObjectCreator::GetAddToListStringInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>STRING_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes (NAME)</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes (NAME)</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"</table>\n");
}



QString ObjectCreator::GetAddToListVector2Info(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>VECTOR_2_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"</table>\n");
}


QString ObjectCreator::GetAddToListVector3Info(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>VECTOR_3_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"</table>\n");
}



QString ObjectCreator::GetAddToListQuaternionInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>QUATERNION_LIST</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>SHARE_ITEM</th>\n"
	"<th>COPY_ONLY_DATA</th>\n"
	"<th>COPY_IN_VARIABLE</th>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"    
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"   
	"</tr>\n"

	"</table>\n");
}



QString ObjectCreator::GetCopyToListInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<caption>SHARE_ITEM/COPY_ONLY_DATA/COPY_IN_VARIABLE</caption>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>INTEGER</th>\n"
	"<th>FLOAT</th>\n"
	"<th>BOOLEAN</th>\n"
	"<th>STRING</th>\n"
	"<th>VECTOR_2</th>\n"
	"<th>VECTOR_3</th>\n"
	"<th>QUATERNION</th>\n"

	"<th>ABSTRACT</th>\n"
	"<th>TRANSLATABLE</th>\n"
	"<th>SCALABLE</th>\n"
	"<th>ROTATABLE</th>\n"
	"<th>COLORABLE</th>\n"
	"<th>SET</th>\n"  
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n" 
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font>/<font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font>/<font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"</table>\n");
}



QString ObjectCreator::GetDataFromListInfo(void)
{
	return QString("<table border=1 align=center cellpadding=5>\n"
	
	"<tr>\n"
	"<th></th>\n"
	"<th>INTEGER</th>\n"
	"<th>FLOAT</th>\n"
	"<th>BOOLEAN</th>\n"
	"<th>STRING</th>\n"
	"<th>VECTOR_2</th>\n"
	"<th>VECTOR_3</th>\n"
	"<th>QUATERNION</th>\n"

	"<th>ABSTRACT</th>\n"
	"<th>TRANSLATABLE</th>\n"
	"<th>SCALABLE</th>\n"
	"<th>ROTATABLE</th>\n"
	"<th>COLORABLE</th>\n" 
	"<th>SET</th>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>INTEGER</th>\n"
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>FLOAT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>BOOLEAN</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>STRING</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"


	"<tr>\n"
	"<th>VECTOR_2</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>VECTOR_3</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>QUATERNION</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>ABSTRACT</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>TRANSLATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"  
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>SCALABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"  
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>ROTATABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"  
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n"
	"</tr>\n"

	"<tr>\n"
	"<th>COLORABLE</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n"  
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"</tr>\n"

	"<tr>\n"
	"<th>SET</th>\n"
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#ff0000\">no</font></td>\n" 
	"<td align = center><font color = \"#00b200\">yes</font></td>\n" 
	"</tr>\n"

	"</table>\n");
}

