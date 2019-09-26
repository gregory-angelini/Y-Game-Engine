#ifndef QUATERNIONLERPCREATOR_H
#define QUATERNIONLERPCREATOR_H
#include <QtGui/QWidget>
#include <QDialog>
#include <Editor/AnimatedLabel/AnimatedLabel.h>
#include <Editor/Viewport/Viewport.h>
#include <Editor/QuaternionLerpCreator/QuaternionLerpViewer/QuaternionLerpViewer.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
class QuaternionLerpCreator: public QDialog 
{
	Q_OBJECT
	private: QTimer* intervalTimer;
	private: Viewport* viewport;
	private: QGroupBox* viewportGroupBox;
    private: QuaternionLerpViewer* engine;

	private: QGroupBox* viewGroupBox;
	private: QLabel *angleFormatLabel;
	private: QComboBox *angleFormatComboBox;
	private: QGroupBox *axisAngleGroupBox;
    private: FloatSpinBox *axisXSpinBox;
    private: FloatSpinBox *axisYSpinBox;
    private: FloatSpinBox *axisZSpinBox;
	private: FloatSpinBox *angleSpinBox;
    private: QLabel *axisLabel;
    private: QLabel *angleLabel;
	private: QGroupBox *eulerGroupBox;
    private: FloatSpinBox *pitchSpinBox;
    private: FloatSpinBox *yawSpinBox;
	private: FloatSpinBox *rollSpinBox;
    private: QLabel *pitchLabel;
    private: QLabel *yawLabel;
    private: QLabel *rollLabel;
    private: QLabel *qCurrentDataLabel;
	private: FloatSpinBox *qCurrentXSpinBox;
    private: FloatSpinBox *qCurrentYSpinBox;
    private: FloatSpinBox *qCurrentZSpinBox;
    private: FloatSpinBox *qCurrentWSpinBox;
	private: QLabel *showLocalSpinsLabel;
    private: QCheckBox *showLocalSpinsCheckBox;
	private: QLabel *showGlobalSpinsLabel;
    private: QCheckBox *showGlobalSpinsCheckBox;
    private: QLabel *showGlobalAxisesLabel;
    private: QCheckBox *showGlobalAxisesCheckBox;

	private: QComboBox *rotationModeComboBox;
	private: QLabel *rotationModeLabel;

	private: QLabel *currentZoomLabel;
    private: QLabel *relativeRotationLabel;
	private: QToolButton *homeButton;

	private: QPushButton* rightView;
	private: QPushButton* topDownView;
	private: QPushButton* frontView;
	private: int nameMaxLength;
	private: QLabel *nameLabel;
    private: QLineEdit *nameLineEdit;
	private: QGroupBox* floatLerpGroupBox;

    private: QPushButton* loadButton;
	private: QPushButton* editButton;
	
    private: QLabel *fLerpNameLabel;
	private: QLineEdit* fLerpNameLineEdit;
    private: QLabel *qStartDataLabel;
	private: FloatSpinBox *qStartXSpinBox;
    private: FloatSpinBox *qStartYSpinBox;
    private: FloatSpinBox *qStartZSpinBox;
    private: FloatSpinBox *qStartWSpinBox;
    private: QLabel *qTargetDataLabel;
	private: FloatSpinBox *qTargetXSpinBox;
    private: FloatSpinBox *qTargetYSpinBox;
    private: FloatSpinBox *qTargetZSpinBox;
    private: FloatSpinBox *qTargetWSpinBox;

	private: QPushButton* startQImportButton;
	private: QPushButton* targetQImportButton;


	private: QPushButton* startQButton;
	private: QPushButton* targetQButton;

	private: QPushButton* playButton;
	private: QPushButton* stopButton;
    private: bool fastCreatingEnable;
    private: QPushButton *okButton;
    private: QPushButton *cancelButton;	

	private: QToolButton* helpButton;
    public: QuaternionLerpCreator(QWidget* _parent = 0);
    public: virtual ~QuaternionLerpCreator(void);
	public: void SetFastCreating(bool _state);
	private slots: void LoadFloatLerp(void);
	private slots: void EditFloatLerp(void);
	private slots: void ReloadFloatLerp(QString _name);
    protected: virtual void closeEvent(QCloseEvent *_event);
	public: void SetNameDisable(bool _disable);
	public: void SetObjectName(QString _name);
	private: void CreateEngine(void);
	private: void CreateViewport(void);
	private slots: void CallFrame(void);
	public: void EnginePause(void);
	public: void EnginePlay(void);
	public: void SetQuaternion(Quaternion _quaternion, bool _modifySphere = true);
	public: void UpdateAxisAngle(Quaternion _quaternion);
	public: void UpdateEuler(Quaternion _quaternion);
	public: void ConvertAngle(QString _format);
	public: void ConvertEulers(QString _format);
	public: Quaternion GetQuaternion(void)const;
	private slots: void ShowDoc(void);
    private slots: void IsOk(void);
	public slots: void SetHome(void);
	public: Viewport* GetViewport(void)const;
	public slots: void ViewportCursorEnter(void);
	public slots: void ViewportCursorLeave(void);
	public: void SetZoomValue(int _zoom);
	public slots: void ShowGlobalSpins(int _state);
	public slots: void ShowLocalSpins(int _state);
	public slots: void ShowGlobalAxises(int _state);
	public slots: void SelectRotationMode(QString _mode);
	public: void SetRelativeRotation(QString _text);
	public slots: void RightView(void);
	public slots: void FrontView(void);
	public slots: void TopDownView(void);
	private slots: void EulerAnglesIsChanged(double _value);
	private slots: void ChangeAngleFormat(QString _value);
	public slots: void AxisAngleIsChanged(double _value);
	public: void SetQuaternionLerp(QString _name);
	public: QuaternionLerp* GetQuaternionLerp(void);
	public: void PrepareToPlay(void);
	public: void PrepareToPause(void);
	public slots: void StartQ(void);
	public slots: void TargetQ(void);
	public slots: void Stop(void);
	public slots: void Play(void);
	public: void SetStartQ(Quaternion _quaternion);
	public: void SetTargetQ(Quaternion _quaternion);
	public slots: void CommonFloatLerpEnabled(bool _enable);
	public slots: void PrivateFloatLerpEnabled(bool _enable);
	private slots: void QuaternionLerpNameIsChanged(QString _name);
	private slots: void FloatLerpNameIsChanged(QString _name);
	public slots: void StartQImport(void);
	public slots: void TargetQImport(void);

};

#endif 

