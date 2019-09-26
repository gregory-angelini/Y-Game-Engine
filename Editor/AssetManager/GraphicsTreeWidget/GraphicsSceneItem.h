#ifndef GRAPHICSSCENEITEM_H
#define GRAPHICSSCENEITEM_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Core/MessageHandler/MessageHandler.h>
class GraphicsTreeWidget;
class GraphicsSceneItem: public QTreeWidgetItem,
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
		TILED_SPACE,
		FLOAT_LERP,
		QUATERNION_LERP,
		VECTOR_LERP,
		CAMERA
	};
	friend class GraphicsTreeWidget;
	private: int64 objectType;
	private: GraphicsObject* object;
	private: GraphicsTreeWidget* treeWidget;
	public: GraphicsSceneItem(QTreeWidgetItem* _parent, Type _assetType, int64 _objectType);
	public: int64 GetObjectType(void)const;
	public: GraphicsObject* GetObjectPtr(void)const;
	public: void SetObjectPtr(GraphicsObject* _object);
	public: void AddToGraphicsScene(GraphicsObject* _object);
	public: void RemoveFromGraphicsScene(GraphicsObject* _object);
};



#endif 

