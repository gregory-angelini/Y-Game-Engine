#ifndef LINESSIZEDIALOG_H
#define LINESSIZEDIALOG_H
#include <QtGui/QWidget>
#include <QDialog>
#include <Editor/AnimatedLabel/AnimatedLabel.h>
#include <Editor/Viewport/Viewport.h>
#include <Engine/Output/File/File.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
class LinesWidthDialog: public QDialog
{
	Q_OBJECT
	private: FloatSpinBox *linesWidthSpinBox;
    private: QLabel *linesWidthLabel;

    private: QPushButton *cancelButton;
    private: QPushButton *okButton;
    public: LinesWidthDialog(QWidget* _parent = NIL);
    public: virtual ~LinesWidthDialog(void);
	public: void SetLinesWidth(float _size);
	public: float GetLinesWidth(void)const;
	public: static float _GetLinesWidthFromFile(void);
    private slots: void IsOk(void);
};


#endif 

