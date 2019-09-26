#ifndef VIRTUALKEYEDITOR_H
#define VIRTUALKEYEDITOR_H
#include <Editor/ErrorDialog/ErrorDialog.h>
class VirtualKeyEditor: public QPushButton
{	
	Q_OBJECT
	public: VirtualKeyEditor(QWidget* _widget);
	private slots: void OpenVirtualKeyboard(void);
	signals: void KeyboardViewerIsClosed(void);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
};


#endif 

