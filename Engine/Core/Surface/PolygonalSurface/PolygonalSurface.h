#ifndef POLYGONALSURFACE_H
#define POLYGONALSURFACE_H
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Surface/AbstractSurface/AbstractSurface.h>
#include <Engine/Core/Transformation/ModelView/ModelView.h>
#include <Engine/Core/Shape/Mesh/Mesh.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
#include <Engine/Core/Surface/PolygonalSurface/Hitbox/Hitbox.h>
class PolygonalSurface: public virtual AbstractSurface,
	                    public virtual ModelView
{
	private: class CollisionGroup: public MagicList<PolygonalSurface>,
                                   public MagicNode
	{
		private: int32 groupIndex;
        private: bool isDestroying;
		public: CollisionGroup(void);
		public: CollisionGroup(int32 _groupIndex);
		public: virtual ~CollisionGroup(void);
		public: int32 GetIndex(void)const;
		public: bool IsDestroying(void)const;
		public: bool AddObject(PolygonalSurface* _object);
		public: bool RemoveObject(PolygonalSurface* _object);
	};
	public: class CollisionInfo
	{
		public: PolygonalSurface* targetObject;
		public: StringANSI sourceMeshName;
		public: StringANSI targetMeshName;
		public: SimpleList<int32>& sourceMeshIntersectedPolygons;
		public: SimpleList<int32>& targetMeshIntersectedPolygons;
		public: CollisionInfo(PolygonalSurface* _targetObject, StringANSI _sourceMeshName, StringANSI _targetMeshName, SimpleList<int32>& _sourceMeshIntersectedPolygons, SimpleList<int32>& _targetMeshIntersectedPolygons): targetObject(_targetObject), sourceMeshName(_sourceMeshName), targetMeshName(_targetMeshName), sourceMeshIntersectedPolygons(_sourceMeshIntersectedPolygons), targetMeshIntersectedPolygons(_targetMeshIntersectedPolygons)
		{}
	};
	public: enum: int64
	{
		POLYGONAL_SURFACE_CLASS = 4398046511104
	};
	public: enum
	{
		CHECK_LINE_IN_POLYGON_MESSAGE = 19,
		CHANGE_MESH_MESSAGE = 121,
		CHANGE_COLLISION_GROUP_MESSAGE = 182,
		COLLISION_BETWEEN_OBJECTS_MESSAGE = 184,
		ENABLE_COLLISION_CHECKING_MESSAGE = 185,
		ENABLE_COLLISION_POLYGONS_INFO_MESSAGE = 186,
			CHECK_COLLISION_OBJECT_OBJECT_MESSAGE = 187,
			CHECK_COLLISION_GROUP_GROUP_MESSAGE = 183,
			CHECK_COLLISION_OBJECT_GROUP_MESSAGE = 254,
		CHANGE_HITBOX_MESSAGE = 188,
		CHANGE_HITBOX_ENABLE_MESSAGE = 200
	};
	friend class GraphicsSurface;
	friend class TexturingSurface;
	friend class TiledSpace3D;
	template<typename A, typename B> friend class Class2;
	private: MagicPointer<Mesh> mesh;
	private: MagicPointer<CollisionGroup> collisionGroup;
	private: int32 collisionGroupIndex;
	private: bool collisionEnable;
	private: bool collisionPolygonsInfoEnable;
	private: SimpleList<int32> intersectedPolygons;
	private: MagicPointer<Hitbox> hitbox;
	private: MagicList<PolygonalSurface> childrens;
	private: MagicPointer<PolygonalSurface> parent;
    private: static MagicBinaryTree<CollisionGroup> collisionGroups;
	public: PolygonalSurface(void);
	public: virtual ~PolygonalSurface(void);
	public: bool IsExist(void)const;
	public: virtual void Destroy(void);
    public: PolygonalSurface& operator = (const PolygonalSurface& _object);
	public: Sphere GetTransformedSphere(void);
	public: Circle GetTransformedCircle(void);
	public: Box GetTransformedBox(void);
	public: Quad GetTransformedQuad(void);
	public: virtual Vector3 GetAbsolutePosition(void);
	public: virtual Quaternion GetAbsoluteRotation(void);
	public: virtual Vector3 GetAbsoluteScale(void);
    public: virtual Matrix4x4 GetTransformation(void);
	protected: virtual void ResetTransformationValid(void);
	public: void SetCollisionEnable(bool _enable);
	public: bool IsCollisionEnabled(void)const;
	public: void SetCollisionGroupIndex(int32 _groupIndex);
	public: int32 GetCollisionGroupIndex(void)const;
	public: void SetCollisionPolygonsInfoEnable(bool _enable);
	public: bool IsCollisionPolygonsInfoEnabled(void)const;
	public: SimpleList<int32>& GetIntersectedPolygons(void);
	public: static int32 _GetAmountOfCollisionGroups(void);
	private: PolygonalSurface* GetRoot(void)const;
	private: void CollisionDetectedSingleReaction(PolygonalSurface* _targetObject, Mesh* _sourceMesh, Mesh* _targetMesh);
	private: void CollisionDetectedChainReaction(PolygonalSurface* _targetObject, Mesh* _sourceMesh, Mesh* _targetMesh);
	private: void UpdateChildrens(void);
	private: static bool _AddToCollisionGroup(PolygonalSurface* _object);
	private: static bool _RemoveFromCollisionGroup(PolygonalSurface* _object);
	public: static void _CheckCollision(int32 _groupIndex1, int32 _groupIndex2);
    public: static bool _CheckCollision(PolygonalSurface* _sourceObject, PolygonalSurface* _targetObject);
    public: static bool _CheckCollision(PolygonalSurface* _object, int32 _groupIndex1, int32 _groupIndex2);
	private: bool IsIntersected(PolygonalSurface* _object);
	public: bool IsBoundingVolumeIntersected(PolygonalSurface* _object);
	public: virtual int32 IsIntersected(Line _line, Vector3* _intersectionPoint = NIL);
	public: virtual bool IsIntersected(Line _line, int32 _index, Vector3* _intersectionPoint = NIL);
	public: virtual bool IsBoundingVolumeIntersected(Line _line);
	public: void BindHitbox(Hitbox* _hitbox);
	public: Hitbox* GetHitbox(void)const;
	private: void HitboxIsDestroyed(void);
	private: void HitboxIsCreated(void);
	public: virtual void BindMesh(Mesh* _mesh);
	public: Mesh* GetMesh(void)const;
	public: using AbstractObject::SaveToFile;
	public: virtual bool SaveToFile(File& _file);
	protected: void SaveDerivedToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	public: using AbstractObject::LoadFromFile;
	protected: virtual bool LoadFromFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
};



#endif 