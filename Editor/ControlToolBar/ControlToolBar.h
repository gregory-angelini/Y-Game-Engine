#ifndef CONTROLTOOLBAR_H
#define CONTROLTOOLBAR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Editor/AssetManager/ObjectTreeWidget/ObjectTreeWidget.h>
#include <Editor/AssetManager/LogicTreeWidget/LogicTreeWidget.h>
#include <Editor/AssetManager/GraphicsTreeWidget/GraphicsTreeWidget.h>
class ControlToolBar: public QToolBar
{
	Q_OBJECT
	public: enum
	{
		STOP,
		PLAY,
		PAUSE,
		SINGLE_STEP,
		RECORD
	};
	private: QAction* playAction;
    private: QToolButton* playToolButton;
	private: QIcon buttonPlayEnabled;
	private: QIcon buttonPlayDisabled;
    private: QAction* stopAction;
    private: QToolButton* stopToolButton;
	private: QIcon buttonStopEnabled;
	private: QIcon buttonStopDisabled;
    private: QAction* pauseAction;
    private: QToolButton* pauseToolButton;
	private: QIcon buttonPauseEnabled;
	private: QIcon buttonPauseDisabled;
    private: QAction* nextFrameAction;
    private: QToolButton* nextFrameToolButton;
	private: QIcon buttonNextEnabled;
	private: QIcon buttonNextDisabled;
    private: QAction* recordAction;
    private: QToolButton* recordToolButton;
	private: QIcon buttonRecordEnabled;
	private: QIcon buttonRecordDisabled;
    private: bool recordEnable;
    private: QAction* timeAction;
    private: QToolButton* timeToolButton;
	private: QIcon buttonTimeEnabled;
	private: QIcon buttonTimeDisabled;
    private: bool timeShowEnable;
    private: QAction* timeSpeedAction;
    private: QToolButton* timeSpeedToolButton;
    private: QMenu* timeSpeedContextMenu;
	private: QAction* timeSpeedX1Act;
	private: QAction* timeSpeedX2Act;
	private: QAction* timeSpeedX4Act;
	private: QAction* timeSpeedX8Act;
	private: QAction* timeSpeedX16Act;
    private: QAction* timeSpeedX05Act;
	private: QAction* timeSpeedX025Act;
	private: QAction* timeSpeedX0125Act;
	private: QAction* timeSpeedX00625Act;
	private: QIcon buttonTimeX1;
	private: QIcon buttonTimeX2;
	private: QIcon buttonTimeX4;
    private: QIcon buttonTimeX8;
    private: QIcon buttonTimeX16;
    private: QIcon buttonTimeX05;
    private: QIcon buttonTimeX025;
    private: QIcon buttonTimeX0125;
	private: QIcon buttonTimeX00625;
	private: float currentTimeSpeed;

    private: int currentTool;
	public: ControlToolBar(QWidget* _parent = 0);
	public: virtual ~ControlToolBar(void);
	public: int GetCurrentTool(void);
	public: int GetCurrentTool(void)const;
	private: void CreateTimeTool(void);
	private slots: void ToTime(void);
	signals: void ShowTimeToolIsChanged(void);
	public: bool IsTimeShowEnabled(void)const;
	private: void CreateTimeSpeedTool(void);
	private slots: void NextTimeSpeed(void);
	public: float GetCurrentTimeSpeed(void)const;
	signals: void TimeSpeedToolIsChanged(void);
	private slots: void UpdateTimeSpeedContextMenu(const QPoint& _point);
	private slots: void UpdateTimeSpeedContextMenu(void);
    private: void CreateTimeSpeedContextMenu(void);
	private slots: void TimeSpeedX1IsChecked(void);
	private slots: void TimeSpeedX2IsChecked(void);
	private slots: void TimeSpeedX4IsChecked(void);
	private slots: void TimeSpeedX8IsChecked(void);
	private slots: void TimeSpeedX16IsChecked(void);
	private slots: void TimeSpeedX05IsChecked(void);
	private slots: void TimeSpeedX025IsChecked(void);
	private slots: void TimeSpeedX0125IsChecked(void);
	private slots: void TimeSpeedX00625IsChecked(void);
    private: void CreatePlayTool(void);
	private slots: void ToPlay(void);
	signals: void PickPlayTool(void);
    private: void CreateStopTool(void);
	private slots: void ToStop(void);
	signals: void PickStopTool(void);
	private: void CreatePauseTool(void);
	private slots: void ToPause(void);
	signals: void PickPauseTool(void);
	private: void CreateNextTool(void);
	private slots: void ToNextFrame(void);
	signals: void PickNextTool(void);
	private: void CreateRecordTool(void);
	private slots: void ToRecord(void);
	private slots: void NextButtonTimeout(void);
	public: bool IsRecordEnabled(void)const;
	signals: void RecordToolIsDisabled(void);
    public: void DisableRecord(void);
};


#endif 

