#include "TriggerCreator.h"
#include <Editor/NameCreator/NameCreator.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/WarningDialog/WarningDialog.h>




TriggerCreator::TriggerCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), fastCreatingEnable(false), trigger(NIL)
{
	nameLabel = new QLabel(this);
    nameLabel->setGeometry(QRect(20+250-240+80-60-20-10, 40+3-30+5-20+15, 41, 21));
	nameLabel->setText("Name:");

	nameMaxLength = 256;
   
	nameLineEdit = new QLineEdit(this);
    nameLineEdit->setGeometry(QRect(85+230-240+80-60-20-10, 40+3-30+5-20+15, 100+60+60-30+10+10+7-30+40, 20));
	nameLineEdit->setMaxLength(nameMaxLength);
	nameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	activationLimitLabel = new QLabel(this);
    activationLimitLabel->setGeometry(QRect(20+230+160+20+85-240+10+80-30+10-50-10-10+40, 40+3-30+5-20+15, 61+20, 21));
	activationLimitLabel->setText("Activation limit:");

	activationLimitSpinBox = new LimitIntSpinBox(this);
	activationLimitSpinBox->setGeometry(QRect(85+230+160+20+90-240+20+10+80-30-10+10-50-10-10+40, 40+3-30+5-20+15, 100, 20));
	activationLimitSpinBox->setValue(-1);
	repeatLimitLabel = new QLabel(this);
    repeatLimitLabel->setGeometry(QRect(20+230+160+20+85-240+10+80-30+10-50-10-10+165+30+10+40, 40+3-30+5-20+15, 61+20, 21));
	repeatLimitLabel->setText("Repeat limit:");

	repeatLimitSpinBox = new LimitIntSpinBox(this);
	repeatLimitSpinBox->setGeometry(QRect(85+230+160+20+90-240+20+10+80-30-10+10-50-10-10+150+30+10+40, 40+3-30+5-20+15, 100, 20));
	repeatLimitSpinBox->setValue(0);
	activatedLabel = new QLabel(this);
    activatedLabel->setGeometry(QRect(20+230+160+20+85+205-240+20+15+10+80-30+16-30+50+60+40, 40+3-30+5-20+15, 61+10+10, 21));
	activatedLabel->setText("Activation state:");

	activatedSpinBox = new BoolSpinBox(this);
	activatedSpinBox->setGeometry(QRect(85+230+160+20+85+205-240+20+10+10+10+15+10+80-30-10+16-30+50+60+40, 40+3-30+5-20+15, 100-40, 20));
	activatedSpinBox->setValue(false);
	deactivationModeLabel = new QLabel(this);
    deactivationModeLabel->setGeometry(QRect(20+230+160+20+85+205-240+20+15+10+80+100+150-80-30+35+60+40, 40+3-30+5-20+15, 61+10+10+15, 21));
	deactivationModeLabel->setText("Deactivation mode:");

	deactivationModeComboBox = new QComboBox(this);
	deactivationModeComboBox->setGeometry(QRect(85+230+160+20+85+205-240+20+10+10+10+15+10+80+100+150-80-30+4+35+60+40, 40+3-30+5-20+15, 100+30, 20));
	deactivationModeComboBox->addItem("NEVER_SWITCH_OFF");
	deactivationModeComboBox->addItem("AUTO_SWITCH_OFF");
	argsLoadModeLabel = new QLabel(this);
    argsLoadModeLabel->setGeometry(QRect(20+230+160+20+85+205-240+20+15+10+80+100+150+150+6+20+60+40, 40+3-30+5-20+15, 61+10+10+15, 21));
	argsLoadModeLabel->setText("Args loading mode:");


	loadingModeSpinBox = new LoadingModeIntSpinBox(this);
	loadingModeSpinBox->setGeometry(QRect(85+230+160+20+85+205-240+20+10+10+10+15+10+80+100+150+150+4+6+20+60+40, 40+3-30+5-20+15, 100-30+10+10, 20));
	loadingModeSpinBox->SetValue("AUTO");
	inSpoiler = new Spoiler("Events", 300, this);
	inSpoiler->setFixedSize(1200+100, 650);
	inSpoiler->move(20, 45);
	inSpoiler->SetToggleButtonFixedSize(1200+100, 25);
    inLayout = new QGridLayout(inSpoiler);
    inLayout->setContentsMargins(0, 0, 0, 0);
	inLayout->setSpacing(5);
	inFakeLabel = new QLabel(inSpoiler);
    inLayout->addWidget(inFakeLabel, 0, 0, 1, 1);
    inLabel = new QLabel(inSpoiler);
    inLabel->setText("IN:");
    inLayout->addWidget(inLabel, 1, 0, 1, 1, Qt::AlignHCenter);
    inLineEdit = new EventLineEditEx(inSpoiler);
	inLineEdit->resize(1134+100, 30); 
	connect(inLineEdit, SIGNAL(textChanged(QString)), SLOT(EventExprChanged(QString)));
    inLayout->addWidget(inLineEdit, 1, 1, 1, 18);
    inArgsCompleteLabel = new QLabel(inSpoiler);
    inArgsCompleteLabel->setText("Arguments complete: 0/0");
    inLayout->addWidget(inArgsCompleteLabel, 2, 1, 1, 7, Qt::AlignHCenter);
    existEventsList = new EventsExistList(inSpoiler);
	existEventsList->setFixedSize(500+40, 520-30);
	connect(existEventsList, SIGNAL(EventIsEdited(QString)), this, SLOT(RefreshEventsLibrary(QString)));
    connect(existEventsList, SIGNAL(TriggerIsEdited(QString)), this, SLOT(RefreshTriggersLibrary(QString)));
	connect(existEventsList, SIGNAL(VariableIsEdited(QString)), this, SLOT(RefreshVariablesLibrary(QString)));
	connect(existEventsList, SIGNAL(itemSelectionChanged()), this, SLOT(EventIsSelected()));
    inLayout->addWidget(existEventsList, 3, 0, 20, 9);
	eventsLibraryLabel = new QLabel(inSpoiler);
    eventsLibraryLabel->setText("Events library:");
    inLayout->addWidget(eventsLibraryLabel, 2, 11, 1, 7, Qt::AlignHCenter);
    eventsLibraryTree = new EventsLibraryTree(inSpoiler);
	eventsLibraryTree->setFixedSize(620+50, 520-30);
	connect(eventsLibraryTree, SIGNAL(EventIsEdited(QString)), this, SLOT(RefreshEventsLibrary(QString)));
    connect(eventsLibraryTree, SIGNAL(TriggerIsEdited(QString)), this, SLOT(RefreshTriggersLibrary(QString)));
	connect(eventsLibraryTree, SIGNAL(VariableIsEdited(QString)), this, SLOT(RefreshVariablesLibrary(QString)));
	inLayout->addWidget(eventsLibraryTree, 3, 10, 20, 9);
    newEventPushButton = new QPushButton(inSpoiler);
    newEventPushButton->setText("New event");
	newEventPushButton->setFocusPolicy(Qt::NoFocus);
	connect(newEventPushButton, SIGNAL(clicked()), this, SLOT(CreateNewEvent()));
    inLayout->addWidget(newEventPushButton, 5, 9, 1, 1);
	refreshEventsLibraryPushButton = new QPushButton(inSpoiler);
    refreshEventsLibraryPushButton->setText("Refresh");
	refreshEventsLibraryPushButton->setFocusPolicy(Qt::NoFocus);
	connect(refreshEventsLibraryPushButton, SIGNAL(clicked()), this, SLOT(RefreshEventsLibrary()));
    inLayout->addWidget(refreshEventsLibraryPushButton, 20+1, 9, 1, 1);


	inSpoiler->SetContentLayout(inLayout);
	outSpoiler = new Spoiler("Actions", 300, this);
	outSpoiler->setFixedSize(1200+100, 650);
	outSpoiler->move(20, 75);
	outSpoiler->SetToggleButtonFixedSize(1200+100, 25);
	outSpoiler->SetCollapseEnable(false);
    outLayout = new QGridLayout(outSpoiler);
    outLayout->setContentsMargins(0, 0, 0, 0);
	outLayout->setSpacing(5);
	outFakeLabel = new QLabel(outSpoiler);
    outLayout->addWidget(outFakeLabel, 0, 0, 1, 1);
    outLabel = new QLabel(outSpoiler);
    outLabel->setText("OUT:");
    outLayout->addWidget(outLabel, 1, 0, 1, 1, Qt::AlignHCenter);
    outLineEdit = new ActionLineEditEx(outSpoiler);
	outLineEdit->resize(1134+100, 30); 
	connect(outLineEdit, SIGNAL(textChanged(QString)), SLOT(ActionExprChanged(QString)));
    outLayout->addWidget(outLineEdit, 1, 1, 1, 18);
    outArgsCompleteLabel = new QLabel(outSpoiler);
    outArgsCompleteLabel->setText("Arguments complete: 0/0");
    outLayout->addWidget(outArgsCompleteLabel, 2, 1, 1, 7, Qt::AlignHCenter);
    existActionsList = new ActionsExistList(outSpoiler);
	existActionsList->setFixedSize(500+40, 520-30);
	connect(existActionsList, SIGNAL(ActionIsEdited(QString)), this, SLOT(RefreshActionsLibrary(QString)));
    connect(existActionsList, SIGNAL(itemSelectionChanged()), this, SLOT(ActionIsSelected()));
    outLayout->addWidget(existActionsList, 3, 0, 20, 9);
	actionsLibraryLabel = new QLabel(outSpoiler);
    actionsLibraryLabel->setText("Actions library:");
    outLayout->addWidget(actionsLibraryLabel, 2, 11, 1, 7, Qt::AlignHCenter);
    actionsLibraryTree = new ActionsLibraryTree(outSpoiler);
	actionsLibraryTree->setFixedSize(620+50, 520-30); 
	connect(actionsLibraryTree, SIGNAL(ActionIsEdited(QString)), this, SLOT(RefreshActionsLibrary(QString)));
    outLayout->addWidget(actionsLibraryTree, 3, 10, 20, 9);
    newActionPushButton = new QPushButton(outSpoiler);
    newActionPushButton->setText("New action");
	newActionPushButton->setFocusPolicy(Qt::NoFocus);
	connect(newActionPushButton, SIGNAL(clicked()), this, SLOT(CreateNewAction()));
    outLayout->addWidget(newActionPushButton, 5, 9, 1, 1);
	refreshActionsLibraryPushButton = new QPushButton(outSpoiler);
    refreshActionsLibraryPushButton->setText("Refresh");
	refreshActionsLibraryPushButton->setFocusPolicy(Qt::NoFocus);
	connect(refreshActionsLibraryPushButton, SIGNAL(clicked()), this, SLOT(RefreshActionsLibrary()));
    outLayout->addWidget(refreshActionsLibraryPushButton, 20+1, 9, 1, 1);
	
	outSpoiler->SetContentLayout(outLayout);
	connect(inSpoiler->GetToolButton(), SIGNAL(clicked(bool)), this, SLOT(ClickOnINSpoiler(bool)));
	connect(outSpoiler->GetToolButton(), SIGNAL(clicked(bool)), this, SLOT(ClickOnOUTSpoiler(bool)));
	outSpoilerMovingAnimation = new QPropertyAnimation(outSpoiler, "pos");
	outSpoilerMovingAnimation->setStartValue(QPoint(20, 75));
	outSpoilerMovingAnimation->setEndValue(QPoint(20, 75) + QPoint(0,583-30));
	outSpoilerMovingAnimation->setDuration(300);
    okButton = new QPushButton(this);
    okButton->setGeometry(QRect(500+210+150+200+30+80, 560+100+5+3, 91, 31));
	okButton->setText("Ok");
	okButton->setFocusPolicy(Qt::NoFocus);
	connect(okButton, SIGNAL(clicked()), this, SLOT(IsOk()));
    cancelButton = new QPushButton(this);
    cancelButton->setGeometry(QRect(390+210+150+200+30+80, 560+100+5+3, 91, 31));
	cancelButton->setText("Cancel");
	cancelButton->setFocusPolicy(Qt::NoFocus);
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	helpButton = new QToolButton(this);
	helpButton->setIcon(QIcon(Editor::_GetWorkDir() + Editor::_GetResDir() + "help.png"));
	helpButton->setAutoRaise(true);
	helpButton->setToolTip("Help");
	helpButton->setFocusPolicy(Qt::NoFocus);
	helpButton->setGeometry(QRect(390+210-50+150+200+30+80, 560+100+5+3, 32, 32));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(ShowDoc()));


	setFixedSize(614+210+150+50-5+120+100+100, 604+100+5+3);
	setWindowTitle("Trigger Creator");
}



TriggerCreator::~TriggerCreator(void)
{
}




QString TriggerCreator::StringOfDeactivationMode(int32 _mode)
{
	if(_mode == AbstractEvent::NEVER_SWITCH_OFF)
	{
		return "NEVER_SWITCH_OFF";
	}
	else if(_mode == AbstractEvent::AUTO_SWITCH_OFF)
	{
		return "AUTO_SWITCH_OFF";
	}
	return "";
}


int32 TriggerCreator::IndexOfDeactivationMode(QString _text)
{
	if(_text == "NEVER_SWITCH_OFF")
	{
		return AbstractEvent::NEVER_SWITCH_OFF;
	}
	else if(_text == "AUTO_SWITCH_OFF")
	{
		return AbstractEvent::AUTO_SWITCH_OFF;
	}
	return -1;
}



Trigger* TriggerCreator::GetTrigger(void)
{
	return trigger;
}


void TriggerCreator::ClickOnOUTSpoiler(bool _checked)
{
	if(_checked)
	{
		inSpoiler->SetCollapseEnable(true);
		outSpoilerMovingAnimation->setDirection(!_checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
	    outSpoilerMovingAnimation->start();
		outSpoiler->SetTitle("Actions"); 

		if(!inLineEdit->text().isEmpty())
		{
			inSpoiler->SetTitle(inLineEdit->text().replace("&", "&&"));
		}
		else
		{
			inSpoiler->SetTitle("Events"); 
		}
	}
	else
	{
		inSpoiler->SetCollapseEnable(false);
		outSpoilerMovingAnimation->setDirection(!_checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
	    outSpoilerMovingAnimation->start();
		inSpoiler->SetTitle("Events"); 

		if(!outLineEdit->text().isEmpty())
		{
			outSpoiler->SetTitle(outLineEdit->text().replace("&", "&&"));
		}
		else
		{
			outSpoiler->SetTitle("Actions"); 
		}
	}
}


void TriggerCreator::ClickOnINSpoiler(bool _checked)
{
	if(_checked)
	{
		outSpoiler->SetCollapseEnable(true);
		outSpoilerMovingAnimation->setDirection(_checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
	    outSpoilerMovingAnimation->start();
		inSpoiler->SetTitle("Events"); 

		if(!outLineEdit->text().isEmpty())
		{
			outSpoiler->SetTitle(outLineEdit->text().replace("&", "&&"));
		}
		else
		{
			outSpoiler->SetTitle("Actions"); 
		}
	}
	else
	{
		outSpoiler->SetCollapseEnable(false);
		outSpoilerMovingAnimation->setDirection(_checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
	    outSpoilerMovingAnimation->start();
		outSpoiler->SetTitle("Actions"); 

		if(!inLineEdit->text().isEmpty())
		{
			inSpoiler->SetTitle(inLineEdit->text().replace("&", "&&"));
		}
		else
		{
			inSpoiler->SetTitle("Events"); 
		}
	}
}




void TriggerCreator::ShowDoc(void)
{
	QString path = QString("editor\\docs\\") + "TriggerEditor.htm";

	QUrl url = QUrl(path);

	QDesktopServices::openUrl(url);
}




void TriggerCreator::SetObjectName(QString _name)
{
	nameLineEdit->setText(_name);
}
 


void TriggerCreator::SetNameDisable(bool _disable)
{
	nameLineEdit->setDisabled(_disable);
}


void TriggerCreator::SetFastCreating(bool _state)
{
	fastCreatingEnable = _state;
}



void TriggerCreator::SetTrigger(QString _name)
{ 
	File file(AssetLibrary::_GetDir() + Trigger::_GetDir() + _name.toAscii().data() + "." + Trigger::_GetFileSuffix(), File::READ);
	int32 version;
	file.Read(version);
	StringANSI triggerName;
	int32 length;
	file.Read(length);
	file.ReadString(triggerName, length);
	int32 activationLimit;
	file.Read(activationLimit);
	bool loadArgsEnable;
	file.Read(loadArgsEnable);
	int32 deactivationMode;
	file.Read(deactivationMode);
	int32 repeatLimit;
	file.Read(repeatLimit);
	bool activated;
	file.Read(activated);
	EnumerationExprParser actionExprParser;
	actionExprParser.LoadFromFile(file);
	BooleanExprParser eventExprParser;
	eventExprParser.LoadFromFile(file);
	nameLineEdit->setText(triggerName.c_str());
	activationLimitSpinBox->setValue(activationLimit);
	repeatLimitSpinBox->setValue(repeatLimit);
	loadingModeSpinBox->setValue(loadArgsEnable ? 1 : 0);
	deactivationModeComboBox->setCurrentIndex(deactivationModeComboBox->findText(StringOfDeactivationMode(deactivationMode)));
	activatedSpinBox->setValue(activated ? 1 : 0);
	inLineEdit->setText(eventExprParser.GetBooleanExpr().c_str());
	outLineEdit->setText(actionExprParser.GetEnumerationExpr().c_str());
	if(!inLineEdit->text().isEmpty())
	{
		inSpoiler->SetTitle(inLineEdit->text().replace("&", "&&"));
	}

	setWindowTitle("Trigger Editor");
}



void TriggerCreator::ActionExprChanged(QString _text)
{
	existActionsList->clear();

	actionParser.Parse(_text.toAscii().data());
	outArgsCompleteLabel->setText("Arguments complete: " + QString::number(existActionsList->count()) + "/" + QString::number(actionParser.GetAmountOfUniqueVariables()));
	SimpleList<StringANSI> args = actionParser.GetVariables();
	
	for(int i = 0; i < args.GetSize(); ++i)
	{
		if(File::_IsExist((AssetLibrary::_GetDir() + AbstractAction::_GetDir()) + args[i]->key + "." + AbstractAction::_GetFileSuffix()))
		{
			existActionsList->AddAction(args[i]->key.c_str());

			outArgsCompleteLabel->setText("Arguments complete: " + QString::number(existActionsList->count()) + "/" + QString::number(actionParser.GetAmountOfUniqueVariables()));
		}
	}
} 



void TriggerCreator::EventExprChanged(QString _text)
{ 
	existEventsList->clear();

	eventParser.Parse(_text.toAscii().data());
	inArgsCompleteLabel->setText("Arguments complete: " + QString::number(existEventsList->count()) + "/" + QString::number(eventParser.GetAmountOfUniqueVariables()));
	SimpleList<StringANSI> args = eventParser.GetVariables();
	
	for(int i = 0; i < args.GetSize(); ++i)
	{
		if(File::_IsExist((AssetLibrary::_GetDir() + AbstractEvent::_GetDir()) + args[i]->key + "." + AbstractEvent::_GetFileSuffix()))
		{
			existEventsList->AddEvent(args[i]->key.c_str());

			inArgsCompleteLabel->setText("Arguments complete: " + QString::number(existEventsList->count()) + "/" + QString::number(eventParser.GetAmountOfUniqueVariables()));
		}
		else if(File::_IsExist((AssetLibrary::_GetDir() + Trigger::_GetDir()) + args[i]->key + "." + Trigger::_GetFileSuffix()))
		{
			existEventsList->AddTrigger(args[i]->key.c_str());

			inArgsCompleteLabel->setText("Arguments complete: " + QString::number(existEventsList->count()) + "/" + QString::number(eventParser.GetAmountOfUniqueVariables()));
		}
		else if(File::_IsExist((AssetLibrary::_GetDir() + AbstractVariable::_GetDir()) + args[i]->key + "." + AbstractVariable::_GetFileSuffix()))
		{
			AbstractVariable* variable = AbstractVariable::_LoadFromFile(args[i]->key);
						
			if(variable)
			{
				if(variable->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
				{
					existEventsList->AddVariable(args[i]->key.c_str());

					inArgsCompleteLabel->setText("Arguments complete: " + QString::number(existEventsList->count()) + "/" + QString::number(eventParser.GetAmountOfUniqueVariables()));
				}
				delete variable;
			}
		}
	}
}



void TriggerCreator::ActionIsSelected(void)
{
	QList<QListWidgetItem*> list = existActionsList->selectedItems();

	if(!list.empty())
	{
		outLineEdit->SetSelectedArg(list.at(0)->text());
	}
	else
	{ 
		outLineEdit->SetSelectedArg("");
	}

	outLineEdit->ParseString();
	outLineEdit->FindSelectedArg();
}




void TriggerCreator::EventIsSelected(void)
{
	QList<QListWidgetItem*> list = existEventsList->selectedItems();

	if(!list.empty())
	{
		inLineEdit->SetSelectedArg(list.at(0)->text());
	}
	else
	{ 
		inLineEdit->SetSelectedArg("");
	}

	inLineEdit->ParseString();
	inLineEdit->FindSelectedArg();
}


void TriggerCreator::RefreshEventsLibrary(void)
{
	inLineEdit->ParseString();

	eventsLibraryTree->Refresh();

	EventExprChanged(eventParser.GetBooleanExpr().c_str());
} 




void TriggerCreator::RefreshVariablesLibrary(QString _name)
{ 
	inLineEdit->ParseString();

	eventsLibraryTree->RefreshVariable(_name);
	
	EventExprChanged(eventParser.GetBooleanExpr().c_str());
}


void TriggerCreator::RefreshTriggersLibrary(QString _name)
{ 
	inLineEdit->ParseString();

	eventsLibraryTree->RefreshTrigger(_name);
	
	EventExprChanged(eventParser.GetBooleanExpr().c_str());
} 



void TriggerCreator::RefreshEventsLibrary(QString _name)
{ 
	inLineEdit->ParseString();

	eventsLibraryTree->RefreshEvent(_name);
	
	EventExprChanged(eventParser.GetBooleanExpr().c_str());
}


void TriggerCreator::RefreshActionsLibrary(void)
{
	outLineEdit->ParseString();

	actionsLibraryTree->Refresh();

	ActionExprChanged(actionParser.GetEnumerationExpr().c_str());
} 





void TriggerCreator::RefreshActionsLibrary(QString _name)
{ 
	outLineEdit->ParseString();

	actionsLibraryTree->Refresh(_name);
	
	ActionExprChanged(actionParser.GetEnumerationExpr().c_str());
}



void TriggerCreator::UpdateActionList(void)
{
	existActionsList->clear();
	outArgsCompleteLabel->setText("Arguments complete: 0/0"); 

	SimpleList<StringANSI> args = actionParser.GetVariables();
	
	for(int i = 0; i < args.GetSize(); ++i)
	{
		if(File::_IsExist((AssetLibrary::_GetDir() + AbstractAction::_GetDir()) + args[i]->key + "." + AbstractAction::_GetFileSuffix()))
		{
			existActionsList->AddAction(args[i]->key.c_str());

			outArgsCompleteLabel->setText("Arguments complete: " + QString::number(existActionsList->count()) + "/" + QString::number(actionParser.GetAmountOfUniqueVariables()));
		}
	}
}




void TriggerCreator::UpdateEventList(void)
{
	existEventsList->clear();
	inArgsCompleteLabel->setText("Arguments complete: 0/0"); 

	SimpleList<StringANSI> args = eventParser.GetVariables();
	
	for(int i = 0; i < args.GetSize(); ++i)
	{
		if(File::_IsExist((AssetLibrary::_GetDir() + AbstractEvent::_GetDir()) + args[i]->key + "." + AbstractEvent::_GetFileSuffix()))
		{
			existEventsList->AddEvent(args[i]->key.c_str());

			inArgsCompleteLabel->setText("Arguments complete: " + QString::number(existEventsList->count()) + "/" + QString::number(eventParser.GetAmountOfUniqueVariables()));
		}
	}
}


void TriggerCreator::CreateNewAction(void)
{  
	ActionCreator* dialog = new ActionCreator();
	
	dialog->setAttribute(Qt::WA_DeleteOnClose);

	dialog->show();

	connect(dialog, SIGNAL(accepted()), this, SLOT(UpdateActionList()));

	connect(dialog, SIGNAL(ActionIsEdited(QString)), this, SLOT(RefreshActionsLibrary(QString)));
}



void TriggerCreator::CreateNewEvent(void)
{
	EventCreator* dialog = new EventCreator();
	
	dialog->setAttribute(Qt::WA_DeleteOnClose);

	dialog->show();

	connect(dialog, SIGNAL(accepted()), this, SLOT(UpdateEventList()));

	connect(dialog, SIGNAL(EventIsEdited(QString)), this, SLOT(RefreshEventsLibrary(QString)));
}



void TriggerCreator::IsOk(void)
{ 
	int32 errorCode;
	if(nameLineEdit->text().isEmpty())
	{
		ErrorDialog("Error", "Trigger name is not defined", 155, 40);
		return;
	}
	if((errorCode = BooleanExprParser::_IsNameValid(nameLineEdit->text().toAscii().data())) >= 0)
	{
		ErrorDialog("Error", "Trigger name is not valid: '" + QString(nameLineEdit->text().at(errorCode)) + "'", 190, 40);
		return;
	}
	{
		BooleanExprParser::ErrorType error = eventParser.Parse(inLineEdit->text().toAscii().data());

		if(error == BooleanExprParser::EXPR_NOT_FOUND || error == BooleanExprParser::NO_ERRORS)
		{}
		else
		{
			ErrorDialog("Error", "IN-string is not valid (" + QString(eventParser.GetCurrentErrorString().c_str()) + " '" + QString(eventParser.GetErrorArg().c_str()) + "')", 290, 40);
			return; 
		}
	}
	{
		EnumerationExprParser::ErrorType error = actionParser.Parse(outLineEdit->text().toAscii().data());

		if(error == EnumerationExprParser::EXPR_NOT_FOUND || error == EnumerationExprParser::NO_ERRORS)
		{}
		else
		{
			ErrorDialog("Error", "OUT-string is not valid (" + QString(actionParser.GetCurrentErrorString().c_str()) + " '" + QString(actionParser.GetErrorArg().c_str()) + "')", 290, 40);
			return; 
		}
	}
	trigger = new Trigger();
	trigger->Rename(nameLineEdit->text().toAscii().data());

	trigger->SetLoadArgsEnable(0);
	trigger->SetEventExpr(inLineEdit->text().toAscii().data());
	trigger->SetActionExpr(outLineEdit->text().toAscii().data());
	trigger->Variable<bool>::SetValue(activatedSpinBox->value() == 1 ? true : false);
	trigger->SetActivationLimit(activationLimitSpinBox->value());
	trigger->SetRepeatLimit(repeatLimitSpinBox->value());
	trigger->SetLoadArgsEnable(loadingModeSpinBox->value() == 1 ? true : false);
	int32 deactivationMode = IndexOfDeactivationMode(deactivationModeComboBox->itemText(deactivationModeComboBox->currentIndex()));
	trigger->SetDeactivationMode(deactivationMode);
	
	if(!fastCreatingEnable)
	{
		int assetType;
		if(File::_IsExist(AssetLibrary::_GetDir() + Trigger::_GetDir() + trigger->GetName() + "." + Trigger::_GetFileSuffix()))
		{
			WarningDialog warning("Warning", "Trigger '" + QString(trigger->GetName().c_str()) + "' is already exist. Rewrite file?", 200, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				delete trigger; 
				trigger = NIL;
				return;
			}
		}
		else if((assetType = AssetLibrary::_IsAssetExist(trigger->GetName())) != AssetLibrary::UNKNOWN_ASSET)
		{
			WarningDialog warning("Warning", "Name '" + QString(trigger->GetName().c_str()) + "' is already reserved (" + QString(trigger->GetName().c_str()) + "." + QString(AssetLibrary::_GetFileSuffixOfAssetType(assetType).c_str()) + "). Ignore?", 280, 90);
			
			if(warning.exec() == QDialog::Rejected)
			{
				delete trigger;
				trigger = NIL;
				return;
			}
		}

		trigger->SaveToFile();
		
		emit TriggerIsEdited(QString(trigger->GetName().c_str()));
		delete trigger;
		trigger = NIL;
	} 
	else
	{
		if(AssetLibrary::_GetAssets().FindObject(trigger->GetName()) != NIL)
		{
			ErrorDialog("Error", "Name '" + QString(trigger->GetName().c_str()) + "' is already reserved", 195, 40);
			delete trigger;
			trigger = NIL;
			return;
		} 
		File file((Editor::_GetWorkDir() + Editor::_GetTempDir() + nameLineEdit->text() + "." + Trigger::_GetFileSuffix().c_str()).toStdString(), File::REWRITE);
			
		if(trigger->SaveToFile(file))
		{
			if(file.Open(File::READ))
			{
				trigger->LoadFromFile(file);

				AssetLibrary::_GetAssets().AddObject(trigger);

				file.Remove();
			}
		}

		emit TriggerIsEdited(QString(trigger->GetName().c_str()));
	}

	accept();
}
