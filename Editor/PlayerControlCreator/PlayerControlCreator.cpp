#include "PlayerControlCreator.h"
#include <Engine/Output/File/File.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Editor/WarningDialog/WarningDialog.h>


PlayerControlCreator::PlayerControlCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), fastCreatingEnable(false)
{
	control = new PlayerControl();
	nameLabel = new QLabel(this); 
	nameLabel->setGeometry(QRect(20, 30, 31, 21));
	nameLabel->setText("Name:");

	nameMaxLength = 256;
   
	nameLineEdit = new QLineEdit(this);
    nameLineEdit->setGeometry(QRect(90, 30, 121, 20));
	nameLineEdit->setMaxLength(nameMaxLength);
	nameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");


	maxTiming =	1000.0f;
	timingLabel = new QLabel(this);
    timingLabel->setGeometry(QRect(20, 70, 41, 21));
	timingLabel->setText("Timing:");

	timingSpinBox = new QDoubleSpinBox(this);
    timingSpinBox->setGeometry(QRect(90, 70, 71, 22));
	timingSpinBox->setValue(control->GetTimingInSec());
	timingSpinBox->setToolTip("0 - " + QString::number(maxTiming) + " (sec)");
	timingSpinBox->setDecimals(6);
	timingSpinBox->setRange(0, maxTiming);
	timingSpinBox->setSingleStep(0.1);
	modeLabel = new QLabel(this);
    modeLabel->setGeometry(QRect(20, 110, 61, 21));
	modeLabel->setText("Mode:");
       
	modeComboBox = new QComboBox(this);
    modeComboBox->setGeometry(QRect(90, 110, 101, 22));
	modeComboBox->addItem("SINGLE");
	modeComboBox->addItem("SEQUENCE");
	modeComboBox->addItem("COMBO");
	connect(modeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(SelectNewMode(QString)));
	SelectNewMode("SINGLE");
	eventGroupBox = new QGroupBox(this);
    eventGroupBox->setGeometry(QRect(240, 20, 321+100+25+5-90+80+20, 251+50-5));
	eventGroupBox->setTitle("Events (" + QString::number(0) + "):");
        
	eventListWidget = new PlayerControlEventListWidget(eventGroupBox);
    eventListWidget->setGeometry(QRect(10, 20, 301+100+25+5-90+80+20, 221));
	connect(eventListWidget, SIGNAL(UpdateEvents()), this, SLOT(UpdateAmountOfEvents()));
	addEventButton = new QPushButton(eventGroupBox);
    addEventButton->setGeometry(QRect(30-20, 221+30, 81, 31));
	addEventButton->setText("Add");
	connect(addEventButton, SIGNAL(clicked()), eventListWidget, SLOT(NewEvent()));
	removeEventButton = new QPushButton(eventGroupBox);
    removeEventButton->setGeometry(QRect(30+81+20-20, 221+30, 81, 31));
	removeEventButton->setText("Remove");
	connect(removeEventButton, SIGNAL(clicked()), eventListWidget, SLOT(RemoveSelectedItems()));
	helpButton = new QToolButton(this);
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	helpButton->setGeometry(QRect(370+100+25+5-90-50+60+20, 310+30-5, 32, 32));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));
	okButton = new QPushButton(this);
    okButton->setGeometry(QRect(470+100+25+5-90+60+20, 310+30-5, 81, 31));
	okButton->setText("Ok");
	connect(okButton, SIGNAL(clicked()), this, SLOT(IsOk()));
	cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(370+100+25+5-90+60+20, 310+30-5, 81, 31));
	cancelButton->setText("Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	cancelButton->setFocus();

	setWindowTitle("Player Control Creator");
    setFixedSize(590+100+25+5-90+80+20, 364+15);
} 




void PlayerControlCreator::ShowDoc(void)
{
	QString path = (Editor::_GetWorkDir() + Editor::_GetDocsDir() + "PlayerControlEditor.doc");

	QUrl url = QUrl::fromLocalFile(path);

	QDesktopServices::openUrl(url);
}



void PlayerControlCreator::SelectNewMode(QString _mode)
{
	if(_mode == "SINGLE")
	{
		timingSpinBox->setEnabled(false);
	}
	else if(_mode == "COMBO" || _mode == "SEQUENCE")
	{
		timingSpinBox->setEnabled(true);
	}
}

void PlayerControlCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}



PlayerControlCreator::~PlayerControlCreator(void)
{
}


void PlayerControlCreator::SetPlayerControl(QString _name)
{
	delete control; 
	control = NIL;

	if(control = dynamic_cast<PlayerControl*>(AssetLibrary::_LoadPrivateAsset<PlayerControl>(_name.toAscii().data())))
	{
		nameLineEdit->setText(control->GetName().c_str());
		timingSpinBox->setValue(control->GetTimingInSec());
		modeComboBox->setCurrentIndex(GetActivationMode(control->GetActivationMode()));
		for(int i = 0; i < control->GetEvents().GetSize(); ++i)
		{ 
			eventListWidget->NewEvent(control->GetEvents()[i]);
		} 
	}
	else
	{
		control = new PlayerControl();
	}
	
	UpdateAmountOfEvents();

	setWindowTitle("Player Control Editor");
}



void PlayerControlCreator::UpdateAmountOfEvents(void)
{
    QList<QTreeWidgetItem*> list;
	eventListWidget->GetEventList(list);
	eventGroupBox->setTitle("Events (" + QString::number(list.size()) + "):");
}


int PlayerControlCreator::GetMouseEventType(QString _eventType)
{
	if(_eventType == "KEY_DOWN") return Mouse::KEY_DOWN;
	if(_eventType == "KEY_UP") return Mouse::KEY_UP;
	if(_eventType == "DOUBLE_CLICK") return Mouse::DBL_CLICK;
	if(_eventType == "MOVE") return Mouse::MOVE;
	if(_eventType == "WHEEL") return Mouse::WHEEL;
	return 0;
}


int PlayerControlCreator::GetMouseEventKey(QString _eventKey)
{
	if(_eventKey == "LEFT_BUTTON") return Mouse::LEFT_BUTTON;
	if(_eventKey == "RIGHT_BUTTON") return Mouse::RIGHT_BUTTON;
	if(_eventKey == "MIDDLE_BUTTON") return Mouse::MIDDLE_BUTTON;
	return 0;
}



int PlayerControlCreator::GetActivationMode(PlayerControl::ActivationMode _mode)
{
	if(_mode == PlayerControl::SINGLE)
		return 0;
	else if(_mode == PlayerControl::SEQUENCE)
		return 1;
		return 2;
}

PlayerControl::ActivationMode PlayerControlCreator::GetActivationMode(void)
{
	if(modeComboBox->itemText(modeComboBox->currentIndex()) == "SINGLE")
		return PlayerControl::SINGLE;
	else if(modeComboBox->itemText(modeComboBox->currentIndex()) == "SEQUENCE")
		return PlayerControl::SEQUENCE;
		return PlayerControl::COMBO;
}




void PlayerControlCreator::CreatePlayerControl(void)
{
	control->Destroy();
	control->Rename(nameLineEdit->text().toAscii().data());
	control->SetTimingInSec(timingSpinBox->value());
	control->SetActivationMode(GetActivationMode());
	QList<QTreeWidgetItem*> eventList;
	eventListWidget->GetEventList(eventList);
	for(int i = 0; i < eventList.size(); ++i)
	{  
		PlayerControlEventListWidget::ItemInputEvent* item = dynamic_cast<PlayerControlEventListWidget::ItemInputEvent*>(eventList.at(i));

		if(item->text(PlayerControlEventListWidget::ItemInputEvent::DEVICE_PROPERTY) == "KEYBOARD")
		{
			Keyboard::Event* newEvent = new Keyboard::Event();
			newEvent->SetEventType((Keyboard::EventType)GetKeyboardEventType(item->text(PlayerControlEventListWidget::ItemInputEvent::TYPE_PROPERTY)));
			newEvent->SetKey((Keyboard::Key)Keyboard::_GetKeyId(item->text(PlayerControlEventListWidget::ItemInputEvent::KEY_PROPERTY).toAscii().data()));
			control->AddEvent(newEvent);
		}
		else if(item->text(PlayerControlEventListWidget::ItemInputEvent::DEVICE_PROPERTY) == "MOUSE")
		{
			if(item->text(PlayerControlEventListWidget::ItemInputEvent::TYPE_PROPERTY) == "KEY_DOWN" |
			   item->text(PlayerControlEventListWidget::ItemInputEvent::TYPE_PROPERTY) == "KEY_UP" |
			   item->text(PlayerControlEventListWidget::ItemInputEvent::TYPE_PROPERTY) == "DOUBLE_CLICK")
			{		
				Mouse::Event* newEvent = new Mouse::Event();
				newEvent->SetEventType((Mouse::EventType)GetMouseEventType(item->text(PlayerControlEventListWidget::ItemInputEvent::TYPE_PROPERTY)));
				newEvent->SetKey((Mouse::Key)GetMouseEventKey(item->text(PlayerControlEventListWidget::ItemInputEvent::KEY_PROPERTY).toAscii().data()));
				control->AddEvent(newEvent);
			}
			else
			{ 
				Mouse::Event* newEvent = new Mouse::Event();
				newEvent->SetEventType((Mouse::EventType)GetMouseEventType(item->text(PlayerControlEventListWidget::ItemInputEvent::TYPE_PROPERTY)));
				control->AddEvent(newEvent);
			}
		}  
	}
}


void PlayerControlCreator::SetNameDisable(bool _disable)
{
	nameLineEdit->setDisabled(_disable);
}



QString PlayerControlCreator::GetObjectName(void)const
{
	return nameLineEdit->text();
}



PlayerControl* PlayerControlCreator::GetPlayerControl(void)const
{
	return control;
}


void PlayerControlCreator::SetObjectName(QString _name)
{
	nameLineEdit->setText(_name);
}



int PlayerControlCreator::GetKeyboardEventType(QString _eventType)
{
	if(_eventType == "KEY_DOWN") return Keyboard::KEY_DOWN;
	if(_eventType == "KEY_UP") return Keyboard::KEY_UP;
	return 0;
}
	
void PlayerControlCreator::IsOk(void)
{
	{
		int32 errorCode;
		if(nameLineEdit->text().isEmpty())
		{
			ErrorDialog("Error", "Player control name is not defined", 185, 40);
			return;
		}
		if((errorCode = BooleanExprParser::_IsNameValid(nameLineEdit->text().toAscii().data())) >= 0)
		{
			ErrorDialog("Error", "Player control name is not valid: '" + QString(nameLineEdit->text().at(errorCode)) + "'", 190, 40);
			return;
		} 
	}
	
	QList<QTreeWidgetItem*> eventList;
	eventListWidget->GetEventList(eventList);
	if(eventList.size() == 0)
	{
		ErrorDialog("Error", "Events is not defined", 135, 40);
		return;
	}
	if(modeComboBox->itemText(modeComboBox->currentIndex()) == "COMBO" ||
	   modeComboBox->itemText(modeComboBox->currentIndex()) == "SEQUENCE")
	{
		if(eventList.size() < 2)
		{
			ErrorDialog("Error", modeComboBox->itemText(modeComboBox->currentIndex()) + ": it requires at least 2 events", 215, 40);
			return;
		}
	}
	for(int i = 0; i < eventList.size(); ++i)
	{
		PlayerControlEventListWidget::ItemInputEvent* item = dynamic_cast<PlayerControlEventListWidget::ItemInputEvent*>(eventList.at(i));

		if(!item->IsValid())
		{
			ErrorDialog("Error", "event " + QString::number(i+1) + " is not valid", 190, 40);
			return;
		}
	}

	CreatePlayerControl();
	if(File::_IsExist(AssetLibrary::_GetDir() + PlayerControl::_GetDir() + StringANSI(nameLineEdit->text().toAscii().data()) + "." + PlayerControl::_GetFileSuffix()))
	{
		WarningDialog warning("Warning", "Player control '" +  nameLineEdit->text() + "' is already exist. Rewrite file?", 200, 90);
		
		if(warning.exec() == QDialog::Rejected)
		{
			return;
		}
	}

	if(!fastCreatingEnable)
	{

		control->SaveToFile();

		delete control;
		control = NIL;
	}
	else
	{
		if(AssetLibrary::_GetAssets().AddObject(control) == NIL)
		{
			ErrorDialog("Error", "Name '" + nameLineEdit->text() + "' is already reserved", 195, 40);
			return;
		}
	}

	accept();
}

