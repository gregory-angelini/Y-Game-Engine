
#include "..\..\Editor.h"


void Editor::CreateHelpMenu(void)
{
	helpMenu = menuBar()->addMenu("Help");
	showDocAct = new QAction("Manual", this);  
	connect(showDocAct, SIGNAL(triggered()), SLOT(ShowDoc()));
	helpMenu->addAction(showDocAct);
	showContactsAct = new QAction("Contact", this);  
	connect(showContactsAct, SIGNAL(triggered()), SLOT(ShowContacts()));
	helpMenu->addAction(showContactsAct);
}




void Editor::ShowDoc(void)
{
	QString path = (Editor::_GetWorkDir() + Editor::_GetDocsDir() + "CONTENTS.doc");

	QUrl url(path);

	QDesktopServices::openUrl(url);
}




void Editor::ShowContacts(void)
{
	QDialog dialog(0, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	dialog.setWindowTitle("Contacts");
	dialog.setFixedSize(260, 70);
		
	QLabel* labelContact = new QLabel(&dialog);
	labelContact->setWordWrap(true);
	labelContact->setText("<li><b>Developers</b>: Vovchok Grygoriy Konstantinovich</li>"
		                  "<li><b>E-mail</b>: grygoriy.vovchok@gmail.com</li>"
	                      "<li><b>Skype</b>: rubbish188</li>");
	QGridLayout* layout = new QGridLayout(&dialog);
	layout->addWidget(labelContact, 0, 0);
	dialog.setLayout(layout);

	dialog.exec();
}

