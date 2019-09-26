#ifndef HITBOX_H
#define HITBOX_H
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Surface/AbstractSurface/AbstractSurface.h>
#include <Engine/Core/Transformation/ModelView/ModelView.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
class Hitbox: public AbstractObject
{
	public: enum: int64
	{
		HITBOX_CLASS = 288230376151711744
	};
	public: class ShapeInfo: public MagicNode
    {
		friend class Hitbox;
		friend class PolygonalSurface;
		private: MagicPointer<Mesh> mesh;
		private: StringANSI name;
		private: int32 collisionGroupIndex;
		private: bool collisionEnable;
		private: bool collisionPolygonsInfoEnable;
		public: ShapeInfo(void);
		public: ShapeInfo(Mesh* _mesh, StringANSI _name, int32 _collisionGroupIndex, bool _collisionEnable = true, bool _collisionPolygonsInfoEnable = false);
		public: Mesh* GetMesh(void)const;
		public: StringANSI GetName(void)const;
		public: int32 GetCollisionGroupIndex(void)const;
		public: bool GetCollisionEnable(void)const;
		public: bool GetCollisionPolygonsInfoEnable(void)const;
		public: virtual bool SaveToFile(File& _file);
		public: virtual bool LoadFromFile(File& _file);
	};
	private: enum
	{
		DESTROY_HITBOX_MESSAGE = 189,
		INIT_HITBOX_MESSAGE = 190
	};
	friend class PolygonalSurface;
	friend class GraphicsSurface;
	private: MagicList<ShapeInfo> shapes;
    private: static StringANSI hitboxesSuffix;
    private: static StringANSI hitboxesDir;
	public: Hitbox(void);
	public: virtual ~Hitbox(void);
	public: bool IsExist(void)const;
	public: virtual void Destroy(void);
	public: bool AddShape(Mesh* _mesh, StringANSI _name, int32 _collisionGroupIndex, bool _collisionEnable = true, bool _collisionPolygonsInfoEnable = false);
	public: bool RemoveShape(StringANSI _name);
	public: MagicList<ShapeInfo>& GetShapes(void);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static Hitbox* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static Hitbox* _LoadFromFile(File& _file);
};



#endif 