#ifndef FLOATEDITOREX_H
#define FLOATEDITOREX_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/FloatSpinBox/FloatSpinBox.h>
#include <Editor/UniversalDelegate/TimeHMSEditor/TimeHMSEditor.h>
#include <Editor/UniversalDelegate/VirtualKeyEditor/VirtualKeyEditor.h>
#include <Editor/UniversalDelegate/AngleEditor/AngleEditor.h>
class FloatEditorEx: public QWidget
{
	Q_OBJECT
	private: QComboBox* constsComboBox;
	private: FloatSpinBox* floatValue;
	private: TimeHMSEditor* hmsEditor;
	private: AngleEditor* angleDegreesEditor;
	private: QComboBox* angleRadiansComboBox;
    public: FloatEditorEx(QWidget* _parent = 0);
	private: virtual void resizeEvent(QResizeEvent* _event);
	private slots: void FocusOut(void);
	signals: void FocusOut(QWidget* _widget);
	public: void Set_0_1_Mode(void);
	public: void Set_0_360_Mode(void);
	public: QString GetValueString(void)const;
	public: void SetValue(float _value);
	private: float GetAngleRadiansIndex(int _value);
	private slots: void PickAngleRadiansValue(const QString& _value);
	private slots: void PickConstValue(const QString& _value);
	private slots: void ChangeTimeValue(const QTime& _time);
	private slots: void ChangeAngleValue(double _value);
};



#endif 

