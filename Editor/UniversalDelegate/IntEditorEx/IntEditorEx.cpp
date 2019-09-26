#include "IntEditorEx.h"
#include <Engine/Core/Parser/List/List.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Shape/Plane/Plane.h>


IntEditorEx::IntEditorEx(QWidget* _parent): QWidget(_parent)
{
	intValue = new IntSpinBox(this);
	intValue->setGeometry(QRect(30, 0, 30+25, 24));
	intValue->setToolTip("value");
	connect(intValue, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	assetTypeComboBox = new QComboBox(this);
	assetTypeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	assetTypeComboBox->setToolTip("value");
	assetTypeComboBox->hide();

	assetTypeComboBox->addItem("ATLAS");
	assetTypeComboBox->addItem("TEXTURE");
	assetTypeComboBox->addItem("TEXTURED_ANIMATION");

	assetTypeComboBox->addItem("VARIABLE");
	assetTypeComboBox->addItem("EVENT");
	assetTypeComboBox->addItem("ACTION");
	assetTypeComboBox->addItem("TRIGGER");

	assetTypeComboBox->addItem("OBJECT");
	assetTypeComboBox->addItem("TIMER");
	assetTypeComboBox->addItem("MESH");
	assetTypeComboBox->addItem("HITBOX");
	assetTypeComboBox->addItem("SCENE");
	assetTypeComboBox->addItem("CAMERA");
	assetTypeComboBox->addItem("PLAYER_CONTROL");
	assetTypeComboBox->addItem("LIST");

	assetTypeComboBox->addItem("FLOAT_LERP");
	assetTypeComboBox->addItem("QUATERNION_LERP");
	assetTypeComboBox->addItem("VECTOR_LERP");
	connect(assetTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickAssetTypeValue(const QString&)));
	playingStateComboBox = new QComboBox(this);
	playingStateComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	playingStateComboBox->setToolTip("value");
	playingStateComboBox->hide();
	playingStateComboBox->addItem("PLAY");
	playingStateComboBox->addItem("PAUSE");
	playingStateComboBox->addItem("STOP");
	connect(playingStateComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickPlayingStateValue(const QString&)));
	inputDeviceComboBox = new QComboBox(this);
	inputDeviceComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	inputDeviceComboBox->setToolTip("value");
	inputDeviceComboBox->hide();
	inputDeviceComboBox->addItem("KEYBOARD_DEVICE");
	inputDeviceComboBox->addItem("MOUSE_DEVICE");
	connect(inputDeviceComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickInputDeviceValue(const QString&)));
	deactivationModeComboBox = new QComboBox(this);
	deactivationModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	deactivationModeComboBox->setToolTip("value");
	deactivationModeComboBox->hide();
	deactivationModeComboBox->addItem("NEVER_SWITCH_OFF");
	deactivationModeComboBox->addItem("AUTO_SWITCH_OFF");
	deactivationModeComboBox->addItem("OVERLAY_SWITCH_OFF");
	connect(deactivationModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickDeactivationModeValue(const QString&)));
	activationLimitComboBox = new QComboBox(this);
	activationLimitComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	activationLimitComboBox->setToolTip("value");
	activationLimitComboBox->hide();
	activationLimitComboBox->addItem("UNLIMITED");
	activationLimitComboBox->addItem("STOPED");
	connect(activationLimitComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickActivationLimitValue(const QString&)));
	playingModeComboBox = new QComboBox(this);
	playingModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	playingModeComboBox->setToolTip("value");
	playingModeComboBox->hide();
	playingModeComboBox->addItem("FORWARD_PLAYING");
	playingModeComboBox->addItem("BACKWARD_PLAYING");
	playingModeComboBox->addItem("PING_PONG_PLAYING");
	playingModeComboBox->addItem("ONEWAY_PLAYING");
	playingModeComboBox->addItem("LOOP_PLAYING");
	playingModeComboBox->addItem("SINGLE_SHOT_PLAYING");
	connect(playingModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickPlayingModeValue(const QString&)));
	variableTypeComboBox = new QComboBox(this);
	variableTypeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	variableTypeComboBox->setToolTip("value");
	variableTypeComboBox->hide();
	variableTypeComboBox->addItem("INTEGER"); 
	variableTypeComboBox->addItem("FLOAT");
	variableTypeComboBox->addItem("BOOLEAN");
	variableTypeComboBox->addItem("STRING");
	variableTypeComboBox->addItem("VECTOR_2");
	variableTypeComboBox->addItem("VECTOR_3");
	variableTypeComboBox->addItem("QUATERNION"); 
	connect(variableTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickVariableTypeValue(const QString&)));
	projectionModeComboBox = new QComboBox(this);
	projectionModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	projectionModeComboBox->setToolTip("value");
	projectionModeComboBox->hide();
	projectionModeComboBox->addItem("PERSPECTIVE_PROJECTION"); 
	projectionModeComboBox->addItem("ORTHOGRAPHIC_PROJECTION");
	projectionModeComboBox->addItem("SCREEN_PROJECTION");
	connect(projectionModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickProjectionModeValue(const QString&)));
	movementModeComboBox = new QComboBox(this);
	movementModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	movementModeComboBox->setToolTip("value");
	movementModeComboBox->hide();
	movementModeComboBox->addItem("FLIGHT_MOVEMENT"); 
	movementModeComboBox->addItem("SPECTATOR_MOVEMENT");
	movementModeComboBox->addItem("OVERLAND_MOVEMENT");
	connect(movementModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickMovementModeValue(const QString&)));
	rotationModeComboBox = new QComboBox(this);
	rotationModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	rotationModeComboBox->setToolTip("value");
	rotationModeComboBox->hide();
	rotationModeComboBox->addItem("FIRST_PERSON_ROTATION"); 
	rotationModeComboBox->addItem("OBJECT_VIEWER_ROTATION");
	connect(rotationModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickRotationModeValue(const QString&)));
	intersectionModeComboBox = new QComboBox(this);
	intersectionModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	intersectionModeComboBox->setToolTip("value");
	intersectionModeComboBox->hide();
	intersectionModeComboBox->addItem("APPLY_TRANSFORMATION"); 
	intersectionModeComboBox->addItem("CHECK_ONLY_FRONT_FACE");
	intersectionModeComboBox->addItem("CHECK_ONLY_COLORED_PIXEL"); 
	connect(intersectionModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickIntersectionModeValue(const QString&)));
	boxFaceComboBox = new QComboBox(this);
	boxFaceComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	boxFaceComboBox->setToolTip("value");
	boxFaceComboBox->hide();
	boxFaceComboBox->addItem("FRONT_FACE"); 
	boxFaceComboBox->addItem("BACK_FACE");
	boxFaceComboBox->addItem("LEFT_FACE"); 
	boxFaceComboBox->addItem("RIGHT_FACE"); 
	boxFaceComboBox->addItem("TOP_FACE");
	boxFaceComboBox->addItem("BOTTOM_FACE"); 
	connect(boxFaceComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickBoxFaceValue(const QString&)));
	planeComboBox = new QComboBox(this);
	planeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	planeComboBox->setToolTip("value");
	planeComboBox->hide();
	planeComboBox->addItem("XY"); 
	planeComboBox->addItem("XZ");
	planeComboBox->addItem("YZ");  
	connect(planeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickPlaneValue(const QString&)));
	listTypeComboBox = new QComboBox(this);
	listTypeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	listTypeComboBox->setToolTip("value");
	listTypeComboBox->hide();
	listTypeComboBox->addItem("INTEGER"); 
	listTypeComboBox->addItem("FLOAT");
	listTypeComboBox->addItem("BOOLEAN");
	listTypeComboBox->addItem("STRING");
	listTypeComboBox->addItem("VECTOR_2");
	listTypeComboBox->addItem("VECTOR_3");
	listTypeComboBox->addItem("QUATERNION");
	listTypeComboBox->addItem("ABSTRACT_OBJECT");  
	listTypeComboBox->addItem("TRANSLATABLE_OBJECT");
	listTypeComboBox->addItem("SCALABLE_OBJECT");
	listTypeComboBox->addItem("ROTATABLE_OBJECT");
	listTypeComboBox->addItem("COLORABLE_OBJECT");
	listTypeComboBox->addItem("SET");
	connect(listTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickListTypeValue(const QString&)));
	keyboardEventComboBox = new QComboBox(this);
	keyboardEventComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	keyboardEventComboBox->setToolTip("value");
	keyboardEventComboBox->hide();
	keyboardEventComboBox->addItem("KEY_DOWN");
	keyboardEventComboBox->addItem("KEY_UP");
	connect(keyboardEventComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickKeyboardEventValue(const QString&)));
	mouseEventComboBox = new QComboBox(this);
	mouseEventComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	mouseEventComboBox->setToolTip("value");
	mouseEventComboBox->hide();
	mouseEventComboBox->addItem("KEY_DOWN");
	mouseEventComboBox->addItem("KEY_UP");
	mouseEventComboBox->addItem("DOUBLE_CLICK");
	mouseEventComboBox->addItem("MOVE");
	mouseEventComboBox->addItem("WHEEL");
	connect(mouseEventComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickMouseEventValue(const QString&)));
	mouseKeyComboBox = new QComboBox(this);
	mouseKeyComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	mouseKeyComboBox->setToolTip("value");
	mouseKeyComboBox->hide();
	mouseKeyComboBox->addItem("LEFT_BUTTON");
	mouseKeyComboBox->addItem("RIGHT_BUTTON");
	mouseKeyComboBox->addItem("MIDDLE_BUTTON");
	connect(mouseKeyComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickMouseKeyValue(const QString&)));
	listIndexComboBox = new QComboBox(this);
	listIndexComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	listIndexComboBox->setToolTip("value");
	listIndexComboBox->hide();
	listIndexComboBox->addItem("HEAD");
	listIndexComboBox->addItem("TAIL");
	listIndexComboBox->addItem("RANDOM");
	connect(listIndexComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickListIndex(const QString&)));
	frameIndexComboBox = new QComboBox(this);
	frameIndexComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	frameIndexComboBox->setToolTip("value");
	frameIndexComboBox->hide();
	frameIndexComboBox->addItem("FIRST");
	frameIndexComboBox->addItem("LAST"); 
	connect(frameIndexComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickFrameIndex(const QString&)));
	floatLerpModeComboBox = new QComboBox(this);
	floatLerpModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	floatLerpModeComboBox->setToolTip("value");
	floatLerpModeComboBox->hide();
	floatLerpModeComboBox->addItem("UNITS_PER_SECOND");
	floatLerpModeComboBox->addItem("FIXED_TIME");
	floatLerpModeComboBox->addItem("PARABOLA");
	floatLerpModeComboBox->addItem("DAMPING");
	floatLerpModeComboBox->addItem("MAGNETIC");
	connect(floatLerpModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickFloatLerpMode(const QString&)));
	vectorLerpModeComboBox = new QComboBox(this);
	vectorLerpModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	vectorLerpModeComboBox->setToolTip("value");
	vectorLerpModeComboBox->hide();
	vectorLerpModeComboBox->addItem("BAZIER");
	vectorLerpModeComboBox->addItem("LAGRANGE");
	vectorLerpModeComboBox->addItem("RING");
	connect(vectorLerpModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickVectorLerpMode(const QString&)));
	listRemoveModeComboBox = new QComboBox(this);
	listRemoveModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	listRemoveModeComboBox->setToolTip("value");
	listRemoveModeComboBox->hide();
	listRemoveModeComboBox->addItem("DESTROY_ITEM");
	listRemoveModeComboBox->addItem("REMOVE_ITEM"); 
	connect(listRemoveModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickListRemoveMode(const QString&)));
	listCopyModeComboBox = new QComboBox(this);
	listCopyModeComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	listCopyModeComboBox->setToolTip("value");
	listCopyModeComboBox->hide();
	listCopyModeComboBox->addItem("SHARE_ITEM");
	listCopyModeComboBox->addItem("COPY_IN_VARIABLE");
	listCopyModeComboBox->addItem("COPY_ONLY_DATA");
	connect(listCopyModeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickListCopyMode(const QString&)));
	keyNameButton = new VirtualKeyEditor(this);
	keyNameButton->setGeometry(QRect(30, 0, 30+25, 24));
	keyNameButton->setToolTip("key name");
	keyNameButton->hide();
	connect(keyNameButton, SIGNAL(KeyboardViewerIsClosed()), this, SLOT(UpdateVirtualKey()));
	hmsEditor = new TimeHMSEditor(this);
	hmsEditor->setGeometry(QRect(30, 0, 30+25, 24));
	hmsEditor->hide();
	connect(hmsEditor, SIGNAL(timeChanged(const QTime&)), this, SLOT(ChangeTimeValue(const QTime&)));
	axisComboBox = new QComboBox(this);
	axisComboBox->setGeometry(QRect(30, 0, 30+25, 24));
	axisComboBox->setToolTip("value");
	axisComboBox->hide();
	axisComboBox->addItem("X"); 
	axisComboBox->addItem("Y");
	axisComboBox->addItem("Z");
    connect(axisComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickAxisIndexValue(const QString&)));
	constsComboBox = new QComboBox(this);
	constsComboBox->setGeometry(QRect(0, 0, 60, 24));
	constsComboBox->setToolTip("Help");
	constsComboBox->addItem("NUMBER");

	constsComboBox->addItem("ASSET_TYPE");

	constsComboBox->addItem("PLAYING_STATE");
	constsComboBox->addItem("PLAYING_MODE");

	constsComboBox->addItem("FRAME_INDEX");

	constsComboBox->addItem("VARIABLE_TYPE");
	
	constsComboBox->addItem("INPUT_DEVICE");
	constsComboBox->addItem("KEYBOARD_EVENT");
	constsComboBox->addItem("KEY"); 
	constsComboBox->addItem("MOUSE_EVENT");
	constsComboBox->addItem("MOUSE_KEY");

	constsComboBox->addItem("TIME"); 

	constsComboBox->addItem("LIST_TYPE");
	constsComboBox->addItem("ITEM_INDEX");
	constsComboBox->addItem("REMOVE_MODE");
	constsComboBox->addItem("COPY_MODE");

	constsComboBox->addItem("PROJECTION_MODE");
	constsComboBox->addItem("ROTATION_MODE");
	constsComboBox->addItem("MOVEMENT_MODE");

	constsComboBox->addItem("INTERSECTION_MODE"); 

	constsComboBox->addItem("BOX_FACE"); 

	constsComboBox->addItem("PLANE"); 

	constsComboBox->addItem("FLOAT_LERP_MODE");

	constsComboBox->addItem("VECTOR_LERP_MODE");

	constsComboBox->addItem("AXIS"); 

	constsComboBox->addItem("ACTIVATION_LIMIT"); 

	constsComboBox->addItem("DEACTIVATION_MODE"); 

	connect(constsComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(PickConstValue(const QString&)));
}



void IntEditorEx::FocusOut(void)
{
	emit FocusOut(this);
}



void IntEditorEx::ChangeTimeValue(const QTime& _time)
{
	HMS newTime(_time.hour(), _time.minute(), _time.second(), _time.msec());
	
	intValue->setValue(HMS::_HMSToMs(newTime));
}




int IntEditorEx::GetKeyboardEventIndex(int _value)
{ 
	if(_value == Keyboard::KEY_DOWN)
	{
		return keyboardEventComboBox->findText("KEY_DOWN");
	}
	else if(_value == Keyboard::KEY_UP)
	{
		return keyboardEventComboBox->findText("KEY_UP");
	}
	return -1;
}


int IntEditorEx::GetMouseEventIndex(int _value)
{ 
	if(_value == Mouse::KEY_DOWN)
	{
		return mouseEventComboBox->findText("KEY_DOWN");
	}
	else if(_value == Mouse::KEY_UP)
	{
		return mouseEventComboBox->findText("KEY_UP");
	}
	else if(_value == Mouse::DBL_CLICK)
	{
		return mouseEventComboBox->findText("DOUBLE_CLICK");
	}	
	else if(_value == Mouse::MOVE)
	{
		return mouseEventComboBox->findText("MOVE");
	}
	else if(_value == Mouse::WHEEL)
	{
		return mouseEventComboBox->findText("WHEEL");
	}
	return -1;
}


int IntEditorEx::GetMouseKeyIndex(int _value)
{ 
	if(_value == Mouse::LEFT_BUTTON)
	{
		return mouseKeyComboBox->findText("LEFT_BUTTON");
	}
	else if(_value == Mouse::RIGHT_BUTTON)
	{
		return mouseKeyComboBox->findText("RIGHT_BUTTON");
	}
	else if(_value == Mouse::MIDDLE_BUTTON)
	{
		return mouseKeyComboBox->findText("MIDDLE_BUTTON");
	}	
	return -1;
}



void IntEditorEx::PickKeyboardEventValue(const QString& _value)
{
	if(_value == "KEY_DOWN")
	{
		intValue->setValue(Keyboard::KEY_DOWN);
	}
	else if(_value == "KEY_UP")
	{
		intValue->setValue(Keyboard::KEY_UP);
	}
}



void IntEditorEx::PickMouseEventValue(const QString& _value)
{ 
	if(_value == "KEY_DOWN")
	{
		intValue->setValue(Mouse::KEY_DOWN);
	}
	else if(_value == "KEY_UP")
	{
		intValue->setValue(Mouse::KEY_UP);
	}
	else if(_value == "DOUBLE_CLICK")
	{
		intValue->setValue(Mouse::DBL_CLICK);
	}
	else if(_value == "MOVE")
	{
		intValue->setValue(Mouse::MOVE);
	}
	else if(_value == "WHEEL")
	{
		intValue->setValue(Mouse::WHEEL);
	}
}


 
void IntEditorEx::PickMouseKeyValue(const QString& _value)
{
	if(_value == "LEFT_BUTTON")
	{
		intValue->setValue(Mouse::LEFT_BUTTON);
	}
	else if(_value == "RIGHT_BUTTON")
	{
		intValue->setValue(Mouse::RIGHT_BUTTON);
	}
	else if(_value == "MIDDLE_BUTTON")
	{
		intValue->setValue(Mouse::MIDDLE_BUTTON);
	}
}


void IntEditorEx::PickListIndex(const QString& _value)
{
	if(_value == "HEAD")
	{
		intValue->setValue(AbstractList::HEAD);
	}
	else if(_value == "TAIL")
	{
		intValue->setValue(AbstractList::TAIL);
	}
	else if(_value == "RANDOM")
	{
		intValue->setValue(AbstractList::RANDOM);
	}
}


void IntEditorEx::PickFrameIndex(const QString& _value)
{
	if(_value == "FIRST")
	{
		intValue->setValue(0);
	}
	else if(_value == "LAST")
	{
		intValue->setValue(-1);
	}
}


void IntEditorEx::PickFloatLerpMode(const QString& _value)
{
	if(_value == "UNITS_PER_SECOND")
	{
		intValue->setValue(FloatLerp::UNITS_PER_SECOND);
	}
	else if(_value == "FIXED_TIME")
	{
		intValue->setValue(FloatLerp::FIXED_TIME);
	}
	else if(_value == "PARABOLA")
	{
		intValue->setValue(FloatLerp::PARABOLA);
	}
	else if(_value == "DAMPING")
	{
		intValue->setValue(FloatLerp::DAMPING);
	}
	else if(_value == "MAGNETIC")
	{
		intValue->setValue(FloatLerp::MAGNETIC);
	}
}



void IntEditorEx::PickVectorLerpMode(const QString& _value)
{
	if(_value == "BAZIER")
	{
		intValue->setValue(VectorLerp::BAZIER);
	}
	else if(_value == "LAGRANGE")
	{
		intValue->setValue(VectorLerp::LAGRANGE);
	}
	else if(_value == "RING")
	{
		intValue->setValue(VectorLerp::RING);
	}
}


void IntEditorEx::PickListRemoveMode(const QString& _value)
{
	if(_value == "DESTROY_ITEM")
	{
		intValue->setValue(AbstractList::DESTROY_ITEM);
	}
	else if(_value == "REMOVE_ITEM")
	{
		intValue->setValue(AbstractList::REMOVE_ITEM);
	}
}


void IntEditorEx::PickListCopyMode(const QString& _value)
{
	if(_value == "SHARE_ITEM")
	{
		intValue->setValue(AbstractList::SHARE_ITEM);
	}
	else if(_value == "COPY_IN_VARIABLE")
	{
		intValue->setValue(AbstractList::COPY_IN_VARIABLE);
	}
	else if(_value == "COPY_ONLY_DATA")
	{
		intValue->setValue(AbstractList::COPY_ONLY_DATA);
	}
}

 
int IntEditorEx::GetListIndex(int _value)
{ 
	if(_value == AbstractList::HEAD)
	{
		return listIndexComboBox->findText("HEAD");
	}
	else if(_value == AbstractList::TAIL)
	{
		return listIndexComboBox->findText("TAIL");
	}
	else if(_value == AbstractList::RANDOM)
	{
		return listIndexComboBox->findText("RANDOM");
	}
	return -1;
}


int IntEditorEx::GetFrameIndex(int _value)
{ 
	if(_value == 0)
	{
		return frameIndexComboBox->findText("FIRST");
	}
	else if(_value == -1)
	{
		return frameIndexComboBox->findText("LAST");
	}
	return 0;
}



int IntEditorEx::GetFloatLerpMode(int _value)
{ 
	if(_value == FloatLerp::UNITS_PER_SECOND)
	{
		return floatLerpModeComboBox->findText("UNITS_PER_SECOND");
	}
	else if(_value == FloatLerp::FIXED_TIME)
	{
		return floatLerpModeComboBox->findText("FIXED_TIME");
	}
	else if(_value == FloatLerp::PARABOLA)
	{
		return floatLerpModeComboBox->findText("PARABOLA");
	}
	else if(_value == FloatLerp::DAMPING)
	{
		return floatLerpModeComboBox->findText("DAMPING");
	}
	else if(_value == FloatLerp::MAGNETIC)
	{
		return floatLerpModeComboBox->findText("MAGNETIC");
	}
	return -1;
}



int IntEditorEx::GetVectorLerpMode(int _value)
{ 
	if(_value == VectorLerp::BAZIER)
	{
		return vectorLerpModeComboBox->findText("BAZIER");
	}
	else if(_value == VectorLerp::LAGRANGE)
	{
		return vectorLerpModeComboBox->findText("LAGRANGE");
	}
	else if(_value == VectorLerp::RING)
	{
		return vectorLerpModeComboBox->findText("RING");
	}
	return -1;
}


int IntEditorEx::GetListRemoveMode(int _value)
{ 
	if(_value == AbstractList::DESTROY_ITEM)
	{
		return listRemoveModeComboBox->findText("DESTROY_ITEM");
	}
	else if(_value == AbstractList::REMOVE_ITEM)
	{
		return listRemoveModeComboBox->findText("REMOVE_ITEM");
	}
	return -1;
}


int IntEditorEx::GetListCopyMode(int _value)
{ 
	if(_value == AbstractList::SHARE_ITEM)
	{
		return listCopyModeComboBox->findText("SHARE_ITEM");
	}
	else if(_value == AbstractList::COPY_IN_VARIABLE)
	{
		return listCopyModeComboBox->findText("COPY_IN_VARIABLE");
	}
	else if(_value == AbstractList::COPY_ONLY_DATA)
	{
		return listCopyModeComboBox->findText("COPY_ONLY_DATA");
	}
	return -1;
}



void IntEditorEx::PickPlayingStateValue(const QString& _value)
{
	if(_value == "PLAY")
	{
		intValue->setValue(PlayingObject::PLAY);
	}
	else if(_value == "PAUSE")
	{
		intValue->setValue(PlayingObject::PAUSE);
	}
	else if(_value == "STOP")
	{
		intValue->setValue(PlayingObject::STOP);
	}
}


void IntEditorEx::PickInputDeviceValue(const QString& _value)
{
	if(_value == "KEYBOARD_DEVICE")
	{
		intValue->setValue(Keyboard::KEYBOARD_DEVICE);
	}
	else if(_value == "MOUSE_DEVICE")
	{
		intValue->setValue(Mouse::MOUSE_DEVICE);
	}
}


void IntEditorEx::PickDeactivationModeValue(const QString& _value)
{
	if(_value == "NEVER_SWITCH_OFF")
	{
		intValue->setValue(ActivatableObject::NEVER_SWITCH_OFF);
	}
	else if(_value == "AUTO_SWITCH_OFF")
	{
		intValue->setValue(ActivatableObject::AUTO_SWITCH_OFF);
	}
	else if(_value == "OVERLAY_SWITCH_OFF")
	{
		intValue->setValue(ActivatableObject::OVERLAY_SWITCH_OFF);
	}
}


void IntEditorEx::PickActivationLimitValue(const QString& _value)
{
	if(_value == "UNLIMITED")
	{
		intValue->setValue(-1);
	}
	else if(_value == "STOPED")
	{
		intValue->setValue(0);
	}
}



void IntEditorEx::PickPlayingModeValue(const QString& _value)
{
	if(_value == "FORWARD_PLAYING")
	{
		intValue->setValue(PlayingObject::FORWARD_PLAYING);
	}
	else if(_value == "BACKWARD_PLAYING")
	{
		intValue->setValue(PlayingObject::BACKWARD_PLAYING);
	}
	else if(_value == "PING_PONG_PLAYING")
	{
		intValue->setValue(PlayingObject::PING_PONG_PLAYING);
	}
	else if(_value == "ONEWAY_PLAYING")
	{
		intValue->setValue(PlayingObject::ONEWAY_PLAYING);
	}
	else if(_value == "LOOP_PLAYING")
	{
		intValue->setValue(PlayingObject::LOOP_PLAYING);
	}
	else if(_value == "SINGLE_SHOT_PLAYING")
	{
		intValue->setValue(PlayingObject::SINGLE_SHOT_PLAYING);
	}
}


void IntEditorEx::PickVariableTypeValue(const QString& _value)
{
	if(_value == "INTEGER")
	{
		intValue->setValue(Variable<int32>::INTEGER_TYPE);
	}
	else if(_value == "FLOAT")
	{
		intValue->setValue(Variable<float>::FLOAT_TYPE);
	}
	else if(_value == "BOOLEAN")
	{
		intValue->setValue(Variable<bool>::BOOLEAN_TYPE);
	}
	else if(_value == "STRING")
	{
		intValue->setValue(Variable<StringANSI>::STRING_TYPE);
	}
	else if(_value == "VECTOR_2")
	{
		intValue->setValue(Variable<Vector2>::VECTOR_2_TYPE);
	}
	else if(_value == "VECTOR_3")
	{
		intValue->setValue(Variable<Vector3>::VECTOR_3_TYPE);
	}
	else if(_value == "QUATERNION")
	{
		intValue->setValue(Variable<Quaternion>::QUATERNION_TYPE);
	}
}



void IntEditorEx::PickProjectionModeValue(const QString& _value)
{
	if(_value == "PERSPECTIVE_PROJECTION")
	{
		intValue->setValue(Camera3D::PERSPECTIVE_PROJECTION);
	}
	else if(_value == "ORTHOGRAPHIC_PROJECTION")
	{
		intValue->setValue(Camera3D::ORTHOGRAPHIC_PROJECTION);
	}
	else if(_value == "SCREEN_PROJECTION")
	{
		intValue->setValue(Camera3D::SCREEN_PROJECTION);
	}
}



void IntEditorEx::PickMovementModeValue(const QString& _value)
{
	if(_value == "FLIGHT_MOVEMENT")
	{
		intValue->setValue(Camera3D::FLIGHT_MOVEMENT);
	}
	else if(_value == "SPECTATOR_MOVEMENT")
	{
		intValue->setValue(Camera3D::SPECTATOR_MOVEMENT);
	}
	else if(_value == "OVERLAND_MOVEMENT")
	{
		intValue->setValue(Camera3D::OVERLAND_MOVEMENT);
	}
}



void IntEditorEx::PickRotationModeValue(const QString& _value)
{
	if(_value == "FIRST_PERSON_ROTATION")
	{
		intValue->setValue(Camera3D::FIRST_PERSON_ROTATION);
	}
	else if(_value == "OBJECT_VIEWER_ROTATION")
	{
		intValue->setValue(Camera3D::OBJECT_VIEWER_ROTATION);
	}
}


void IntEditorEx::PickIntersectionModeValue(const QString& _value)
{
	if(_value == "APPLY_TRANSFORMATION")
	{
		intValue->setValue(AbstractSurface::APPLY_TRANSFORMATION);
	}
	else if(_value == "CHECK_ONLY_FRONT_FACE")
	{
		intValue->setValue(AbstractSurface::CHECK_ONLY_FRONT_FACE);
	}
	else if(_value == "CHECK_ONLY_COLORED_PIXEL")
	{
		intValue->setValue(TexturingSurface::CHECK_ONLY_COLORED_PIXEL);
	}
}
  

void IntEditorEx::PickBoxFaceValue(const QString& _value)
{
	if(_value == "FRONT_FACE")
	{
		intValue->setValue(Texture2D::BOX_FRONT_FACE);
	}
	else if(_value == "BACK_FACE")
	{
		intValue->setValue(Texture2D::BOX_BACK_FACE);
	}
	else if(_value == "LEFT_FACE")
	{
		intValue->setValue(Texture2D::BOX_LEFT_FACE);
	}
	else if(_value == "RIGHT_FACE")
	{
		intValue->setValue(Texture2D::BOX_RIGHT_FACE);
	}
	else if(_value == "TOP_FACE")
	{
		intValue->setValue(Texture2D::BOX_TOP_FACE);
	}
	else if(_value == "BOTTOM_FACE")
	{
		intValue->setValue(Texture2D::BOX_BOTTOM_FACE);
	}
}
  

void IntEditorEx::PickPlaneValue(const QString& _value)
{
	if(_value == "XY")
	{
		intValue->setValue(Plane::XY_PLANE);
	}
	else if(_value == "XZ")
	{
		intValue->setValue(Plane::XZ_PLANE);
	}
	else if(_value == "YZ")
	{
		intValue->setValue(Plane::YZ_PLANE);
	}
}
  

void IntEditorEx::PickListTypeValue(const QString& _value)
{
	if(_value == "INTEGER")
	{
		intValue->setValue(List<int32>::INTEGER_TYPE);
	}
	else if(_value == "FLOAT")
	{
		intValue->setValue(List<float>::FLOAT_TYPE);
	}
	else if(_value == "BOOLEAN")
	{
		intValue->setValue(List<bool>::BOOLEAN_TYPE);
	}
	else if(_value == "STRING")
	{
		intValue->setValue(List<StringANSI>::STRING_TYPE);
	}
	else if(_value == "VECTOR_2")
	{
		intValue->setValue(List<Vector2>::VECTOR_2_TYPE);
	}
	else if(_value == "VECTOR_3")
	{
		intValue->setValue(List<Vector3>::VECTOR_3_TYPE);
	}
	else if(_value == "QUATERNION")
	{
		intValue->setValue(List<Quaternion>::QUATERNION_TYPE);
	}
	else if(_value == "ABSTRACT_OBJECT")
	{
		intValue->setValue(List<AbstractObject>::OBJECT_TYPE);
	}
	else if(_value == "TRANSLATABLE_OBJECT")
	{
		intValue->setValue(List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE);
	}
	else if(_value == "SCALABLE_OBJECT")
	{
		intValue->setValue(List<ScalableObject>::SCALABLE_OBJECT_TYPE);
	}
	else if(_value == "ROTATABLE_OBJECT")
	{
		intValue->setValue(List<RotatableObject>::ROTATABLE_OBJECT_TYPE);
	}
	else if(_value == "COLORABLE_OBJECT")
	{
		intValue->setValue(List<ColorableObject>::COLORABLE_OBJECT_TYPE);
	}
	else if(_value == "SET")
	{
		intValue->setValue(List<Set>::SET_TYPE);
	}
}



void IntEditorEx::PickAxisIndexValue(const QString& _value)
{
	if(_value == "X")
	{
		intValue->setValue(Vector3::X);
	}
	else if(_value == "Y")
	{
		intValue->setValue(Vector3::Y);
	}
	else if(_value == "Z")
	{
		intValue->setValue(Vector3::Z);
	}
}



void IntEditorEx::PickAssetTypeValue(const QString& _value)
{
	if(_value == "ATLAS")
	{
		intValue->setValue(AssetLibrary::ATLAS_2D_ASSET);
	}
	else if(_value == "TEXTURE")
	{
		intValue->setValue(AssetLibrary::TEXTURE_2D_ASSET);
	}
	else if(_value == "TEXTURED_ANIMATION")
	{
		intValue->setValue(AssetLibrary::TEXTURED_ANIMATION_ASSET);
	}
	else if(_value == "TRIGGER")
	{
		intValue->setValue(AssetLibrary::TRIGGER_ASSET);
	}
	else if(_value == "OBJECT")
	{
		intValue->setValue(AssetLibrary::OBJECT_ASSET);
	}
	else if(_value == "TIMER")
	{
		intValue->setValue(AssetLibrary::TIMER_ASSET);
	}
	else if(_value == "PLAYER_CONTROL")
	{
		intValue->setValue(AssetLibrary::PLAYER_CONTROL_ASSET);
	}
	else if(_value == "EVENT")
	{
		intValue->setValue(AssetLibrary::EVENT_ASSET);
	}
	else if(_value == "ACTION")
	{
		intValue->setValue(AssetLibrary::ACTION_ASSET);
	}
	else if(_value == "VARIABLE")
	{
		intValue->setValue(AssetLibrary::VARIABLE_ASSET);
	}
	else if(_value == "LIST")
	{
		intValue->setValue(AssetLibrary::LIST_ASSET);
	}
	else if(_value == "MESH")
	{
		intValue->setValue(AssetLibrary::MESH_ASSET);
	}
	else if(_value == "SCENE")
	{
		intValue->setValue(AssetLibrary::SCENE_ASSET);
	}
	else if(_value == "FLOAT_LERP")
	{
		intValue->setValue(AssetLibrary::FLOAT_LERP_ASSET);
	}
	else if(_value == "QUATERNION_LERP")
	{
		intValue->setValue(AssetLibrary::QUATERNION_LERP_ASSET);
	}
		else if(_value == "VECTOR_LERP")
	{
		intValue->setValue(AssetLibrary::VECTOR_LERP_ASSET);
	}
	else if(_value == "CAMERA")
	{
		intValue->setValue(AssetLibrary::CAMERA_ASSET);
	}
	else if(_value == "HITBOX")
	{
		intValue->setValue(AssetLibrary::HITBOX_ASSET);
	}
}


 
int IntEditorEx::GetVariableTypeIndex(int _value)
{
	if(_value == Variable<int32>::INTEGER_TYPE)
	{
		return variableTypeComboBox->findText("INTEGER");
	}
	else if(_value == Variable<float>::FLOAT_TYPE)
	{
		return variableTypeComboBox->findText("FLOAT");
	}
	else if(_value == Variable<bool>::BOOLEAN_TYPE)
	{
		return variableTypeComboBox->findText("BOOLEAN");
	}
	else if(_value == Variable<StringANSI>::STRING_TYPE)
	{
		return variableTypeComboBox->findText("STRING");
	}
	else if(_value == Variable<Vector2>::VECTOR_2_TYPE)
	{
		return variableTypeComboBox->findText("VECTOR_2");
	}
	else if(_value == Variable<Vector3>::VECTOR_3_TYPE)
	{
		return variableTypeComboBox->findText("VECTOR_3");
	}
	else if(_value == Variable<Quaternion>::QUATERNION_TYPE)
	{
		return variableTypeComboBox->findText("QUATERNION");
	}
	return -1;
}



 
int IntEditorEx::GetProjectionModeIndex(int _value)
{
	if(_value == Camera3D::PERSPECTIVE_PROJECTION)
	{
		return projectionModeComboBox->findText("PERSPECTIVE_PROJECTION");
	}
	else if(_value == Camera3D::ORTHOGRAPHIC_PROJECTION)
	{
		return projectionModeComboBox->findText("ORTHOGRAPHIC_PROJECTION");
	}
	else if(_value == Camera3D::SCREEN_PROJECTION)
	{
		return projectionModeComboBox->findText("SCREEN_PROJECTION");
	}
	
	return -1;
}


int IntEditorEx::GetMovementModeIndex(int _value)
{
	if(_value == Camera3D::FLIGHT_MOVEMENT)
	{
		return movementModeComboBox->findText("FLIGHT_MOVEMENT");
	}
	else if(_value == Camera3D::SPECTATOR_MOVEMENT)
	{
		return movementModeComboBox->findText("SPECTATOR_MOVEMENT");
	}
	else if(_value == Camera3D::OVERLAND_MOVEMENT)
	{
		return movementModeComboBox->findText("OVERLAND_MOVEMENT");
	}
	
	return -1;
}



int IntEditorEx::GetRotationModeIndex(int _value)
{
	if(_value == Camera3D::FIRST_PERSON_ROTATION)
	{
		return rotationModeComboBox->findText("FIRST_PERSON_ROTATION");
	}
	else if(_value == Camera3D::OBJECT_VIEWER_ROTATION)
	{
		return rotationModeComboBox->findText("OBJECT_VIEWER_ROTATION");
	}
	
	return -1;
}



int IntEditorEx::GetIntersectionModeIndex(int _value)
{
	if(_value == AbstractSurface::APPLY_TRANSFORMATION)
	{
		return intersectionModeComboBox->findText("APPLY_TRANSFORMATION");
	}
	else if(_value == AbstractSurface::CHECK_ONLY_FRONT_FACE)
	{
		return intersectionModeComboBox->findText("CHECK_ONLY_FRONT_FACE");
	}
	else if(_value == TexturingSurface::CHECK_ONLY_COLORED_PIXEL)
	{
		return intersectionModeComboBox->findText("CHECK_ONLY_COLORED_PIXEL");
	}
	
	return -1;
}



int IntEditorEx::GetBoxFaceIndex(int _value)
{
	if(_value == Texture2D::BOX_FRONT_FACE)
	{
		return boxFaceComboBox->findText("FRONT_FACE");
	}
	else if(_value == Texture2D::BOX_BACK_FACE)
	{
		return boxFaceComboBox->findText("BACK_FACE");
	}
	else if(_value == Texture2D::BOX_LEFT_FACE)
	{
		return boxFaceComboBox->findText("LEFT_FACE");
	}
	else if(_value == Texture2D::BOX_RIGHT_FACE)
	{
		return boxFaceComboBox->findText("RIGHT_FACE");
	}
	else if(_value == Texture2D::BOX_TOP_FACE)
	{
		return boxFaceComboBox->findText("TOP_FACE");
	}
	else if(_value == Texture2D::BOX_BOTTOM_FACE)
	{
		return boxFaceComboBox->findText("BOTTOM_FACE");
	}
	
	return -1;
}


int IntEditorEx::GetPlaneIndex(int _value)
{
	if(_value == Plane::XY_PLANE)
	{
		return planeComboBox->findText("XY");
	}
	else if(_value == Plane::XZ_PLANE)
	{
		return planeComboBox->findText("XZ");
	}
	else if(_value == Plane::YZ_PLANE)
	{
		return planeComboBox->findText("YZ");
	}
	
	return -1;
}


int IntEditorEx::GetAxisIndex(int _value)
{
	if(_value == Vector3::X)
	{
		return axisComboBox->findText("X");
	}
	else if(_value == Vector3::Y)
	{
		return axisComboBox->findText("Y");
	}
	else if(_value == Vector3::Z)
	{
		return axisComboBox->findText("Z");
	}
	return -1;
}


int IntEditorEx::GetListTypeIndex(int _value)
{
	if(_value == List<int32>::INTEGER_TYPE)
	{
		return listTypeComboBox->findText("INTEGER");
	}
	else if(_value == List<float>::FLOAT_TYPE)
	{
		return listTypeComboBox->findText("FLOAT");
	}
	else if(_value == List<bool>::BOOLEAN_TYPE)
	{
		return listTypeComboBox->findText("BOOLEAN");
	}
	else if(_value == List<StringANSI>::STRING_TYPE)
	{
		return listTypeComboBox->findText("STRING");
	}
	else if(_value == List<Vector2>::VECTOR_2_TYPE)
	{
		return listTypeComboBox->findText("VECTOR_2");
	}
	else if(_value == List<Vector3>::VECTOR_3_TYPE)
	{
		return listTypeComboBox->findText("VECTOR_3");
	}
	else if(_value == List<Quaternion>::QUATERNION_TYPE)
	{
		return listTypeComboBox->findText("QUATERNION");
	}
	else if(_value == List<AbstractObject>::OBJECT_TYPE)
	{
		return listTypeComboBox->findText("ABSTRACT_OBJECT");
	}
	else if(_value == List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE)
	{
		return listTypeComboBox->findText("TRANSLATABLE_OBJECT");
	}
	else if(_value == List<ScalableObject>::SCALABLE_OBJECT_TYPE)
	{
		return listTypeComboBox->findText("SCALABLE_OBJECT");
	}
	else if(_value == List<RotatableObject>::ROTATABLE_OBJECT_TYPE)
	{
		return listTypeComboBox->findText("ROTATABLE_OBJECT");
	}
	else if(_value == List<ColorableObject>::COLORABLE_OBJECT_TYPE)
	{
		return listTypeComboBox->findText("COLORABLE_OBJECT");
	}
	else if(_value == List<Set>::SET_TYPE)
	{
		return listTypeComboBox->findText("SET");
	}

	return -1;
}




int IntEditorEx::GetAssetTypeIndex(int _value)
{
	if(_value == AssetLibrary::ATLAS_2D_ASSET)
	{
		return assetTypeComboBox->findText("ATLAS");
	}
	else if(_value == AssetLibrary::TEXTURE_2D_ASSET)
	{
		return assetTypeComboBox->findText("TEXTURE");
	}
	else if(_value == AssetLibrary::TEXTURED_ANIMATION_ASSET)
	{
		return assetTypeComboBox->findText("TEXTURED_ANIMATION");
	}
	else if(_value == AssetLibrary::TRIGGER_ASSET)
	{
		return assetTypeComboBox->findText("TRIGGER");
	}
	else if(_value == AssetLibrary::OBJECT_ASSET)
	{
		return assetTypeComboBox->findText("OBJECT");
	}
	else if(_value == AssetLibrary::TIMER_ASSET)
	{
		return assetTypeComboBox->findText("TIMER");
	}
	else if(_value == AssetLibrary::PLAYER_CONTROL_ASSET)
	{
		return assetTypeComboBox->findText("PLAYER_CONTROL");
	}
	else if(_value == AssetLibrary::EVENT_ASSET)
	{
		return assetTypeComboBox->findText("EVENT");
	}
	else if(_value == AssetLibrary::ACTION_ASSET)
	{
		return assetTypeComboBox->findText("ACTION");
	}
	else if(_value == AssetLibrary::VARIABLE_ASSET)
	{
		return assetTypeComboBox->findText("VARIABLE");
	}
	else if(_value == AssetLibrary::LIST_ASSET)
	{
		return assetTypeComboBox->findText("LIST");
	}
	else if(_value == AssetLibrary::MESH_ASSET)
	{
		return assetTypeComboBox->findText("MESH");
	}
	else if(_value == AssetLibrary::SCENE_ASSET)
	{
		return assetTypeComboBox->findText("SCENE");
	}
	else if(_value == AssetLibrary::FLOAT_LERP_ASSET)
	{
		return assetTypeComboBox->findText("FLOAT_LERP");
	}
	else if(_value == AssetLibrary::QUATERNION_LERP_ASSET)
	{
		return assetTypeComboBox->findText("QUATERNION_LERP");
	}
	else if(_value == AssetLibrary::VECTOR_LERP_ASSET)
	{
		return assetTypeComboBox->findText("VECTOR_LERP");
	}
	else if(_value == AssetLibrary::CAMERA_ASSET)
	{
		return assetTypeComboBox->findText("CAMERA");
	}
	else if(_value == AssetLibrary::HITBOX_ASSET)
	{
		return assetTypeComboBox->findText("HITBOX");
	}

	return -1;
}



int IntEditorEx::GetPlayingStateIndex(int _value)
{ 
	if(_value == PlayingObject::PLAY)
	{
		return playingStateComboBox->findText("PLAY");
	}
	else if(_value == PlayingObject::PAUSE)
	{
		return playingStateComboBox->findText("PAUSE");
	}
	else if(_value == PlayingObject::STOP)
	{
		return playingStateComboBox->findText("STOP");
	}
	return -1;
}


int IntEditorEx::GetInputDeviceIndex(int _value)
{ 
	if(_value == Keyboard::KEYBOARD_DEVICE)
	{
		return inputDeviceComboBox->findText("KEYBOARD_DEVICE");
	}
	else if(_value == Mouse::MOUSE_DEVICE)
	{
		return inputDeviceComboBox->findText("MOUSE_DEVICE");
	}
	return -1;
}


int IntEditorEx::GetDeactivationModeIndex(int _value)
{ 
	if(_value == ActivatableObject::NEVER_SWITCH_OFF)
	{
		return deactivationModeComboBox->findText("NEVER_SWITCH_OFF");
	}
	else if(_value == ActivatableObject::AUTO_SWITCH_OFF)
	{
		return deactivationModeComboBox->findText("AUTO_SWITCH_OFF");
	}
	else if(_value == ActivatableObject::OVERLAY_SWITCH_OFF)
	{
		return deactivationModeComboBox->findText("OVERLAY_SWITCH_OFF");
	}
	return -1;
}


int IntEditorEx::GetActivationLimitIndex(int _value)
{ 
	if(_value == -1)
	{
		return activationLimitComboBox->findText("UNLIMITED");
	}
	else if(_value == 0)
	{ 
		return activationLimitComboBox->findText("STOPED");
	}
	return -1;
}



int IntEditorEx::GetPlayingModeIndex(int _value)
{ 
	if(_value == PlayingObject::FORWARD_PLAYING)
	{
		return playingModeComboBox->findText("FORWARD_PLAYING");
	}
	else if(_value == PlayingObject::BACKWARD_PLAYING)
	{
		return playingModeComboBox->findText("BACKWARD_PLAYING");
	}
	else if(_value == PlayingObject::PING_PONG_PLAYING)
	{
		return playingModeComboBox->findText("PING_PONG_PLAYING");
	}
	else if(_value == PlayingObject::ONEWAY_PLAYING)
	{
		return playingModeComboBox->findText("ONEWAY_PLAYING");
	}
	else if(_value == PlayingObject::LOOP_PLAYING)
	{
		return playingModeComboBox->findText("LOOP_PLAYING");
	}
	else if(_value == PlayingObject::SINGLE_SHOT_PLAYING)
	{
		return playingModeComboBox->findText("SINGLE_SHOT_PLAYING");
	}
	return -1;
}


void IntEditorEx::PickConstValue(const QString& _value)
{
	intValue->hide();
	assetTypeComboBox->hide();
	playingStateComboBox->hide();
	inputDeviceComboBox->hide();
	deactivationModeComboBox->hide();
	activationLimitComboBox->hide();
	playingModeComboBox->hide();
	variableTypeComboBox->hide();
	projectionModeComboBox->hide();
	movementModeComboBox->hide();
	rotationModeComboBox->hide();
	intersectionModeComboBox->hide();
	boxFaceComboBox->hide();
	planeComboBox->hide();
	listTypeComboBox->hide();
	keyboardEventComboBox->hide();
	mouseEventComboBox->hide();
	mouseKeyComboBox->hide();
	listIndexComboBox->hide();
	frameIndexComboBox->hide();
	floatLerpModeComboBox->hide();
	vectorLerpModeComboBox->hide();
	listRemoveModeComboBox->hide();
	listCopyModeComboBox->hide();
	hmsEditor->hide();
	keyNameButton->hide();
	axisComboBox->hide();
	
	if(_value == "NUMBER")
	{
		intValue->show();
	}
	else if(_value == "ASSET_TYPE")
	{
		assetTypeComboBox->show();
		assetTypeComboBox->setCurrentIndex(GetAssetTypeIndex(intValue->value()));
	}
	else if(_value == "VARIABLE_TYPE")
	{
		variableTypeComboBox->show();
		variableTypeComboBox->setCurrentIndex(GetVariableTypeIndex(intValue->value()));
	}  
	else if(_value == "PROJECTION_MODE")
	{
		projectionModeComboBox->show();
		projectionModeComboBox->setCurrentIndex(GetProjectionModeIndex(intValue->value()));
	} 
	else if(_value == "MOVEMENT_MODE")
	{
		movementModeComboBox->show();
		movementModeComboBox->setCurrentIndex(GetMovementModeIndex(intValue->value()));
	} 
	else if(_value == "ROTATION_MODE")
	{
		rotationModeComboBox->show();
		rotationModeComboBox->setCurrentIndex(GetRotationModeIndex(intValue->value()));
	} 
	else if(_value == "INTERSECTION_MODE")
	{
		intersectionModeComboBox->show();
		intersectionModeComboBox->setCurrentIndex(GetIntersectionModeIndex(intValue->value()));
	} 
	else if(_value == "BOX_FACE")
	{
		boxFaceComboBox->show();
		boxFaceComboBox->setCurrentIndex(GetBoxFaceIndex(intValue->value()));
	} 
	else if(_value == "PLANE")
	{
		planeComboBox->show();
		planeComboBox->setCurrentIndex(GetPlaneIndex(intValue->value()));
	}
	else if(_value == "LIST_TYPE")
	{
		listTypeComboBox->show();
		listTypeComboBox->setCurrentIndex(GetListTypeIndex(intValue->value()));
	} 
	else if(_value == "PLAYING_STATE")
	{
		playingStateComboBox->show();
		playingStateComboBox->setCurrentIndex(GetPlayingStateIndex(intValue->value()));
	}
	else if(_value == "INPUT_DEVICE")
	{
		inputDeviceComboBox->show();
		inputDeviceComboBox->setCurrentIndex(GetInputDeviceIndex(intValue->value()));
	}
	else if(_value == "DEACTIVATION_MODE")
	{
		deactivationModeComboBox->show();
		deactivationModeComboBox->setCurrentIndex(GetDeactivationModeIndex(intValue->value()));
	}
	else if(_value == "ACTIVATION_LIMIT")
	{ 
		activationLimitComboBox->show();
		activationLimitComboBox->setCurrentIndex(GetActivationLimitIndex(intValue->value()));
	}
	else if(_value == "PLAYING_MODE")
	{
		playingModeComboBox->show();
		playingModeComboBox->setCurrentIndex(GetPlayingModeIndex(intValue->value()));
	}
	else if(_value == "KEYBOARD_EVENT") 
	{
		keyboardEventComboBox->show();
		keyboardEventComboBox->setCurrentIndex(GetKeyboardEventIndex(intValue->value()));
	}
	else if(_value == "MOUSE_EVENT") 
	{
		mouseEventComboBox->show();
		mouseEventComboBox->setCurrentIndex(GetMouseEventIndex(intValue->value()));
	}
	else if(_value == "MOUSE_KEY") 
	{
		mouseKeyComboBox->show();
		mouseKeyComboBox->setCurrentIndex(GetMouseKeyIndex(intValue->value()));
	}
	else if(_value == "ITEM_INDEX") 
	{
		listIndexComboBox->show();
		listIndexComboBox->setCurrentIndex(GetListIndex(intValue->value()));
	}
	else if(_value == "FRAME_INDEX") 
	{
		frameIndexComboBox->show();
		frameIndexComboBox->setCurrentIndex(GetFrameIndex(intValue->value()));
	}
	else if(_value == "FLOAT_LERP_MODE") 
	{
		floatLerpModeComboBox->show();
		floatLerpModeComboBox->setCurrentIndex(GetFloatLerpMode(intValue->value()));
	}
	else if(_value == "VECTOR_LERP_MODE") 
	{
		vectorLerpModeComboBox->show();
		vectorLerpModeComboBox->setCurrentIndex(GetVectorLerpMode(intValue->value()));
	}
	else if(_value == "REMOVE_MODE") 
	{
		listRemoveModeComboBox->show();
		listRemoveModeComboBox->setCurrentIndex(GetListRemoveMode(intValue->value()));
	}
	else if(_value == "COPY_MODE") 
	{
		listCopyModeComboBox->show();
		listCopyModeComboBox->setCurrentIndex(GetListCopyMode(intValue->value()));
	}
	else if(_value == "TIME")
	{
		hmsEditor->show();
		HMS time = HMS::_MsToHMS(intValue->value());
		hmsEditor->setTime(QTime(time.GetHours(), time.GetMinutes(), time.GetSeconds(), time.GetMs()));
	}
	else if(_value == "KEY")
	{
		keyNameButton->show();
		keyNameButton->setText(Keyboard::_GetKeyName((Keyboard::Key)intValue->value()).c_str());
	}
	else if(_value == "AXIS")
	{
		axisComboBox->show();
		axisComboBox->setCurrentIndex(GetAxisIndex(intValue->value()));
	}
}




void IntEditorEx::UpdateVirtualKey(void)
{
	intValue->setValue((int)Keyboard::_GetKeyId(keyNameButton->text().toAscii().data()));
}




void IntEditorEx::SetValue(int _value)
{
	intValue->setValue(_value);
}


QString IntEditorEx::GetValueString(void)const
{
	return QString::number(intValue->value());
}
 



void IntEditorEx::resizeEvent(QResizeEvent* _event)
{
	int32 size = _event->size().width() / 2;

	constsComboBox->setGeometry(QRect(0, 0, size, 24));
	
	intValue->setGeometry(QRect(size, 0, size, 24)); 
	keyboardEventComboBox->setGeometry(QRect(size, 0, size, 24)); 
	mouseEventComboBox->setGeometry(QRect(size, 0, size, 24)); 
	mouseKeyComboBox->setGeometry(QRect(size, 0, size, 24)); 
	assetTypeComboBox->setGeometry(QRect(size, 0, size, 24)); 
	playingStateComboBox->setGeometry(QRect(size, 0, size, 24)); 
	inputDeviceComboBox->setGeometry(QRect(size, 0, size, 24)); 
	deactivationModeComboBox->setGeometry(QRect(size, 0, size, 24)); 
	activationLimitComboBox->setGeometry(QRect(size, 0, size, 24)); 
	playingModeComboBox->setGeometry(QRect(size, 0, size, 24)); 
	variableTypeComboBox->setGeometry(QRect(size, 0, size, 24));
	projectionModeComboBox->setGeometry(QRect(size, 0, size, 24));
	movementModeComboBox->setGeometry(QRect(size, 0, size, 24));
	rotationModeComboBox->setGeometry(QRect(size, 0, size, 24));
	intersectionModeComboBox->setGeometry(QRect(size, 0, size, 24));
	boxFaceComboBox->setGeometry(QRect(size, 0, size, 24));
	planeComboBox->setGeometry(QRect(size, 0, size, 24));
	listTypeComboBox->setGeometry(QRect(size, 0, size, 24)); 
	listIndexComboBox->setGeometry(QRect(size, 0, size, 24));
	frameIndexComboBox->setGeometry(QRect(size, 0, size, 24));
	floatLerpModeComboBox->setGeometry(QRect(size, 0, size, 24));
	vectorLerpModeComboBox->setGeometry(QRect(size, 0, size, 24));
	listRemoveModeComboBox->setGeometry(QRect(size, 0, size, 24));
	listCopyModeComboBox->setGeometry(QRect(size, 0, size, 24));
	hmsEditor->setGeometry(QRect(size, 0, size, 24)); 
	keyNameButton->setGeometry(QRect(size-1, -1, size+2, 24+2)); 
	axisComboBox->setGeometry(QRect(size, 0, size, 24)); 
	
	QWidget::resizeEvent(_event);
}


