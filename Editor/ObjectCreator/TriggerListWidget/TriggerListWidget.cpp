
#include "TriggerListWidget.h"
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Editor/TriggerCreator/EventCreator/EventCreator.h>
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Editor/TriggerCreator/VariableCreator/VariableCreator.h>



TriggerListWidget::TriggerListWidget(QWidget* _parent): QListWidget(_parent)
{
	setSortingEnabled(true);

	setIconSize(QSize(25, 25));
}




void TriggerListWidget::SetFrames(QList<QPixmap> _frames)
{
	frames = _frames;
}



TriggerListWidgetItem* TriggerListWidget::AddItem(QString _name, bool _animated)
{
	QList<QListWidgetItem*> search = findItems(_name, Qt::MatchExactly);

	if(search.isEmpty())
	{
		TriggerListWidgetItem* item = new TriggerListWidgetItem;
	
		item->setText(_name);

		item->SetFrames(frames);

		if(_animated)
		{ 
			item->Play(); 
			item->SetFPS(2.0f);
		} 
	
		addItem(item);
		return item;
	}
	return NIL;
}





