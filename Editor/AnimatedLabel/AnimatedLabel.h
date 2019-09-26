#ifndef ANIMATEDLABEL_H
#define ANIMATEDLABEL_H
#include <QDialog>
#include <QtGui>
#include <QMainWindow>
class AnimatedLabel: public QLabel
{
	Q_OBJECT 
	private: QList<QPixmap> frames;

	private: QTimer timer;
	private: float fps;
	private: int currentFrame;
    private: int frameWidth;
    private: int frameHeight;
	private: bool pause;
	public: AnimatedLabel(QWidget* _parent = 0);
	public: void SetFrames(QList<QPixmap> _frames);
    public: void SetFPS(float _fps);
	public: float GetFPS(void)const;
	public: void Pause(void);
	public: void Play(void);
	public: int GetAmountOfFrames(void);
	public: void SetCurrentFrame(int _index);
	public: int GetCurrentFrame(void)const;
	void SetFrameWidth(float _width);
	void SetFrameHeight(float _height);
	public: int GetFrameWidth(void);
	public: int GetFrameHeight(void);
	private slots: void UpdateAnim(void);
};


#endif 

