#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
#include <Engine/Core/Container/vector/vector.h>
#include <Engine/Output/Graphics/Scene/GraphicsScene/RenderList/RenderList.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Output/Graphics/Scene/GraphicsContainer/GraphicsContainer.h>
class PixelBuffer;
class GraphicsScene: public GraphicsContainer
{
	public: enum: int64
	{
		GRAPHICS_SCENE_CLASS = 128
	};
	public: enum
	{
		ADD_TO_RENDER_LIST_MESSAGE = 18,
		REMOVE_FROM_RENDER_LIST_MESSAGE = 45
	};
	friend class GraphicsObject;
	friend class RenderList;
	friend class Scene;
	template<typename A, typename B> friend class Class2;
	private: RenderList renderList;
	public: GraphicsScene(void);
	public: virtual ~GraphicsScene(void);
	protected: virtual void ResetTransformationValid(void);
	private: void UpdateTransformation(MagicList<GraphicsObject>::AddressDerived* _address)const;
	public: virtual RenderList::AddressDerived* AddToRenderList(GraphicsObject* _object);
	public: virtual RenderList::AddressDerived* AddToRenderList(GraphicsObject* _parent, GraphicsObject* _object);
	public: virtual void Destroy(void);
	public: virtual void RemoveFromRenderList(GraphicsObject* _object);
	public: virtual void Draw(void);
	public: RenderList& GetRenderList(void);
	public: int32 GetAmountOfGraphicsObjects(void);
	private: void GetAmountOfGraphicsObjects(MagicList<GraphicsObject>::AddressDerived* _address, int32& _size)const;
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void SaveDerivedToFile(File& _file);
	protected: void SaveGraphicsObjectToFile(MagicList<GraphicsObject>::AddressDerived* _address, File& _file);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
	protected: void LoadGraphicsObjectFromFile(File& _file);
};



#endif 