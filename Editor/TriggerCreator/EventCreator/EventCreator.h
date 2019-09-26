#ifndef EVENTCREATOR_H
#define EVENTCREATOR_H
#include <QTreeWidget>
#include <QListView>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Editor/TriggerCreator/EventCreator/EventTableWidget/EventTableWidget.h>
#include <Editor/AssetsLibraryTree/AssetsLibraryTree.h>
class EventCreator: public QDialog 
{
	Q_OBJECT
	private: QLabel* eventStateLabel;
	private: EventTableWidget *eventTable;

	private: AbstractEvent* event;
	private: bool fastCreatingEnable;
	private: QLabel *autoNameLabel;
    private: QCheckBox *autoNameCheckBox;

	private: QFrame *line;
     	
    private: QLabel* assetsLibraryLabel;
	private: AssetsLibraryTree* assetsLibraryTree;
    private: QPushButton* refreshAssetsLibraryPushButton;
   
    private: QToolButton* helpButton;
    private: QPushButton *okButton;
    private: QPushButton *cancelButton;
	public: EventCreator(QWidget* _parent = 0);
	public: virtual ~EventCreator(void);
    private slots: void IsOk(void);
	private slots: void ShowDoc(void);
	public slots: void RefreshAssetsLibrary(void);
	public slots: void RefreshEventTable(void);
	public: void SetNameDisable(bool _disable);
	public: void SetObjectName(QString _name);
	public slots: void AutoNameIsChanged(int _state);
	public: QString Vector3ToString(Vector3 _vector);
	public: Vector3 StringToVector3(QString _vector);
	public: QString Vector2ToString(Vector2 _vector);
	public: Vector2 StringToVector2(QString _vector);
	public: QString QuaternionToString(Quaternion _quaternion);
	public: Quaternion StringToQuaternion(QString _quaternion);
	public: int32 IndexOfDeactivationMode(QString _text);
	public: QString StringOfDeactivationMode(int32 _mode);
	public: void SetFastCreating(bool _state);
	public: AbstractEvent* GetEvent(void)const;
	public: void SetEvent(QString _name);
	signals: void EventIsEdited(QString _name);
	private: void CHANGE_NAME_SetEvent(int _eventType);
	private: void CHANGE_NAME_Create(void);
	private: void SEND_MESSAGE_SetEvent(int _eventType);
	private: void SEND_MESSAGE_Create(void);
	private: void CREATE_OBJECT_SetEvent(int _eventType);
	private: void CREATE_OBJECT_Create(void);
	private: void DESTROY_OBJECT_SetEvent(int _eventType);
	private: void DESTROY_OBJECT_Create(void);
	private: void TIMER_FINISH_SetEvent(int _eventType);
	private: void TIMER_FINISH_Create(void);
	private: void FLOAT_LERP_FINISH_SetEvent(int _eventType);
	private: void FLOAT_LERP_FINISH_Create(void);
	private: void PICK_SCENE_SetEvent(int _eventType);
	private: void PICK_SCENE_Create(void);
	private: void ADD_TO_LAYER_SetEvent(int _eventType);
	private: void ADD_TO_LAYER_Create(void);
	private: void REMOVE_FROM_LAYER_SetEvent(int _eventType);
	private: void REMOVE_FROM_LAYER_Create(void);
	private: void ADD_TO_UPDATE_LIST_SetEvent(int _eventType);
	private: void ADD_TO_UPDATE_LIST_Create(void);
	private: void REMOVE_FROM_UPDATE_LIST_SetEvent(int _eventType);
	private: void REMOVE_FROM_UPDATE_LIST_Create(void);
	private: void REMOVE_FROM_RENDER_LIST_SetEvent(int _eventType);
	private: void REMOVE_FROM_RENDER_LIST_Create(void);
	private: void ADD_TO_RENDER_LIST_SetEvent(int _eventType);
	private: void ADD_TO_RENDER_LIST_Create(void);
	private: void CHANGE_CAMERA_SetEvent(int _eventType);
	private: void CHANGE_CAMERA_Create(void);
	private: void ENABLE_POSITION_OFFSET_SetEvent(int _eventType);
	private: void ENABLE_POSITION_OFFSET_Create(void);
	private: void CHANGE_FRONT_TEXTURE_SetEvent(int _eventType);
	private: void CHANGE_FRONT_TEXTURE_Create(void);
	private: void CHANGE_BACK_TEXTURE_SetEvent(int _eventType);
	private: void CHANGE_BACK_TEXTURE_Create(void);
	private: void CHANGE_FRONT_TEXTURED_ANIMATION_SetEvent(int _eventType);
	private: void CHANGE_FRONT_TEXTURED_ANIMATION_Create(void);
	private: void CHANGE_BACK_TEXTURED_ANIMATION_SetEvent(int _eventType);
	private: void CHANGE_BACK_TEXTURED_ANIMATION_Create(void);
	private: void CHANGE_ANIMATION_FRAME_SetEvent(int _eventType);
	private: void CHANGE_ANIMATION_FRAME_Create(void);
	private: void ENABLE_PROPERTY_SetEvent(int _eventType);
	private: void ENABLE_PROPERTY_Create(void);
	private: void CHANGE_ATLAS_SetEvent(int _eventType);
	private: void CHANGE_ATLAS_Create(void);
	private: void PLAYER_CONTROL_FINISH_SetEvent(int _eventType);
	private: void PLAYER_CONTROL_FINISH_Create(void);
	private: void TRIGGER_FINISH_SetEvent(int _eventType);
	private: void TRIGGER_FINISH_Create(void);
	private: void PICK_CAMERA_SetEvent(int _eventType);
	private: void PICK_CAMERA_Create(void);
	private: void CHANGE_MESH_SetEvent(int _eventType);
	private: void CHANGE_MESH_Create(void);
	private: void CHANGE_FLOAT_LERP_SetEvent(int _eventType);
	private: void CHANGE_FLOAT_LERP_Create(void);
	private: void CHANGE_PLAYING_STATE_SetEvent(int _eventType);
	private: void CHANGE_PLAYING_STATE_Create(void);
	private: void CHANGE_PLAYING_MODE_SetEvent(int _eventType);
	private: void CHANGE_PLAYING_MODE_Create(void);
	private: void CHANGE_VALUE_SetEvent(int _eventType);
	private: void CHANGE_VALUE_Create(void);
	private: void COLLISION_BETWEEN_OBJECTS_SetEvent(int _eventType);
	private: void COLLISION_BETWEEN_OBJECTS_Create(void);
	private: void CHANGE_HITBOX_SetEvent(int _eventType);
	private: void CHANGE_HITBOX_Create(void);
	private: void FLOAT_LERP_START_SetEvent(int _eventType);
	private: void FLOAT_LERP_START_Create(void);

			
};


#endif 

