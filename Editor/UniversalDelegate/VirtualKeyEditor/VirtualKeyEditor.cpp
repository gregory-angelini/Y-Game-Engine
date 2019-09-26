#include "VirtualKeyEditor.h"
#include <Editor/PlayerControlCreator/KeyboardViewport.h>



VirtualKeyEditor::VirtualKeyEditor(QWidget* _widget): QPushButton(_widget)
{
    connect(this, SIGNAL(clicked()), this, SLOT(OpenVirtualKeyboard()));
}


void VirtualKeyEditor::keyPressEvent(QKeyEvent* _event)
{
	if(_event->key() == Qt::Key_Space)
	{
		QPushButton::keyPressEvent(_event);
	}
	else if(_event->key() == Qt::Key_Return)
	{
	}
}

void VirtualKeyEditor::OpenVirtualKeyboard(void)
{
	KeyboardViewport* dialog = new KeyboardViewport(this);
 
	if(!text().isEmpty())
	{
		dialog->SetKeyId(Keyboard::_GetKeyId(text().toAscii().data()));
	}
	if(dialog->exec() == QDialog::Accepted)
	{}

	dialog->setAttribute(Qt::WA_DeleteOnClose);

	setText(Keyboard::_GetKeyName((Keyboard::Key)dialog->GetKeyId()).c_str());
	
	emit KeyboardViewerIsClosed();
}