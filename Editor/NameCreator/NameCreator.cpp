#include "NameCreator.h"
#include <Engine/Core/Parser/BooleanExprParser/BooleanExprParser.h>



NameCreator::NameCreator(QWidget* _parent): QDialog(_parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), assetLibraryCheckEnable(false) 
{
	nameMaxLength = 256;
	nameLabel = new QLabel(this);
    nameLabel->setText("Name:");
    nameLabel->setGeometry(QRect(20, 10, 81, 41));
	nameLineEdit = new QLineEdit(this);
    nameLineEdit->setGeometry(QRect(100-25-5-5, 20, 170+50, 20));
    nameLineEdit->setMaxLength(nameMaxLength);
	nameLineEdit->setFocus();
	nameLineEdit->setToolTip("1 - " + QString::number(nameMaxLength) + " symbols");
	cancelButton = new QPushButton(this);
    cancelButton->setText("Cancel");
	cancelButton->setGeometry(QRect(190+20+20+5-25-5-5, 60-5, 81+5, 23));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    okButton = new QPushButton(this);
    okButton->setText("Ok");
    okButton->setGeometry(QRect(100+20+20-25-5-5, 60-5, 81+5, 23));
	connect(okButton, SIGNAL(clicked()), SLOT(IsOk()));
	

	setWindowTitle("Rename");
	setFixedSize(350-25-10-5-5, 100-10);
}



void NameCreator::SetAssetLibraryCheckEnable(bool _state)
{
	assetLibraryCheckEnable = _state;
}


bool NameCreator::IsAssetLibraryCheckEnabled(void)const
{
	return assetLibraryCheckEnable;
}



void NameCreator::SetNameList(QList<QString>& _nameList)
{
	nameList = _nameList;
}
 

void NameCreator::SetName(QString _name)
{
	nameLineEdit->setText(_name);
}



QString NameCreator::GetName(void)
{
	return nameLineEdit->text();
}


 
void NameCreator::IsOk(void)
{
	int errorCode;

	if(nameLineEdit->text().isEmpty())
	{
		ErrorDialog("Error", "Name is not defined", 125, 40);
		return;
	}

	if((errorCode = BooleanExprParser::_IsNameValid(nameLineEdit->text().toStdString())) >= 0)
	{
		ErrorDialog("Error", "Name is not valid: '" + QString(nameLineEdit->text().at(errorCode)) + "'", 210, 40);
		return;
	}

	emit PickName(nameLineEdit->text());
	if(!nameList.isEmpty())
	{
		int index = nameList.indexOf(nameLineEdit->text());

		if(index != -1)
		{
			ErrorDialog("Error", "Name '" + nameLineEdit->text() + "' is already reserved", 230, 40);
			return;
		}
	}
	if(assetLibraryCheckEnable)
	{
		if(AssetLibrary::_GetAssets().FindObject(nameLineEdit->text().toStdString()))
		{
			ErrorDialog("Error", "Name '" + nameLineEdit->text() + "' is already reserved", 230, 40);
			return;
		}
	}
	accept();
}



QObject* NameCreator::ConvertToQObject(void)
{
	return static_cast<QObject*>(this);
}



