
#include "Editor.h"
#include <Editor/WarningDialog/WarningDialog.h>



void Editor::closeEvent(QCloseEvent *_event)
{
	if(PreClose() == false)
	{ 
		_event->ignore();
		return;
	}

	EnginePause();
}




bool Editor::PreClose(void)
{
	WarningDialog dialog("Exit", "Do you really want to leave?", 250, 50);
	
	if(dialog.exec() == QDialog::Rejected)
	{
		return false;
	}

	_DestroyProcesses();
	return true;
}
