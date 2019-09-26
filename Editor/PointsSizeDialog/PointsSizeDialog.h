#ifndef POINTSSIZEDIALOG_H
#define POINTSSIZEDIALOG_H
#include <QtGui/QWidget>
#include <QDialog>
#include <Editor/AnimatedLabel/AnimatedLabel.h>
#include <Editor/Viewport/Viewport.h>
#include <Engine/Output/File/File.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
class PointsSizeDialog: public QDialog
{
	Q_OBJECT
	private: FloatSpinBox *pointsSizeSpinBox;
    private: QLabel *pointsSizeLabel;

    private: QPushButton *cancelButton;
    private: QPushButton *okButton;
    public: PointsSizeDialog(QWidget* _parent = NIL);
    public: virtual ~PointsSizeDialog(void);
	public: void SetPointsSize(float _size);
	public: float GetPointsSize(void)const;
	public: static float _GetPointsSizeFromFile(void);
    private slots: void IsOk(void);
};


#endif 

