#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Transformation/ModelView/ModelView.h>
#include <Engine/Output/Graphics/Object/ColorableObject/ColorableObject.h>
class GraphicsObject: public virtual ColorableObject,
	                  public virtual ModelView
{
	public: enum: int64
	{
		GRAPHICS_OBJECT_CLASS = 8
	};
	public: enum
	{
		ENABLE_SHOW_MESSAGE = 235
	};
	friend class RenderList;
	friend class GraphicsScene;
	friend class TiledSpace3D;
	friend class GraphicsLine;
	friend class GraphicsPoint;
	friend class GraphicsQuad;
	friend class GraphicsCircle;
	friend class GraphicsPolygon;
	friend class GraphicsStrip;
	friend class TexturingSurface;
	friend class GraphicsSurface;
	friend class Graph;
	friend class Function;
	private: Vector3 color;
	private: float alpha;
	private: int64 graphicsType;
	private: MagicPointer<GraphicsObject> graphicsContainer;
	private: bool showEnable;
	public: GraphicsObject(void);
	public: virtual ~GraphicsObject(void);
    public: GraphicsObject& operator = (const GraphicsObject& _object);
	public: virtual Vector3 GetAbsolutePosition(void);
	public: virtual Quaternion GetAbsoluteRotation(void);
	public: virtual Vector3 GetAbsoluteScale(void);
	public: void BeginTransform(void);
	public: void EndTransform(void);
    public: virtual Matrix4x4 GetTransformation(void);
	public: void SetShowEnable(bool _enable);
	public: bool IsShowEnabled(void)const;
	protected: virtual void ModifyGraphicsType(int64 _property, bool _enable);
	public: int64 GetGraphicsType(void)const;
	public: bool IsGraphicsTypeEnabled(int64 _property)const;
	public: StringANSI GetGraphicsTypeString(void)const;
	public: virtual void Draw(void) = 0;
	public: GraphicsObject* GetGraphicsContainer(void)const;
	public: virtual void SetColorRGB(Vector3 _rgb);
	public: virtual void DisplaceColorRGB(Vector3 _offset);
	public: virtual Vector3 GetColorRGB(void);
	public: virtual void SetRed(float _red);
	public: virtual void SetGreen(float _green);
	public: virtual void SetBlue(float _blue);
	public: virtual void SetAlpha(float _alpha);
	public: virtual float GetAlpha(void);
	public: virtual void SetColorRGBA(Vector4 _rgba);
	public: virtual Vector4 GetColorRGBA(void);
	public: virtual void SetColor(RGBA _color);
	public: virtual void SetColorHSL(Vector3 _hsl);
	public: virtual Vector3 GetColorHSL(void);
	public: virtual void SetHue(float _hue);
	public: virtual void SetHue360(float _hue);
	public: virtual void SetSaturation(float _saturation);
	public: virtual void SetLightness(float _lightness);
	protected: void SaveDerivedToFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
};

#endif 