#ifndef OBJECTSCENEITEM_H
#define OBJECTSCENEITEM_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/MessageHandler/MessageHandler.h>
#include <Engine/Helpers/Scene/Scene.h>
class ObjectTreeWidget;
class ObjectSceneItem: public QTreeWidgetItem,
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
		TIMER,
		HITBOX,
		SCENE,
		FLOAT_LERP,
		QUATERNION_LERP,
		VECTOR_LERP,
		CAMERA
	};
	friend class ObjectTreeWidget;
	private: int64 objectType;
	private: AbstractObject* object;
	private: ObjectTreeWidget* treeWidget;
	public: ObjectSceneItem(Type _assetType);
	public: virtual ~ObjectSceneItem(void);
	public: void SetObjectClass(int64 _classType);
	public: int64 GetObjectClass(void)const;
	public: AbstractObject* GetObjectPtr(void)const;
	public: void SetObjectPtr(AbstractObject* _object);
	public: void AddToObjectScene(AbstractObject* _object);
	public: void RemoveFromObjectScene(AbstractObject* _object);
};



#endif 

