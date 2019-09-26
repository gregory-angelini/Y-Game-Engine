#ifndef TRIGGERCREATOR_H
#define TRIGGERCREATOR_H
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
#include <Engine/Core/Trigger/Trigger.h>
#include <Editor/Spoiler/Spoiler.h>
#include <Editor/TriggerCreator/EventCreator/EventCreator.h>
#include <Editor/TriggerCreator/ActionCreator/ActionCreator.h>
#include <Editor/TriggerCreator/ActionsLibraryTree/ActionsLibraryTree.h>
#include <Editor/TriggerCreator/EventsLibraryTree/EventsLibraryTree.h>
#include <Editor/TriggerCreator/ActionsExistList/ActionsExistList.h>
#include <Editor/TriggerCreator/EventsExistList/EventsExistList.h>
#include <Editor/UniversalDelegate/LimitEditor/LimitEditor.h>
#include <Editor/UniversalDelegate/LoadingModeEditor/LoadingModeEditor.h>
#include <Editor/BoolSpinBox/BoolSpinBox.h>
#include <Editor/TriggerCreator/BooleanExprLineEdit/BooleanExprLineEdit.h>
#include <Editor/TriggerCreator/EnumerationExprLineEdit/EnumerationExprLineEdit.h>
class ActionLineEditEx: public EnumerationExprLineEdit
{
	public: ActionLineEditEx(QWidget* _parent);
	public: bool IsEmpty(QString _expr);
	public: bool OperationIsLast(QString _expr);
	protected: virtual void dragEnterEvent(QDragEnterEvent* _event);
	protected: virtual void dropEvent(QDropEvent* _event);
};
class EventLineEditEx: public BooleanExprLineEdit
{
	public: EventLineEditEx(QWidget* _parent);
	public: bool IsEmpty(QString _expr);
	public: bool OperationIsLast(QString _expr);
	protected: virtual void dragEnterEvent(QDragEnterEvent* _event);
	protected: virtual void dropEvent(QDropEvent* _event);
};
class TriggerCreator: public QDialog 
{
	private: class ListWidgetItemEx: public QListWidgetItem
    {
		public: enum
		{
			EVENT_TYPE = 1,
			ACTION_TYPE,
			TRIGGER_TYPE,
			VARIABLE_TYPE
		};
		private: int32 type;
		public: ListWidgetItemEx(void): type(0)
		{}
		public: void SetType(int32 _type)
		{
			type = _type;
		}
		public: int32 GetType(void)const
		{
			return type;
		}
    };
	friend class ActionsExistList;
	friend class EventsExistList;


	Q_OBJECT
	private: int nameMaxLength;
	private: QLabel* nameLabel;
    private: QLineEdit* nameLineEdit;
	private: QLabel* activationLimitLabel;
	private: LimitIntSpinBox* activationLimitSpinBox;
	private: QLabel* repeatLimitLabel;
	private: LimitIntSpinBox* repeatLimitSpinBox;
	private: QLabel* activatedLabel;
	private: BoolSpinBox* activatedSpinBox;
	private: QLabel* argsLoadModeLabel;
	private: LoadingModeIntSpinBox* loadingModeSpinBox;
	private: QLabel* deactivationModeLabel;
	private: QComboBox * deactivationModeComboBox;
	private: QLabel* inFakeLabel;
    private: QGridLayout* inLayout;
    private: QPushButton* refreshEventsLibraryPushButton;
    private: QLabel* inLabel;
    private: QLabel* inArgsCompleteLabel;
    private: EventLineEditEx* inLineEdit;
    private: EventsExistList* existEventsList;
    private: QLabel* eventsLibraryLabel;
    private: QPushButton* newEventPushButton;
    private: EventsLibraryTree* eventsLibraryTree;
	private: Spoiler* inSpoiler;
	private: BooleanExprParser eventParser;
    private: QLabel* outFakeLabel;
    private: QGridLayout* outLayout;
    private: QPushButton* refreshActionsLibraryPushButton;
    private: QLabel* outLabel;
    private: QLabel* outArgsCompleteLabel;
    private: ActionLineEditEx* outLineEdit;
    private: ActionsExistList* existActionsList;
    private: QLabel* actionsLibraryLabel;
    private: QPushButton* newActionPushButton;
    private: ActionsLibraryTree* actionsLibraryTree;
	private: Spoiler* outSpoiler;
	private: EnumerationExprParser actionParser;

	private: QPropertyAnimation* outSpoilerMovingAnimation;
	private: bool fastCreatingEnable;
    private: Trigger* trigger;


	private: QToolButton* helpButton;
    private: QPushButton* okButton;
    private: QPushButton* cancelButton;
	public: TriggerCreator(QWidget* _parent = 0);
	public: virtual ~TriggerCreator(void);
    private slots: void IsOk(void);
	private slots: void ShowDoc(void);
    public: Trigger* GetTrigger(void);
	public: void SetObjectName(QString _name);
	public: void SetNameDisable(bool _disable);
	private slots: void ClickOnINSpoiler(bool _checked);
	private slots: void ClickOnOUTSpoiler(bool _checked);
	public: void SetTrigger(QString _name);
	public: void SetFastCreating(bool _state);
	signals: void TriggerIsEdited(QString _name);
	public: int32 IndexOfDeactivationMode(QString _text);
	public: QString StringOfDeactivationMode(int32 _mode);
	private slots: void EventExprChanged(QString _text);
	private slots: void EventIsSelected(void);
	public slots: void RefreshEventsLibrary(void);
	public slots: void RefreshEventsLibrary(QString _name);
	public slots: void RefreshTriggersLibrary(QString _name);
	public slots: void RefreshVariablesLibrary(QString _name);
	public slots: void CreateNewEvent(void);
	private slots: void UpdateEventList(void);
	private slots: void ActionExprChanged(QString _text);
	private slots: void ActionIsSelected(void);
	public slots: void RefreshActionsLibrary(void);
	public slots: void RefreshActionsLibrary(QString _name);
	public slots: void CreateNewAction(void);
	private slots: void UpdateActionList(void);
};


#endif 

