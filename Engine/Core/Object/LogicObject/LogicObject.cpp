#include "LogicObject.h"
#include <Engine/Core/Scene/LogicScene/LogicScene.h>



LogicObject::LogicObject(void): freezeEnable(false), deltaScale(1.0f), totalDeltaScale(1.0f)
{
	AddClassProperty(LOGIC_OBJECT_CLASS);
	logicContainer.SetRemoveMode(MagicContainer::REMOVE_NODE);
}


LogicObject::~LogicObject(void)
{
	Destroying();
}


LogicObject* LogicObject::GetLogicContainer(void)const
{
	return logicContainer.Get();
}


float LogicObject::GetDeltaScale(void)const
{
	return totalDeltaScale;
}


void LogicObject::SetDeltaScale(float _scale)
{
	if(_scale > 0.0f)
	{
		deltaScale = _scale;
		totalDeltaScale = _scale;

		if(logicContainer.IsNotEmpty())
		{
			totalDeltaScale *= logicContainer.Get()->GetDeltaScale(); 
		}
	}
}


void LogicObject::ResetDeltaScale(void)
{
	totalDeltaScale = deltaScale;
}


void LogicObject::UpdateDeltaScale(void)
{
	totalDeltaScale = deltaScale * logicContainer.Get()->GetDeltaScale(); 
}




void LogicObject::SetFreezeEnable(bool _enable)
{
	freezeEnable = _enable; 
}



bool LogicObject::IsFreezeEnabled(void)const
{
	return freezeEnable;
}




void LogicObject::SaveDerivedToFile(File& _file)
{
	_file.Write(freezeEnable);
	_file.Write(deltaScale);
}



void LogicObject::LoadDerivedFromFile(File& _file)
{
	_file.Read(freezeEnable);
	_file.Read(deltaScale);
	totalDeltaScale = deltaScale;
}