#ifndef LOADINGMODEEDITOR_H
#define LOADINGMODEEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/IntSpinBox/IntSpinBox.h>
class LoadingModeIntSpinBox: public QSpinBox
{
	Q_OBJECT
	public: LoadingModeIntSpinBox(QWidget* _parent);
	public: virtual QString textFromValue(int _value)const;
	public: QString ValueToString(int _value)const;
	public: int StringToValue(QString _value)const;
	public: QString GetValueString(void)const;
	public: void SetValue(QString _value);
};
class LoadingModeEditor: public QWidget
{
	Q_OBJECT
	private: LoadingModeIntSpinBox* intValue;
    public: LoadingModeEditor(QWidget* _parent = 0);
	private: virtual void resizeEvent(QResizeEvent* _event);
	public: QString GetValueString(void)const;
	public: void SetValue(int _value);
	public: void SetValue(QString _value);
};



#endif 

