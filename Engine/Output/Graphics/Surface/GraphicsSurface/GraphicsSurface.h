#ifndef GRAPHICSSURFACE_H
#define GRAPHICSSURFACE_H
#include <Engine/Core/Surface/PolygonalSurface/PolygonalSurface.h>
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Core/Shape/Triangle/Triangle.h>
#include <Engine/Output/File/File.h>
class GraphicsSurface: public PolygonalSurface,
	                   public virtual GraphicsObject
{
	public: enum: int64
	{
		GRAPHICS_SURFACE_CLASS = 16
	};
	public: enum 
	{
		SHOW_WIREFRAME_BOUNDING_VOLUME = 1,
		SHOW_SHORTEST_BOUNDING_VOLUME = 2,
		SHOW_TRANSFORMED_BOUNDING_VOLUME = 4,
		SHOW_BOUNDING_BOX = 8,
		SHOW_BOUNDING_QUAD = 16,
		SHOW_BOUNDING_SPHERE = 32,
		SHOW_BOUNDING_CIRCLE = 64
	};
	public: enum
	{
		POLYGONAL_SURFACE_GRAPHICS_TYPE = 1,
		BACK_FACE_CULLING_GRAPHICS_TYPE = 2,
		SHOW_BOUNDING_VOLUME_GRAPHICS_TYPE = 4
	};
	public: enum
	{
		ENABLE_BACK_FACE_CULLING_MESSAGE = 87,
		ENABLE_STATIC_OBJECT_MESSAGE = 290
	};
	friend class TexturingSurface;
	private: Caller<> draw;
	private: Vector4 boundingVolumeColor;
	private: int32 boundingVolumeShowMode;
    private: bool staticEnable;
	private: uint32 displayListIndex;
	private: static float boundingVolumeScale;
	private: static float boundingVolumeCornersLength;
	public: GraphicsSurface(void);
	public: virtual ~GraphicsSurface(void);
	public: virtual void Destroy(void);
	public: void SetStaticEnable(bool _enable);
	public: bool IsStaticEnabled(void)const;
    public: GraphicsSurface& operator = (const GraphicsSurface& _object);
	public: virtual Vector3 GetAbsolutePosition(void);
	public: virtual Quaternion GetAbsoluteRotation(void);
	public: virtual Vector3 GetAbsoluteScale(void);
    public: virtual Matrix4x4 GetTransformation(void);
	public: void SetBoundingVolumeShowEnable(bool _enable);
	public: bool IsBoundingVolumeShowEnabled(void)const;
	public: void SetBoundingVolumeColor(Vector4 _color);
	public: void SetBoundingVolumeColor(RGBA _color);
	public: Vector4 GetBoundingVolumeColor(void);
	public: void SetBoundingVolumeShowMode(int32 _mode);
	public: int32 GetBoundingVolumeShowMode(void)const;
	public: void SetBackFaceCullingEnable(bool _enable);
    public: bool IsBackFaceCullingEnabled(void)const;
	protected: virtual void ModifyGraphicsType(int64 _property, bool _enable);
	public: virtual void Draw(void);
	public: static void _DrawTriangle(Triangle _triangle, Vector3 _color, float _alpha = 1.0f);
	public: static void _DrawTriangle(Triangle _triangle, RGBA _color);
	public: static void _DrawCulledTriangle(Triangle _triangle, Vector3 _color, float _alpha = 1.0f);
	public: static void _DrawCulledTriangle(Triangle _triangle, RGBA _color);
	private: void CustomDraw(void);
	private: void DrawCulledPolygonalShape(void);
	private: void DrawCulledPolygonalShapeWithBoundingVolume(void);
	private: void DrawPolygonalShapeWithBoundingVolume(void);
	private: void DrawPolygonalShape(void);
	private: void DrawBoundingVolume(void);
    private: void DrawTransformedBoundingBox(void);
    private: void DrawAlignedBoundingBox(void);
    private: void DrawTransformedBoundingQuad(void);
    private: void DrawAlignedBoundingQuad(void);
    private: void DrawTransformedBoundingCircle(void);
    private: void DrawAlignedBoundingCircle(void);
	public: virtual void BindMesh(Mesh* _mesh);
	private: void MeshIsDestroyed(void);
	private: void MeshIsCreated(void);
	public: static void _SetStippleEnable(bool _enable);
    public: static bool _IsStippleEnabled(void);
    public: static void _SetStipple(uint8* _stipple);
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