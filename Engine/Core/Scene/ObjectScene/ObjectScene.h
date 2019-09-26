#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H
#include <Engine/Core/Scene/ObjectScene/ObjectTree/ObjectTree.h>
class ObjectScene: public virtual AbstractObject
{
	public: enum
	{
		ADD_TO_LAYER_MESSAGE = 14,
		REMOVE_FROM_LAYER_MESSAGE = 15
	};
	public: enum: int64
	{
		OBJECT_SCENE_CLASS = 8388608
	};
	friend class GraphicsScene;
	friend class LogicScene;
	friend class ObjectTree;
	friend class UpdateList;
	friend class RenderList;
	friend class Scene;
	friend class TiledSpace3D;
	friend class PolygonalSurface;
	friend class GraphicsContainer;
	private: ObjectTree objectTree;
	private: static StringANSI scenesSuffix;
    private: static StringANSI scenesDir;
	public: ObjectScene(void);
	public: virtual ~ObjectScene(void);
	public: virtual ObjectBinaryTree::AddressDerived* AddToLayer(AbstractObject* _object);
	public: int32 GetAmountOfObjects(void)const;
	private: void GetAmountOfObjects(MagicBinaryTree<AbstractObject>::AddressDerived* _address, int32& _size)const;
	public: AbstractObject* FindObject(StringANSI _name);
	public: ObjectTree& GetObjectTree(void);
	public: virtual void Destroy(void);
	public: virtual void RemoveFromLayer(AbstractObject* _object);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void SaveDerivedToFile(File& _file);
	protected: virtual void SaveObjectToFile(MagicBinaryTree<AbstractObject>::AddressDerived* _address, File& _file);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
    public: static ObjectScene* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static ObjectScene* _LoadFromFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
	protected: virtual void LoadObjectFromFile(File& _file);
};


#endif 