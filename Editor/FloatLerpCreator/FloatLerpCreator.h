#ifndef FLOATLERPCREATOR_H
#define FLOATLERPCREATOR_H
#include <QtGui/QWidget>
#include <QDialog>
#include <Editor/AnimatedLabel/AnimatedLabel.h>
#include <Editor/Viewport/Viewport.h>
#include <Editor/FloatLerpCreator/FloatLerpViewer/FloatLerpViewer.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
#include <Editor/IntSpinBox/IntSpinBox.h>
class FloatLerpCreator: public QDialog 
{
	Q_OBJECT
	private: QTimer* intervalTimer;
	private: Viewport* viewport;
	private: QGroupBox* viewportGroupBox;
    private: FloatLerpViewer* engine;
	private: int nameMaxLength;
	private: QLabel *nameLabel;
    private: QLineEdit *nameLineEdit;
	private: QLabel *targetLabel;
	private: FloatSpinBox *targetSpinBox;
	private: QLabel *currentLabel;
	private: FloatSpinBox *currentSpinBox;
	private: QLabel *deltaLabel;
	private: FloatSpinBox *deltaSpinBox;
	private: QLabel *tLabel;
	private: FloatSpinBox *tSpinBox;
	private: QLabel *lerpModeLabel;
	private: QComboBox *lerpModeComboBox;
	private: QLabel *playingSpeedLabel;
	private: FloatSpinBox *playingSpeedSpinBox;
	private: QLabel *playingDurationLabel;
	private: FloatSpinBox *playingDurationSpinBox;
	private: QGroupBox* playingModeGroupBox;
    private: QRadioButton* forwardRadioButton;
    private: QRadioButton* backwardRadioButton;
    private: QRadioButton* pingPongRadioButton;
    private: QRadioButton* onewayRadioButton;
    private: QRadioButton* loopRadioButton;
    private: QRadioButton* singleshotRadioButton;
	private: QLabel *showScaleLabel;
	private: QCheckBox *showScaleCheckBox;
	
    private: QLabel *scaleXLabel;
	private: FloatSpinBox *scaleXSpinBox;

	private: QLabel *scaleYLabel;
	private: FloatSpinBox *scaleYSpinBox;
    private: QLabel *currentZoomLabel;
    private: QToolButton *homeButton;
	
	private: QPushButton *playButton;
    private: QPushButton *pauseButton;
	private: bool fastCreatingEnable;
    private: QPushButton *okButton;
    private: QPushButton *cancelButton;	
	private: QToolButton* helpButton;
    public: FloatLerpCreator(QWidget* _parent = 0);
    public: virtual ~FloatLerpCreator(void);
	public: void SetFastCreating(bool _state);
	private slots: void ForwardPlayingModeEnabled(bool _enable);
	private slots: void BackwardPlayingModeEnabled(bool _enable);
	private slots: void PingPongPlayingModeEnabled(bool _enable);
	private slots: void OnewayPlayingModeEnabled(bool _enable);
	private slots: void LoopPlayingModeEnabled(bool _enable);
	private slots: void SingleshotPlayingModeEnabled(bool _enable);
    protected: virtual void closeEvent(QCloseEvent *_event);
	private: void CreateEngine(void);
	private: void CreateViewport(void);
	private slots: void CallFrame(void);
	public: void EnginePause(void);
	public: void EnginePlay(void);
	public: QString GetObjectName(void)const;
	public: void SetFloatLerp(QString _name);
	public: FloatLerp* GetFloatLerp(void)const;
	private slots: void ShowDoc(void);
    private slots: void IsOk(void);
	public slots: void SetHome(void);
	public: Viewport* GetViewport(void)const;
	public slots: void ViewportCursorEnter(void);
	public slots: void ViewportCursorLeave(void);
	public: void SetZoomValue(int _zoom);
	public: float GetZoom100(void)const;
	public: void SetObjectName(QString _name);
	public: void SetNameDisable(bool _disable);
	public slots: void TargetIsChanged(double _target);
	public slots: void PlayingSpeedIsChanged(double _playingSpeed);
	public slots: void LerpModeIsChanged(QString _mode);
	public slots: void PlayLerp(void);
	void StopLerp(void);
	public: void PrepareToPlay(void);
	public: void PrepareToPause(void);
	public slots: void ShowScaleIsChanged(int _enable);
	public slots: void ScaleXIsChanged(double _value);
	public slots: void ScaleYIsChanged(double _value);
	public slots: void UpdatePlayingSpeed(void);
	public slots: void UpdateCurrent(void);
	public slots: void DurationIsChanged(double _value);
	public: bool IsCurrentLerpMode(int32 _mode);
};

#endif 

