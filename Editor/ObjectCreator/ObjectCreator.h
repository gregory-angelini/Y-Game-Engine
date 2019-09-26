#ifndef OBJECTCREATOR_H
#define OBJECTCREATOR_H
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
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Editor/ObjectCreator/TriggerListWidget/TriggerListWidget.h>
class ObjectCreator: public QDialog 
{
	Q_OBJECT
	private: QGroupBox *propertiesGroupBox;
    private: QGroupBox *logicsGroupBox;
    private: QGroupBox *graphicsGroupBox;
    private: QGroupBox *assetsGroupBox;
	private: QLabel* supportedEventsListLabel;
	private: TriggerListWidget* supportedEventsList;
	private: QLabel* supportedActionsListLabel;
	private: TriggerListWidget* supportedActionsList;
	private: QScrollArea* logicsScrollArea;
    private: QWidget* logicsScrollAreaWidget;
	private: QVBoxLayout* logicsVLayout;
	private: QScrollArea* graphicsScrollArea;
    private: QWidget* graphicsScrollAreaWidget;
	private: QVBoxLayout* graphicsVLayout;
	private: QScrollArea* assetsScrollArea;
    private: QWidget* assetsScrollAreaWidget;
	private: QVBoxLayout* assetsVLayout;
    private: QCheckBox *polygonalSurfaceCheckBox;
	private: QCheckBox *logicObjectCheckBox;
	private: QCheckBox *abstractObjectCheckBox;
	private: QCheckBox *translatableObjectCheckBox;
	private: QCheckBox *scalableObjectCheckBox;
	private: QCheckBox *rotatableObjectCheckBox;
	private: QCheckBox *timerCheckBox;
	private: QCheckBox *planeCheckBox;
    private: QCheckBox *setCheckBox;
	private: QCheckBox *abstractSurfaceCheckBox;
	private: QCheckBox *playingObjectCheckBox;
	private: QCheckBox *lerpingObjectCheckBox;
	private: QCheckBox *activatableObjectCheckBox;
    private: QCheckBox *objectSceneCheckBox;
	private: QCheckBox *logicSceneCheckBox;
	private: QCheckBox *graphicsSceneCheckBox;
	private: QCheckBox *tiledSpace3DCheckBox;
	private: QCheckBox *sceneCheckBox;

	private: QCheckBox *playerControlCheckBox;

	private: QCheckBox *triggerCheckBox;

	private: QCheckBox *eventCheckBox;

	private: QCheckBox *actionCheckBox;

	private: QCheckBox *abstractAnimationCheckBox;
	private: QCheckBox *texturedAnimationCheckBox;
			 	
    private: QCheckBox *atlasCheckBox;

	private: QCheckBox *textureCheckBox;

	private: QCheckBox *cameraCheckBox;

	private: QCheckBox *meshCheckBox;

    private: QCheckBox *hitboxCheckBox;

	private: QCheckBox *floatLerpCheckBox;

	private: QCheckBox *quaternionLerpCheckBox;

	private: QCheckBox *vectorLerpCheckBox;
		
    private: QCheckBox *abstractListCheckBox;

	private: QCheckBox *abstractVariableCheckBox;
    private: QCheckBox *variableIntCheckBox;
	private: QCheckBox *variableFloatCheckBox;
    private: QCheckBox *variableBoolCheckBox;
	private: QCheckBox *variableStringCheckBox;
	private: QCheckBox *variableVector2CheckBox;
	private: QCheckBox *variableVector3CheckBox;
	private: QCheckBox *variableQuaternionCheckBox;
	private: QCheckBox *graphicsLineCheckBox;
	private: QCheckBox *graphicsStripCheckBox;;
	private: QCheckBox *graphicsPointCheckBox;
	private: QCheckBox *texturingSurfaceCheckBox;
	private: QCheckBox *graphicsSurfaceCheckBox;
	private: QCheckBox *graphicsObjectCheckBox;
	private: QCheckBox *colorableObjectCheckBox;
	private: QCheckBox *graphicsContainerCheckBox;
	private: int nameMaxLength;
    private: QLabel *nameLabel;
    private: QLineEdit *nameLineEdit;
	private: bool fastCreatingEnable;
    private: QPushButton* cancelButton;
    private: QPushButton* okButton;
	private:QToolButton* helpButton;
    public: ObjectCreator(QWidget* parent = 0);
	public: virtual ~ObjectCreator(void);
    private slots: void IsOk(void);
	private slots: void ShowDoc(void);
	public: void SetObjectName(QString _name);
	public: QString GetObjectName(void)const;
	public: void SetNameDisable(bool _disable);
	public: bool IsNameDisabled(void)const;
	public: void SetFastCreating(bool _state);
	public: void ReviewObject(QString _path);
	public: void ReviewObject(AbstractObject* _object);
	private: QList<int64> GetActiveSelectedProperties(void);
	private: QList<int64> GetActiveAndPassiveSelectedProperties(void);
	private: QList<int64> GetEnabledUnselectedProperties(void);
	private: QList<int64> GetActiveDisabledUnselectedProperties(void);
	private: void DisableCheckBox(int64 _type, bool _disable);
	private: void SelectCheckBox(int64 _type, bool _enable);
	private: void DisableIncompatibleProperties(int _state, int64 _type);
	private: void EnableCompatibleProperties(QList<int64> _selectedProperties);
	private: int64 GetId(QCheckBox* _checkBox);
	private: void UpdateCheckbox(int _state, QCheckBox* _checkBox, QList<int64> _selectedProperties, bool _freeze = false);
	private slots: void GraphicsObjectChecked(int _enable);
	private slots: void GraphicsContainerChecked(int _enable);
	private slots: void ColorableObjectChecked(int _enable);
	private slots: void AbstractObjectChecked(int _enable);
	private slots: void TranslatableObjectChecked(int _enable);
	private slots: void ActivatableObjectChecked(int _enable);
	private slots: void ScalableObjectChecked(int _enable);
	private slots: void RotatableObjectChecked(int _enable);
	private slots: void PlayingObjectChecked(int _enable);
	private slots: void LogicObjectChecked(int _enable);
	private slots: void LerpingObjectChecked(int _enable);
	private slots: void ActionChecked(int _enable);
	private slots: void TriggerChecked(int _enable);
	private slots: void MeshChecked(int _enable);
	private slots: void HitboxChecked(int _enable);
	private slots: void EventChecked(int _enable);
	private slots: void VariableQuaternionChecked(int _enable);
	private slots: void VariableIntChecked(int _enable);
	private slots: void VariableFloatChecked(int _enable);
	private slots: void VariableBoolChecked(int _enable);
	private slots: void VariableStringChecked(int _enable);
	private slots: void VariableVector2Checked(int _enable);
	private slots: void VariableVector3Checked(int _enable);
	private slots: void AbstractVariableChecked(int _enable);
	private slots: void PlayerControlChecked(int _enable);
	private slots: void FloatLerpChecked(int _enable);
	private slots: void VectorLerpChecked(int _enable);
	private slots: void QuaternionLerpChecked(int _enable);
	private slots: void CameraChecked(int _enable);
	private slots: void ObjectSceneChecked(int _enable);
	private slots: void LogicSceneChecked(int _enable);
	private slots: void GraphicsSceneChecked(int _enable);
	private slots: void TiledSpace3DChecked(int _enable);
	private slots: void SceneChecked(int _enable);
	private slots: void TimerChecked(int _enable);
	private slots: void AbstractListChecked(int _enable);
	private slots: void TexturingSurfaceChecked(int _enable);
	private slots: void GraphicsSurfaceChecked(int _enable);
	private slots: void AbstractAnimationChecked(int _enable);
	private slots: void PlaneChecked(int _enable);
	private slots: void SetChecked(int _enable);
	private slots: void GraphicsStripChecked(int _enable);
	private slots: void PolygonalSurfaceChecked(int _enable);
	private slots: void AbstractSurfaceChecked(int _enable);
	private slots: void AtlasChecked(int _enable);
	private slots: void TexturedAnimationChecked(int _enable);
	private slots: void TextureChecked(int _enable);
	private: void UpdateSupportedTriggerList(void);
	public: QString GetAddToListAbstractObjectInfo(void);
	public: QString GetAddToListTranslatableObjectInfo(void);
	public: QString GetAddToListScalableObjectInfo(void);
	public: QString GetAddToListRotatableObjectInfo(void);
	public: QString GetChangeListValueInfo(void);
	public: QString GetChangeItemValueInfo(void);
	public: QString GetAddToListColorableObjectInfo(void);
	public: QString GetAddToListIntegerInfo(void);
	public: QString GetAddToListSetInfo(void);
	public: QString GetAddToListFloatInfo(void);
	public: QString GetAddToListBooleanInfo(void);
	public: QString GetAddToListStringInfo(void);
	public: QString GetAddToListVector2Info(void);
	public: QString GetAddToListVector3Info(void);
	public: QString GetAddToListQuaternionInfo(void);
	public: QString GetCopyToListInfo(void);
	public: QString GetDataFromListInfo(void);
};

#endif 

