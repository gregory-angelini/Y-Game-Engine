#ifndef PLAYERCONTROLCREATOR_H
#define PLAYERCONTROLCREATOR_H
#include <QtGui/QWidget>
#include <QDialog>
#include <Editor/AnimatedLabel/AnimatedLabel.h>
#include <Editor/PlayerControlCreator/PlayerControlEventListWidget/PlayerControlEventListWidget.h>
#include <Editor/Viewport/Viewport.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
class PlayerControlCreator: public QDialog 
{
	Q_OBJECT
	private: QLabel *nameLabel;
    private: QLineEdit *nameLineEdit;
	private: int nameMaxLength;
    private: QLabel *timingLabel;
	private: QDoubleSpinBox *timingSpinBox;
	private: float maxTiming;
    private: QLabel *modeLabel;
    private: QComboBox *modeComboBox;
    private: QGroupBox *eventGroupBox;
    private: PlayerControlEventListWidget *eventListWidget;
	private: bool fastCreatingEnable;
    private: QPushButton *okButton;
    private: QPushButton *cancelButton;
		    
    private: QPushButton *addEventButton;
    private: QPushButton *removeEventButton;

	private: QToolButton* helpButton;
	private: PlayerControl* control;
    public: PlayerControlCreator(QWidget* _parent = 0);
    public: virtual ~PlayerControlCreator(void);
	public: void SetFastCreating(bool _state);
	public: void SetNameDisable(bool _disable);
	public: QString GetObjectName(void)const;
	private: void CreatePlayerControl(void);
	private: int GetKeyboardEventType(QString _eventType);
	private: int GetMouseEventKey(QString _eventKey);
	private: int GetMouseEventType(QString _eventType);
	private: PlayerControl::ActivationMode GetActivationMode(void);
	private: int GetActivationMode(PlayerControl::ActivationMode _mode);
	public: void SetPlayerControl(QString _name);
	public: PlayerControl* GetPlayerControl(void)const;
	private slots: void UpdateAmountOfEvents(void);
	private slots: void SelectNewMode(QString _mode);
	private slots: void ShowDoc(void);
    private slots: void IsOk(void);
	public: void SetObjectName(QString _name);
};

#endif 

