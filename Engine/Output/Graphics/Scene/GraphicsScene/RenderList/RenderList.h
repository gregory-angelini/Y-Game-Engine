#ifndef RENDERLIST_H
#define RENDERLIST_H
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
class RenderList: public MagicList<GraphicsObject>,
	              public MessageHandler<int32>::Receiver
{
	friend class GraphicsScene;
	friend class TiledSpace3D;
	private: GraphicsScene* scene;
	public: RenderList(void);
	public: virtual ~RenderList(void);
	public: AddressDerived* AddObject(GraphicsObject* _object);
	public: AddressDerived* AddObject(GraphicsObject* _parent, GraphicsObject* _object);
	private: bool IsParentScene(GraphicsScene* _scene);
	private: bool IsParentScene(TiledSpace3D* _tiledSpace);
	public: virtual void RemoveNodeEvent(MagicNode* _node);
	public: virtual void AddNodeEvent(MagicNode* _node);
	public: AbstractObject* FindObject(StringANSI _name);
};



#endif 