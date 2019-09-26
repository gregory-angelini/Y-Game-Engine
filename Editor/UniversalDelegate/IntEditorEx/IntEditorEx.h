#ifndef INTEDITOREX_H
#define INTEDITOREX_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/IntSpinBox/IntSpinBox.h>
#include <Editor/UniversalDelegate/TimeHMSEditor/TimeHMSEditor.h>
#include <Editor/UniversalDelegate/VirtualKeyEditor/VirtualKeyEditor.h>
class IntEditorEx: public QWidget
{
	Q_OBJECT
	private: QComboBox* constsComboBox;

	private: IntSpinBox* intValue;
	private: QComboBox* assetTypeComboBox;
	private: QComboBox* playingStateComboBox;
	private: QComboBox* inputDeviceComboBox;
	private: QComboBox* mouseEventComboBox;
	private: QComboBox* mouseKeyComboBox;
	private: QComboBox* activationLimitComboBox;
	private: QComboBox* deactivationModeComboBox;
	private: QComboBox* playingModeComboBox;
	private: QComboBox* variableTypeComboBox;
    private: QComboBox* projectionModeComboBox;
    private: QComboBox* intersectionModeComboBox;
	private: QComboBox* listTypeComboBox;
	private: QComboBox* keyboardEventComboBox;
	private: QComboBox* listIndexComboBox;
	private: QComboBox* frameIndexComboBox;
	private: QComboBox* floatLerpModeComboBox;
	private: QComboBox* vectorLerpModeComboBox;
	private: QComboBox* listRemoveModeComboBox;
	private: QComboBox* listCopyModeComboBox;
	private: TimeHMSEditor* hmsEditor;
	private: VirtualKeyEditor* keyNameButton;
	private: QComboBox* axisComboBox;
	private: QComboBox* boxFaceComboBox;
	private: QComboBox* planeComboBox;
	private: QComboBox* rotationModeComboBox;
	private: QComboBox* movementModeComboBox;
    public: IntEditorEx(QWidget* _parent = 0);
	private: virtual void resizeEvent(QResizeEvent* _event);
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	public: QString GetValueString(void)const;
	public: void SetValue(int _value);
	private slots: void PickConstValue(const QString& _value);
	private slots: void PickAssetTypeValue(const QString& _value);
	private slots: void PickPlayingStateValue(const QString& _value);
	private slots: void PickInputDeviceValue(const QString& _value);
	private slots: void PickDeactivationModeValue(const QString& _value);
	private slots: void PickActivationLimitValue(const QString& _value);
	private slots: void PickPlayingModeValue(const QString& _value);
	private slots: void PickVariableTypeValue(const QString& _value);
	private slots: void PickProjectionModeValue(const QString& _value);
	private slots: void PickMovementModeValue(const QString& _value);
	private slots: void PickRotationModeValue(const QString& _value);
	private slots: void PickIntersectionModeValue(const QString& _value);
	private slots: void PickBoxFaceValue(const QString& _value);
	private slots: void PickPlaneValue(const QString& _value);
	private slots: void PickListTypeValue(const QString& _value);
	private slots: void PickAxisIndexValue(const QString& _value);
	private slots: void PickKeyboardEventValue(const QString& _value);
	private slots: void PickMouseEventValue(const QString& _value);
	private slots: void PickMouseKeyValue(const QString& _value);
	private slots: void PickListIndex(const QString& _value);
	private slots: void PickFrameIndex(const QString& _value);
	private slots: void PickFloatLerpMode(const QString& _value);
	private slots: void PickVectorLerpMode(const QString& _value);
	private slots: void PickListRemoveMode(const QString& _value);
	private slots: void PickListCopyMode(const QString& _value);
	private: int GetAssetTypeIndex(int _value);
	private: int GetPlayingStateIndex(int _value);
	private: int GetInputDeviceIndex(int _value);
	private: int GetDeactivationModeIndex(int _value);
	private: int GetActivationLimitIndex(int _value);
	private: int GetPlayingModeIndex(int _value);
	private: int GetAxisIndex(int _value);
	private: int GetVariableTypeIndex(int _value);
	private: int GetProjectionModeIndex(int _value);
	private: int GetRotationModeIndex(int _value);
	private: int GetMovementModeIndex(int _value);
	private: int GetIntersectionModeIndex(int _value);
	private: int GetBoxFaceIndex(int _value);
	private: int GetPlaneIndex(int _value);
	private: int GetListTypeIndex(int _value);
	private: int GetKeyboardEventIndex(int _value);
	private: int GetMouseEventIndex(int _value);
	private: int GetMouseKeyIndex(int _value);
	private: int GetListIndex(int _value);
	private: int GetFrameIndex(int _value);
	private: int GetFloatLerpMode(int _value);
	private: int GetVectorLerpMode(int _value);
	private: int GetListRemoveMode(int _value);
	private: int GetListCopyMode(int _value);
	private slots: void ChangeTimeValue(const QTime& _time);
	private slots: void UpdateVirtualKey(void);
};



#endif 

