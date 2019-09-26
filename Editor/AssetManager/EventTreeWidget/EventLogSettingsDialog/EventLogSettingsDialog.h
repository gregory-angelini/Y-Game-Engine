#ifndef EVENTLOGSETTINGSDIALOG_H
#define EVENTLOGSETTINGSDIALOG_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/LimitIntSpinBox/LimitIntSpinBox.h>
#include <Editor/AssetManager\EventTreeWidget\EventLogSettingsDialog/IgnoreEventList/IgnoreEventList.h>
class EventLogSettingsDialog: public QDialog
{
	Q_OBJECT
	public: enum
	{
		POSTPONE,
		REAL_TIME
	};
	private: QLabel *logShowModeLabel;
    private: QComboBox *logShowModeComboBox;
	private: QLabel *showTriggersLabel;
    private: QCheckBox *showTriggersCheckBox;
	private: QLabel *showActionsLabel;
    private: QCheckBox *showActionsCheckBox;
	private: QLabel *showEventsLabel;
    private: QCheckBox *showEventsCheckBox;
	private: QLabel *eventsIgnoreListLabel;
    private: IgnoreEventList *eventsIgnoreListWidget;

    private: QToolButton *removePushButton;
    private: QPushButton *clearPushButton;
	private: QPushButton *saveAsPushButton;
    private: QPushButton *importPushButton;

    private: QFrame *line1;
    private: QFrame *line2;
    private: QPushButton *okButton;
    private: QPushButton *cancelButton;
	public: EventLogSettingsDialog(QWidget* _parent = 0);
	public: void SetLogShowMode(int _logShowMode);
	public: int GetLogShowMode(void)const;
	public: void SetTriggersShowEnable(bool _enable);
	public: void SetActionsShowEnable(bool _enable);
	public: void SetEventsShowEnable(bool _enable);
	public: bool IsShowTriggersEnabled(void)const;
	public: bool IsShowEventsEnabled(void)const;
	public: bool IsShowActionsEnabled(void)const;
	public: void AddEvent(int _category, QString _name);
	public: void SetIgnoreList(QList<QPair<int, QString>>& _eventIgnoreList);
	public: QList<QPair<int, QString>> GetIgnoreList(void)const;
	private slots: void SaveAsIsClicked(void);
	private slots: void ImportIsClicked(void);
	private slots: void ClearIsClicked(void);
	private slots: void RemoveIsClicked(void);
};


#endif 

