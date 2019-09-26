#ifndef MESH_H
#define MESH_H
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/Shape/Quad/Quad.h>
#include <Engine/Core/Container/vector/vector.h>
#include <Engine/Core/Shape/Triangle/Triangle.h>
#include <Engine/Core/Shape/Circle/Circle.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Shape/ScreenQuad/ScreenQuad.h>
#include <Engine/Core/Shape/Box/Box.h>
#include <Engine/Core/Shape/Sphere/Sphere.h>

#include <map>
#include <utility>
using namespace std;
class Mesh: public virtual AbstractObject
{
	public: enum 
	{
		NO_SHAPE,
		CUSTOM,
		QUAD,
		TRIANGLE,
		CIRCLE,
		ICOSAHEDRON,
		ICOSPHERE,
		BOX,
		PYRAMID
	};
	public: enum: int64
	{
		MESH_CLASS = 4
	};
	public: enum 
	{
		SPLIT,
		DELAUNAY
	};
	public: enum
	{
		VERTICES_IN_POLYGON = 3
	};
	private: enum
	{
		DESTROY_MESH_MESSAGE = 119,
		INIT_MESH_MESSAGE = 120
	};
	public: enum
	{
		CREATE_QUAD_MESSAGE = 38,
		CREATE_BOX_MESSAGE = 280,
		CREATE_MESH_MESSAGE = 292
	};
	public: enum
	{
		BOUNDING_BOX,
		BOUNDING_QUAD,
		BOUNDING_SPHERE,
		BOUNDING_CIRCLE
	};
    friend class GraphicsSurface;
	friend class TexturingSurface;
	private: vector<Vector3> vertices;
    private: vector<uint32> indices;
	private: int32 amountOfPolygons;

    private: int32 shapeType;

	private: int32 boundingVolumeType;
	private: Box boundingBox;
    private: Sphere boundingSphere;
    private: static StringANSI meshesSuffix;
    private: static StringANSI meshesDir;
	public: Mesh(void);
	public: virtual ~Mesh(void);
	public: bool IsExist(void)const;
	public: virtual void Destroy(void);
	public: void SetBoundingVolumeType(int32 _type);
	public: int32 GetBoundingVolumeType(void)const;
	private: void UpdateBoundingVolume(void);
	public: Quad GetBoundingQuad(void);
	public: Circle GetBoundingCircle(void);
	public: Sphere GetBoundingSphere(void);
	public: Box GetBoundingBox(void);
	private: void ResizeIndices(int32 _size);
	public: void SetIndex(int32 _index, uint32 _value);
	public: uint32 GetIndex(int32 _index);
	private: void DestroyIndices(void);
	public: bool IsIndicesExist(void)const;
	public: int32 GetAmountOfIndices(void)const;
	private: void ResizeVertices(int32 _size);
	private: void DestroyVertices(void);
	public: void SetVertex(int32 _index, Vector3 _vertex);
	public: Vector3 GetVertex(int32 _index)const;
	public: bool IsVerticesExist(void)const;
	public: int32 GetAmountOfVertices(void)const;
	public: bool CreateShape(SimpleList<Vector3>& _triangles);
	public: bool CreateShape(SimpleList<Triangle>& _triangles);
	public: bool CreateShape(SimpleList<Vector3>& _vertices, int32 _triangulationType);
	public: bool IsClockwise(void);
	public: bool IsCountClockwise(void);
	public: void InverseVertices(void);
	public: int32 GetAmountOfPolygons(void)const;
	public: bool TotalPolygonsDivide4(void);
	public: void TotalVertexNormalize(void);
	public: int32 GetShapeType(void)const;
	public: void CreateQuad(Quad _quad);
	public: void CreateTriangle(Triangle _triangle);
	public: void CreateCircle(Circle _circle, int32 _segments = 3);
	public: void CreateBox(Box _box);
	public: void CreateIcosahedron(void);
	public: void CreateIcosphere(int32 _level = 1);
	public: void CreatePyramid(float _radius);
	public: static bool _IsConvex(Vector3 _point, const SimpleList<Vector3>& _polygon);
	public: bool GetTriangle(int32 _index, Triangle& _triangle);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static Mesh* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static Mesh* _LoadFromFile(File& _file);
};



#endif 