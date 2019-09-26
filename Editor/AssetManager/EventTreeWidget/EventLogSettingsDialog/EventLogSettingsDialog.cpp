#include "EventLogSettingsDialog.h"
#include <Editor/Editor/Editor.h>
#include <Editor/NameCreator/NameCreator.h>
#include <Editor/WarningDialog/WarningDialog.h>



EventLogSettingsDialog::EventLogSettingsDialog(QWidget* _parent): QDialog(_parent)
{
	logShowModeLabel = new QLabel(this);
    logShowModeLabel->setGeometry(QRect(20, 40-30, 61, 21));
	logShowModeLabel->setText("Log show mode:");
       
	logShowModeComboBox = new QComboBox(this);
    logShowModeComboBox->setGeometry(QRect(120, 40-30, 111, 22));
	logShowModeComboBox->addItem("POSTPONE");
	logShowModeComboBox->addItem("REAL_TIME");
	logShowModeComboBox->setCurrentIndex(0);
	showTriggersLabel = new QLabel(this);
    showTriggersLabel->setGeometry(QRect(20, 40, 61+20, 21));
	showTriggersLabel->setText("Show triggers:");
       
	showTriggersCheckBox = new QCheckBox(this);
    showTriggersCheckBox->setGeometry(QRect(120, 40, 111-60, 22));
	showTriggersCheckBox->setChecked(false);
	showActionsLabel = new QLabel(this);
    showActionsLabel->setGeometry(QRect(20, 40+30, 61+20, 21));
	showActionsLabel->setText("Show actions:");
       
	showActionsCheckBox = new QCheckBox(this);
    showActionsCheckBox->setGeometry(QRect(120, 40+30, 111-60, 22));
	showActionsCheckBox->setChecked(false);
	showEventsLabel = new QLabel(this);
    showEventsLabel->setGeometry(QRect(20, 40+30+30, 61+20, 21));
	showEventsLabel->setText("Show events:");
       
	showEventsCheckBox = new QCheckBox(this);
    showEventsCheckBox->setGeometry(QRect(120, 40+30+30, 111-60, 22));
	showEventsCheckBox->setChecked(false);
	line1 = new QFrame(this);
    line1->setGeometry(QRect(20, 100-30+30+20, 251+70, 20));
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
	eventsIgnoreListLabel = new QLabel(this);
    eventsIgnoreListLabel->setGeometry(QRect(100+40, 120+30+30, 101, 21));
	eventsIgnoreListLabel->setText("Events ignore list:");
   
	eventsIgnoreListWidget = new IgnoreEventList(this);
    eventsIgnoreListWidget->setGeometry(QRect(20, 140+30-30, 251+70, 411));
	removePushButton = new QToolButton(this);
    removePushButton->setGeometry(QRect(20, 560-5-30-30+30+30+30-30, 31, 23));
	QPixmap removeIcon = QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "AssetManager/remove.png");
	removePushButton->setIcon(removeIcon);
	removePushButton->setFixedSize(removeIcon.size());
	removePushButton->setToolTip("Remove");
	removePushButton->setAutoRaise(true);
	connect(removePushButton, SIGNAL(clicked()), this, SLOT(RemoveIsClicked()));
	clearPushButton = new QPushButton(this);
    clearPushButton->setGeometry(QRect(20+40, 560-30-30+30+30+30-30, 71, 23));
	clearPushButton->setText("Clear");
	connect(clearPushButton, SIGNAL(clicked()), this, SLOT(ClearIsClicked()));
	saveAsPushButton = new QPushButton(this);
    saveAsPushButton->setGeometry(QRect(20+40+80+20+10+10+5+5, 560-30+30+30-30, 71, 23));
	saveAsPushButton->setText("Save as...");
	connect(saveAsPushButton, SIGNAL(clicked()), this, SLOT(SaveAsIsClicked()));
	importPushButton = new QPushButton(this);
    importPushButton->setGeometry(QRect(20+40+80+20+80+20-15+10+10+5, 560-30+30+30-30, 71, 23));
	importPushButton->setText("Import");
	connect(importPushButton, SIGNAL(clicked()), this, SLOT(ImportIsClicked()));
	line2 = new QFrame(this);
    line2->setGeometry(QRect(20, 580+10, 251+70, 20));
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
	okButton = new QPushButton(this);
    okButton->setGeometry(QRect(110+70, 620, 75, 23));
	okButton->setText("Ok");
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(200+70, 620, 75, 23));
	cancelButton->setText("Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setFocus();

	
	setFixedSize(292+70, 654);

	setWindowTitle("Event Log Settings");
}


bool EventLogSettingsDialog::IsShowTriggersEnabled(void)const
{
	return showTriggersCheckBox->isChecked();
}


bool EventLogSettingsDialog::IsShowEventsEnabled(void)const
{
	return showEventsCheckBox->isChecked();
}


bool EventLogSettingsDialog::IsShowActionsEnabled(void)const
{
	return showActionsCheckBox->isChecked();
}



void EventLogSettingsDialog::SetTriggersShowEnable(bool _enable)
{
	showTriggersCheckBox->setChecked(_enable);
}


void EventLogSettingsDialog::SetEventsShowEnable(bool _enable)
{
	showEventsCheckBox->setChecked(_enable);
}


void EventLogSettingsDialog::SetActionsShowEnable(bool _enable)
{
	showActionsCheckBox->setChecked(_enable);
}



void EventLogSettingsDialog::SaveAsIsClicked(void)
{
	if(eventsIgnoreListWidget->count() > 0)
	{
		QString name = "ignore_list";

		QString pathFile = QFileDialog::getSaveFileName(this,
														"Save ignore list as...",
														Editor::_GetWorkDir() + Editor::_GetResDir() + QString("AssetManager/") + name,
														QString("*") + QString("igrlst"));

		if(!pathFile.isEmpty())
		{	
			name = File::_GetBaseName(pathFile.toStdString()).c_str();
			if(File::_IsExist(pathFile.toStdString() + ".igrlst"))
			{ 
				WarningDialog warning("Warning", "Ignore list '" + name + "' is already exist. Rewrite file?", 200, 90);
				
				if(warning.exec() == QDialog::Rejected)
				{
					return;
				}
			}

			
			File file(pathFile.toStdString() + ".igrlst", File::REWRITE);
			
			QList<QPair<int, QString>> eventIgnoreList = eventsIgnoreListWidget->GetIgnoreList();
			file.Write(eventIgnoreList.size());
			for(QList<QPair<int, QString>>::iterator it = eventIgnoreList.begin(); it != eventIgnoreList.end(); ++it)
			{
				file.Write((*it).first);
				file.Write((*it).second.toStdString().length());
				file.WriteString((*it).second.toStdString());
			}
		}
	}
	else
	{
		ErrorDialog("Error", "Ignore list is not defined", 170, 40);
	}
}


void EventLogSettingsDialog::ImportIsClicked(void)
{
	QFileDialog dialog(this, "Choose ignore list");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(QString("*.") + QString("igrlst"));
	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory(Editor::_GetWorkDir() + Editor::_GetResDir() + QString("AssetManager/"));


	if(dialog.exec())
	{
		QStringList assets = dialog.selectedFiles();
	
		if(assets.size() == 0)
		{
			return;
		}
		File file(assets.at(0).toStdString(), File::READ);

		if(file.IsOpened())
		{
			int size;
			file.Read(size);
			for(int i = 0; i < size; i++)
			{
				int category;
				file.Read(category);
				int length;
				StringANSI name;
				file.Read(length);
				file.ReadString(name, length);

				AddEvent(category, name.c_str());
			}
		}
	} 
}


void EventLogSettingsDialog::SetIgnoreList(QList<QPair<int, QString>>& _eventIgnoreList)
{
	for(int i = 0; i < _eventIgnoreList.size(); i++)
	{
		QPair<int, QString> it = _eventIgnoreList.at(i);

		AddEvent(it.first, it.second);
	}
}


QList<QPair<int, QString>> EventLogSettingsDialog::GetIgnoreList(void)const
{
	return eventsIgnoreListWidget->GetIgnoreList();
}



void EventLogSettingsDialog::SetLogShowMode(int _showMode)
{
	switch(_showMode)
	{
		case POSTPONE:
		{
			logShowModeComboBox->setCurrentIndex(0); 
			break;
		}

		case REAL_TIME:
		{
			logShowModeComboBox->setCurrentIndex(1); 
		}
	}
}


int EventLogSettingsDialog::GetLogShowMode(void)const
{
	if(logShowModeComboBox->itemText(logShowModeComboBox->currentIndex()) == "POSTPONE")
	{
		return POSTPONE;
	}
	else if(logShowModeComboBox->itemText(logShowModeComboBox->currentIndex()) == "REAL_TIME")
	{
		return REAL_TIME;
	}
	return -1;
}



void EventLogSettingsDialog::RemoveIsClicked(void)
{
	eventsIgnoreListWidget->RemoveSelectedItems();
}


void EventLogSettingsDialog::ClearIsClicked(void)
{
	eventsIgnoreListWidget->clear();
}


void EventLogSettingsDialog::AddEvent(int _category, QString _name)
{
	eventsIgnoreListWidget->AddEvent(_category, _name);
}

