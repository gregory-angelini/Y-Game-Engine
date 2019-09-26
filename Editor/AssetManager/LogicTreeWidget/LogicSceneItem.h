#ifndef LOGICSCENEITEM_H
#define LOGICSCENEITEM_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Object/LogicObject/LogicObject.h>
#include <Engine/Core/MessageHandler/MessageHandler.h>
class LogicTreeWidget;
class LogicSceneItem: public QTreeWidgetItem,
	                   public MessageHandler<int32>::Receiver
{
	public: enum Type
	{
		ATLAS = 0,
		TEXTURE,
		TEXTURED_ANIMATION,
		TRIGGER,
		EVENT,
		ACTION,
		VARIABLE,
		LIST,
		OBJECT,
		PLAYER_CONTROL,
		MESH,
		HITBOX,
		SCENE,
		FLOAT_LERP,
		TIMER,
		QUATERNION_LERP,
		VECTOR_LERP,
		CAMERA
	};
	friend class LogicTreeWidget;
	private: int64 objectType;
	private: LogicObject* object;
	private: LogicTreeWidget* treeWidget;
	public: LogicSceneItem(QTreeWidgetItem* _parent, Type _assetType, int64 _objectType);
	public: int64 GetObjectType(void)const;
	public: LogicObject* GetObjectPtr(void)const;
	public: void SetObjectPtr(LogicObject* _object);
	public: void AddToLogicScene(LogicObject* _object);
	public: void RemoveFromLogicScene(LogicObject* _object);
};



#endif 

