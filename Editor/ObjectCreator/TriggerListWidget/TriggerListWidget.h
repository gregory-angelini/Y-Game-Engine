#ifndef TRIGGERLISTWIDGET_H
#define TRIGGERLISTWIDGET_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <QtGui>
#include <QObject>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
class TriggerListWidgetItem: public QObject,
							 public QListWidgetItem
{
	Q_OBJECT
	private: QTimer timer;
    private: QList<QPixmap> frames;
	private: float fps;
	private: int currentFrame;
    private: int frameWidth;
    private: int frameHeight;
	private: bool pause;
	public: TriggerListWidgetItem(QListWidget* _parent = 0);
	public: void SetFrames(QList<QPixmap> _frames);
	public: void SetFPS(float _fps);
	public: float GetFPS(void)const;
	public: void Pause(void);
	public: void Play(void);
	void SetFrameWidth(float _width);
	void SetFrameHeight(float _height);
	public: int GetFrameWidth(void);
	public: int GetFrameHeight(void);
	public: void SetCurrentFrame(int _index);
	public: int GetCurrentFrame(void)const;
	public: int GetAmountOfFrames(void);
	private slots: void UpdateAnim(void);
};
class TriggerListWidget: public QListWidget
{
	Q_OBJECT
	private: QList<QPixmap> frames;
	public: TriggerListWidget(QWidget* _parent = 0);
	public: void SetFrames(QList<QPixmap> _frames);
	public slots: TriggerListWidgetItem* AddItem(QString _name, bool _animated = false);
};



#endif 

