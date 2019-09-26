#ifndef QUATERNIONCREATOR_H
#define QUATERNIONCREATOR_H
#include <QtGui/QWidget>
#include <QDialog>
#include <Editor/AnimatedLabel/AnimatedLabel.h>
#include <Editor/Viewport/Viewport.h>
#include <Editor/QuaternionCreator/QuaternionViewer/QuaternionViewer.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
class QuaternionCreator: public QDialog 
{
	Q_OBJECT
	private: QTimer* intervalTimer;
	private: Viewport* viewport;
	private: QGroupBox* viewportGroupBox;
    private: QuaternionViewer* engine;
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
    private: QLabel *qDataLabel;
	private: FloatSpinBox *qxSpinBox;
    private: FloatSpinBox *qySpinBox;
    private: FloatSpinBox *qzSpinBox;
    private: FloatSpinBox *qwSpinBox;
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
    private: QPushButton *okButton;
    private: QPushButton *cancelButton;	

	private: QToolButton* helpButton;
    public: QuaternionCreator(QWidget* _parent = 0);
    public: virtual ~QuaternionCreator(void);
    protected: virtual void closeEvent(QCloseEvent *_event);
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
	public slots: void ShowGlobalAxises(int _state);
	public slots: void SelectRotationMode(QString _mode);
	public: void SetRelativeRotation(QString _text);
	public slots: void RightView(void);
	public slots: void FrontView(void);
	public slots: void TopDownView(void);
	private slots: void EulerAnglesIsChanged(double _value);
	private slots: void ChangeAngleFormat(QString _value);
	public slots: void AxisAngleIsChanged(double _value);
};

#endif 

