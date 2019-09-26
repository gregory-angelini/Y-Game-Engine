#ifndef SPACEUNITSDIALOG_H
#define SPACEUNITSDIALOG_H
#include <QWidget>
#include <QDialog>
#include <Editor/AnimatedLabel/AnimatedLabel.h>
#include <Editor/Viewport/Viewport.h>
#include <Engine/Output/File/File.h>
class SpaceUnitsDialog: public QDialog
{
	Q_OBJECT
	private: QSpinBox *unitsSpinBox;
    private: QLabel *unitsLabel;

    private: QPushButton *cancelButton;
    private: QPushButton *okButton;
    public: SpaceUnitsDialog(QWidget* _parent = NIL);
    public: virtual ~SpaceUnitsDialog(void);
	public: void SetPixelsPerMeter(int32 _value);
	public: int32 GetPixelsPerMeter(void)const;
	public: static int32 _GetUnitsFromFile(void);
    private slots: void IsOk(void);
};


#endif 

